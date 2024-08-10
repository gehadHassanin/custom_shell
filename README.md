# Custom Shell

## Contents
- [Introduction](#introduction)
- [Usage](#usage)
- [Features](#features)
- [External Commands](#external-commands)
- [Internal Commands](#internal-commands)
- [Examples](#examples)

## Introduction 
This project is a custom shell implementation that supports some internal and external commands. It is designed to provide basic shell functionality with additional features, including input/output/error redirection and piping

## Usage 
### you can use only this command 
```
make all
```

## Features 
### 1) Input/Output/Error Redirection:
- Redirect input using `<`
- Redirect output using `>`
- Redirect error output using `2>`
- Append output using `>>`

### 2) Piping:
Support for piping between two processes using `|`

## External Commands 

**The shell supports executing external commands available in the directories specified by the PATH environment variable. Commands can be executed by simply entering their names, without requiring the full path** 

**`some common examples like:`**
* `mv`: Move or rename files or directories.
* `ls`: List directory contents.
* `grep`: Search for patterns in files.
* `cat`: Concatenate and display file contents.
* `mkdir`: Create a new directory.
* `rm`: Remove files or directories.
* `cp`: Copy files or directories.
* `echo`: Display a line of text or a string.
* `ps`: Report a snapshot of current processes.

## Internal Commands 

| command name | Description                                           | Args                    |  Example                  | 
| ------------ | -----------                                           | ----                    | ----------                | 
| help         | show list of supported commands                       |  no                     | help                      | 
| exit         | terminate shell                                       |  no                     | exit                      |
| mypwd          | print working directory                               |  no                     | mypwd                       | 
| cd          | change the current directory to the specified path     |  specified_path           | cd ../                      | 
| mycat          | show content of certain file                          |  filename               | mycat file                  |
| myecho         | print a user input string on stdout                   |  message                | myecho "gehad" <BR>`or`<BR> myecho gehad                  | 
| myrealpath          | print the resolved absolute path                            |  filePath <BR>`or`<BR> dirPath               | myrealpath filePath <BR>`or`<BR> myrealpath dirPath                   |
| mycp           | copy a file to another file                           |  sourcePath, targetPath | mycp  sourcePath targetPath |
| mycp -a        | append the source content to the end of the target file |  sourcePath, targetPath | mycp -a sourcePath targetPath |
| mymv           | move a file to another place           |  sourcePath, targetPath | mymv sourcePath targetPath |
| mymv -f           | force overwriting the target file if exits            |  sourcePath, targetPath | mymv -f sourcePath targetPath |
| myrm           | remove files                         |  filesname        | myrm file1 file2                   |
| myrmdir        | remove directories                   |  filesname        | myrmdir dir1 dir2                  |
| envir          | print all the environment variables  |  no               | envir                 |
| envir variable | print the value of this environment variable if exists   |  variable             | envir  PATH  <BR>`or`<BR> envir anyVariable|    
| type | return the type of the command (internal, external or unsupported command)   |  command            | type ls <BR>`or`<BR> type mypwd|  
| phist | lists the last 10 processes with their exit status   |  no           | phist | 
| myclear | clear the terminal screen   |  no           | myclear | 
| myuptime |  print the uptime for the system and the time spent in the idle process   |  no           | myuptime | 
| myfree | print RAM info ( total size, used and free) and Swap area info (total size, used, free)   |  no           | myfree | 

## Examples