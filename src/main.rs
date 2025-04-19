use chrono::Local;
use owo_colors::OwoColorize;
use rustyline::error::ReadlineError;
use rustyline::{DefaultEditor, Result};
use std::env;

mod shard;

fn main() -> Result<()> {
    let mut rl = DefaultEditor::new()?;
    let mut status: i32 = 0;
    loop {
        let user = env::var("USER").unwrap_or_else(|_| "guest".to_string());
        let now = Local::now();
        let time_str = now.format("%H:%M").to_string();
        println!("as {} at {}", user.green(), time_str.magenta());
        let prompt_suffix = if status == 0 {
            format!("{}", ">> ".green())
        } else {
            format!("{}", "x> ".red())
        };
        let cwd = env::current_dir()?;
        let prompt = format!("{} {}", cwd.display().blue(), prompt_suffix);

        let readline = rl.readline(&prompt);
        match readline {
            Ok(line) => {
                rl.add_history_entry(line.as_str())?;
                let builtins = shard::builtin::get_builtins();
                if shard::command::execute_command(line.as_str(), &builtins) {
                    status = 0
                } else {
                    status = 1
                }
            }
            Err(ReadlineError::Interrupted) | Err(ReadlineError::Eof) => {
                break;
            }
            Err(err) => {
                println!("Error: {:?}", err);
                status = 1;
            }
        }
        println!("");
    }
    Ok(())
}
