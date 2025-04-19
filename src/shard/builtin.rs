use rustyline::Result;
use std::collections::HashMap;
use std::env;

fn echo(args: &[&str]) -> Result<()> {
    println!("{}", args.join(" "));
    Ok(())
}

fn exit(args: &[&str]) -> Result<()> {
    std::process::exit(args[0].parse::<i32>().unwrap_or_else(|_| 0));
}

fn pwd(_args: &[&str]) -> Result<()> {
    let cwd = env::current_dir()?;
    println!("{}", cwd.display());
    Ok(())
}

pub fn get_builtins() -> HashMap<&'static str, Box<dyn Fn(&[&str]) -> Result<()>>> {
    let mut builtins: HashMap<&'static str, Box<dyn Fn(&[&str]) -> Result<()>>> = HashMap::new();

    builtins.insert("echo", Box::new(echo));
    builtins.insert("exit", Box::new(exit));
    builtins.insert("pwd", Box::new(pwd));

    builtins
}
