use owo_colors::OwoColorize;
use rustyline::Result;
use std::collections::HashMap;
use std::process::Command;

/// Execute a command
/// - `command`: The command's name
/// - `builtins`: List of builtins commands to look for builtins
///
/// Example:
/// ```
/// let builtins = shard::builtin::get_builtins();
/// let cmd_result = shard::command::execute_command("echo \"Hello, World!\"", &builtins);
/// ```
pub fn execute_command(
    command: &str,
    builtins: &HashMap<&str, Box<dyn Fn(&[&str]) -> Result<()>>>,
) -> bool {
    let parts: Vec<&str> = command.split_whitespace().collect();

    if parts.is_empty() {
        false;
    }

    let parts: Vec<&str> = command.trim().split_whitespace().collect();

    if parts.is_empty() {
        false;
    }

    let cmd = parts[0];
    let args = &parts[1..];

    if let Some(builtin_fn) = builtins.get(cmd) {
        let _ = builtin_fn(args);
        true
    } else {
        match Command::new(parts[0]).args(&parts[1..]).output() {
            Ok(output) => {
                if !output.stdout.is_empty() {
                    print!("{}", String::from_utf8_lossy(&output.stdout));
                }
                if !output.stderr.is_empty() {
                    eprint!("{}", String::from_utf8_lossy(&output.stderr));
                }
                if output.status.success() {
                    return true;
                } else {
                    return false;
                }
            }
            Err(e) => {
                if e.kind() == std::io::ErrorKind::NotFound {
                    eprintln!("{}: ", "shard".blue().bold());
                    eprintln!(
                        "| {}: '{}' not found",
                        "Error".red().bold(),
                        parts[0].red().bold()
                    );
                    eprintln! {"| {}: are you sure '{}' exists?", "Tip".cyan().bold().underline(), parts[0].magenta().bold()};
                } else {
                    eprintln!("shard: ");
                    eprintln!(
                        "| {} running '{}': {}",
                        "Error".red().blue(),
                        parts[0].red().bold(),
                        e
                    );
                }
            }
        }
        false
    }
}
