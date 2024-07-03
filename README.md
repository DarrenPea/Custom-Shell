[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/u16ttUuk)
# CSEShell

CSEShell is a simple, custom shell for Unix-based systems, designed to provide an interface for executing system programs. This project includes a basic shell implementation, a set of system programs (`find`, `ld`, `ldr`), and some test files.

## Directory Structure

The project is organized as follows:

- `bin/` - Contains compiled executables for system programs.
  - `find` - Program to find files.
  - `ld` - Program for listing the contents of the current directory.
  - `ldr` - Program for listing the contents of the current directory recursively.
- `cseshell` - The main executable for the CSEShell.
- `files/` - Contains various test files used with the shell and system programs.
  - `combined.txt`, `file1.txt`, `file2.txt`, ... - Test text files.
  - `notes.pdf` - A PDF file for testing.
  - `ss.png` - An image file.
- `makefile` - Makefile for building the CSEShell and system programs.
- `source/` - Source code for the shell and system programs.
  - `shell.c` and `shell.h` - Source and header files for the shell.
  - `system_programs/` - Source code and header for the system programs.

## Building the Project

To build the CSEShell and system programs, run the following command in the root directory:

```bash
make
```

This will compile the source code and place the executable files in the appropriate directories.

## Running CSEShell

After building, you can start the shell by running:

```bash
./cseshell
```

From there, you can execute built-in commands and any of the included system programs (e.g., `find`, `ld`, `ldr`).

## System Programs

- `find.c` - Searches for files in a directory.
- `ld.c` - List the contents of the curent directory.
- `ldr.c` - List the contents of the current directory recursively.

Each program can be executed from the CSEShell once it is running. This starter code only allows the shell to execute a command once before exiting because `execv` replace the entire process' address space. Students need to fix this and allow the shell to prompt for more commands in Programming Assignment 1.

## Files Directory

The `files/` directory contains various text, PDF, and image files for testing the functionality of the CSEShell and its system programs.

## Makefile

The Makefile contains rules for compiling the shell and system programs. You can clean the build by running:

```bash
make clean
```

## Source Directory

Contains all the necessary source code for the shell and system programs. It is divided into the shell implementation (`shell.c`, `shell.h`) and system programs (`system_programs/`).


## Builtin Functions Supported
- `cd` - Changes the current directory of the shell to the specified path. If no path is given, it defaults to the project directory.
- `help` - List all builtin commands in the shell.
- `exit` - Exits the shell
- `usage` - Provides a brief usage guide for the shell and its built-in commands
- `env` - Lists all the environment variables currently set in the shell
- `setenv` - Sets or modifies an environment variable for this shell session
- `unsetenv` - Removes an environment variable from the shell
- `tts` - Toggle text-to-speech mode


## Additional Features Supported

### Idle Resource Management
We have implemented a feature where the shell reduces its resource usage while it is idle. Every 30 seconds the user does not submit any input, the shell would remind the user that it is in the idle state for visualisation purposes. Even if the user types a command halfway and the reminder appears, the user will be able to continue where he/she left off, without typing the entire command again. The shell would only wake up after a new command is entered by the user.

##TTS flag in RC/ TTS mode
You can add a "tts" line in your .cseshellrc file to enable TTS on terminal startup. This saves time and allows the user to customize whether or not they prefer Text-to-speech being enabled or not. The shell notifies you ehrn text-to-speech is enabled. To enable or disable TTS manually, the user can use the builtin function `tts`


## Sustainability and Inclusivity

### Sustainability
The implementation of Idle Resource Management allows the shell to reduce resource usage when the shell is idle. This would save the power of the shell and the system, which would in turn extend the battery life of the computer, save electricity and decrease overall power consumption. As such, it will contribute to sustainability.
### Inclusivity
The implementation of a Text-to-speech mode allows for visually impaired users to interact with the terminal, which would open computing to a much more diverse demographic of visually impaired persons. Therefore, it contributes to the inclusivity effort.
