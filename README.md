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
./merger [src path] -o [output path] -m [output filename] -e [excluded filename1]...
```

> -o sets the output path
>
> -m sets the output filename
>
> -e sets filenames(class names) to exclude

examples:

```bash
./merger "C://project//javaTest//src"
```

```bash
./merger "C://project//javaTest//src" -o "C://project//javaTest//merged" -e TestClassA
```

```bash
./merger "C://project//javaTest//src" -m homework -e TestClassA TestClassB
```

Arguments "-o", "-m", "-e" are not necessary.

### Method 2. Setting up in the tool

Following the instructions of the tool:

In default, the output locates at an "output" folder in the parental path of the source folder.

```bash
Enter a folder path to build the task:
> C:\project\javaTest\src
Enter an output path:
>
any files to exclude? (y/n):
> n
do you want to set the merged name? (y/n):
> y
Enter the merged name:
> homework
Path: "C:\\project\\javaTest\\src"
Output path: "C:\\project\\javaTest\\output"
add to merge: "C:\\project\\javaTest\\src\\testClassA" 
add to merge: "C:\\project\\javaTest\\src\\Main" 
saved to: "C:\\project\\javaTest\\output"
```

## License

[MIT](./LICENSE)