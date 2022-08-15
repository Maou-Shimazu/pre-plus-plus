#![allow(unused)]
use crate::compiler::*;
use core::fmt;
use indicatif::ProgressBar;
use serde::{Deserialize, Serialize};
use std::{collections::HashMap, fmt::Formatter, process::Command};

const I_AM_DONE_REGEX: &str = r"(?m)^\s*///?\s*I\s+AM\s+NOT\s+DONE";

#[derive(PartialEq, Debug)]
pub struct ContextLine {
    pub line: String,
    pub number: usize,
    pub important: bool,
}

#[derive(PartialEq, Debug)]
pub enum State {
    Done,
    Pending(Vec<ContextLine>),
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Exercise {
    pub name: String,
    pub path: String,
    pub hint: String,
    pub mode: String,
}

#[derive(Debug)]
pub struct ExerciseOutput {
    pub stdout: String,
    pub stderr: String,
}

impl Exercise {
    pub fn compile(&self) -> Result<ExerciseOutput, ()> {
        let cmd = match self.mode.as_str() {
            "compile" => Command::new(match cpp().unwrap() {
                Compilers::Clangpp => "clang++",
                Compilers::Gpp => "g++",
                Compilers::Msvc => "", // note: unimplimented
            })
            .args(&[self.path.as_str(), "-o", &self.name])
            .output(),
            // "test" => (),
            _ => std::process::exit(0),
        }
        .expect("Failed to compile exercise.");
        if cmd.status.success() {
            Ok(ExerciseOutput {
                stdout: String::from_utf8_lossy(&cmd.stdout).to_string(),
                stderr: String::from_utf8_lossy(&cmd.stderr).to_string(),
            })
        } else {
            Err(())
        }
    }
}

impl fmt::Display for Exercise {
    fn fmt(&self, f: &mut Formatter<'_>) -> Result<(), std::fmt::Error> {
        write!(f, "{} ({})", self.name, self.path)
    }
}

#[derive(Deserialize)]
pub struct ExerciseList {
    pub exercise: Vec<Exercise>,
}

pub fn list() {
    let config: HashMap<String, Vec<Exercise>> =
        toml::from_str(&std::fs::read_to_string("info.toml").unwrap()).unwrap();
    let items: &[Exercise] = &config["exercise"];
    println!("{:<17}\t{:<46}\t{:<7}", "Name", "Path", "Status");
    for i in items {
        println!("{:<17}\t{:<46}\t{:<7}", i.name, i.path, i.mode);
    }
    println!("Progress: You completed 0 / 84 exercises (0.00 %)."); // warning: update this appropriately
}

pub fn find_exercise<'a>(name: &str, exercises: &'a [Exercise]) -> &'a Exercise {
    // if name.eq("next") {
    // exercises.iter().find(|e| !e.looks_done()).unwrap_or_else(|| {
    //     println!("🎉 Congratulations! You have done all the exercises!");
    //     println!("🔚 There are no more exercises to do next!");
    //     std::process::exit(0)
    // })
    // } else {
    exercises
        .iter()
        .find(|e| e.name == name)
        .unwrap_or_else(|| {
            println!("No exercise found for '{}'!", name);
            std::process::exit(0)
        })
    // }
}

fn compile_and_run(exercise: &Exercise) -> Result<(), ()> {
    let progress_bar = ProgressBar::new_spinner();
    progress_bar.set_message(format!("Compiling {}...", exercise));
    progress_bar.enable_steady_tick(100);
    let compilation_result = exercise.compile();
    // let compilation = match compilation_result {
    //     Ok(compilation) => compilation,
    //     Err(output) => {
    //         progress_bar.finish_and_clear();
    //         warn!(
    //             "Compilation of {} failed!, Compiler error message:\n",
    //             exercise
    //         );
    //         println!("{}", output.stderr);
    //         return Err(());
    //     }
    // };
    Ok(())
}
fn test() -> Result<(), ()> {
    Ok(())
}

pub fn run(exercise: &Exercise) -> Result<(), ()> {
    match exercise.mode.as_str() {
        "compile" => compile_and_run(exercise).unwrap(),
        "test" => test().unwrap(),
        _ => (),
    };
    Ok(())
}
