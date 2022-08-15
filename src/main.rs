mod compiler;
mod exercise;
#[macro_use]
mod ui;
use clap::{Parser, Subcommand};
use crate::exercise::ExerciseList;

/// Small C++ code exercises to get you used to C++ code.
#[derive(Parser)]
#[clap(author, about, version)]
struct Args {
    #[clap(short)]
    g: Option<String>,

    #[clap(subcommand)]
    command: Option<Commands>,
}

#[derive(Subcommand, Clone)]
enum Commands {
    Verify,
    /// Run pre++ in watch mode
    Watch,
    /// Run a pre++ exercise
    Run {
        /// Name of exercise
        name: String,
    },
    /// Get a hint about an+ exercise
    Hint {
        /// Name of exercise
        name: String,
    },
    /// List all available exercises
    List,
}

fn main() {
    let _args = Args::parse();
    if _args.g.is_some() {
        let path = &_args.g.unwrap();
        let path = std::path::Path::new(path);
        if !std::path::Path::new(path.parent().unwrap()).exists() {
            std::fs::create_dir(path.parent().unwrap()).expect("could not create parent dir.");
        }
        std::fs::File::create(path).expect("Couldnt create file.");
    }

    let toml_str = &std::fs::read_to_string("info.toml").unwrap();
    let exercises = toml::from_str::<ExerciseList>(toml_str).unwrap().exercise;

    match _args.command.unwrap() {
        Commands::List => crate::exercise::list(),
        Commands::Hint { name } => {
            let exercise = exercise::find_exercise(&name, &exercises);
            println!("{}", exercise.hint);
        }
        Commands::Run { name } => {
            let exercise = exercise::find_exercise(&name, &exercises);
            exercise::run(exercise).unwrap_or_else(|_| std::process::exit(1));
        }
        _ => (),
    }
}
