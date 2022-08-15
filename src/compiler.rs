use std::process::*;
pub enum Compilers {
    Msvc,
    Gpp,
    Clangpp,
}
pub fn subprocess(process: &str, arg: &str) -> std::io::Result<ExitStatus> {
    Command::new(process)
        .arg(arg)
        .stdout(Stdio::null())
        .stderr(Stdio::null())
        .status()
}
pub fn cpp() -> std::io::Result<Compilers> {
    if subprocess("clang++", "-v").is_ok() {
        Ok(Compilers::Clangpp)
    } else if subprocess("g++", "-v").is_ok() {
        Ok(Compilers::Gpp)
    } else {
        crate::warn!("{}", "Neither G++ or Clang++ is installed.");
        Err(std::io::Error::new(
            std::io::ErrorKind::Other,
            "Neither G++ or Clang++ is installed.",
        ))
    }
}