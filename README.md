# A simple .java file merging tool

This is a tool for merging all the .java files in one project together.

## Table of Contents

- [background](#Background)
- [compile](#Compile)
- [usage](#Usage)
- [license](#License)

## Background

I made this cuz my Java course TA only accepts one .java file at a time as an assignment, which can really be a disaster when I have a bunch of packages of classes and interfaces in the project.  I have to copy the contents one by one and also pay attention to those annoying imports. In this case, I desperately needed an automated tool.

> ”虽然没有啥难度，但是确实很难咧......“
>
> ”每次交作业都要忙活半天~“

I'm sure there must be something similar online to solve this problem, but I wrote this anyway to practice programming.

The tool writes in C++, for I feel dizzy after enduring a full day of typing java.

## Compile

```bash
g++ -static -std=c++17 -o merger merger.cpp
```

## Usage

### Method 1. Launching with arguments

You can launch the tool with arguments in the command prompt:

```bash
./merger [src path] [output path] [excluded filename]...
```

examples:

```bash
./merger "C://project//javaTest//src"
```

```bash
./merger "C://project//javaTest//src" "C://project//javaTest//merged" TestClassA
```

```bash
./merger "C://project//javaTest//src" d TestClassA TestClassB
```

Excluded filenames can be left out if you don't have any. Besides, it is allowed to skip the output path by typing "d" in replace of it when there are excluded files. You can also skip them two only offering the source path.

### Method 2. Setting up in the tool

Following the instructions of the tool:

In default, the output locates at an "output" folder in the parental path of the source folder.

```bash
Enter a folder path to build the task:
> C://project//javaTest//src
Enter an output path:
>
any files to exclude? (y/n):
>
Path: "C://project//javaTest//src"
Output path: "C://project//javaTest//output"
```

### License

[MIT](./LICENSE)