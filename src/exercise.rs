use std::{collections::HashMap, io::Read};
use serde::{Deserialize, Serialize};

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

#[derive(Deserialize)]
pub struct ExerciseList {
    pub exercise: Vec<Exercise>
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
        exercises.iter().find(|e| e.name == name).unwrap_or_else(|| {
            println!("No exercise found for '{}'!", name);
            std::process::exit(0)
        })
    // }
}

pub fn run(exercise: &Exercise) -> Result<(), ()> {
    Ok(())
}