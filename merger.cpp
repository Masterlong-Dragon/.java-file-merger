#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <set>
#include <unordered_map>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;

fs::path oPath;                                // output path
fs::path path;                                 // input path
string pathName;                               // input path name
string mergedContent;                          // merged content
unordered_map<string, bool> excludedFiles;     // excluded files
unordered_map<string, bool> importsFromLocals; // imports from locals
set<string> importsPaths;                      // imports paths

bool parsePath(int argc, char *argv[])
{
    if (argc > 1)
    {
        // set path
        path = argv[1];
        if (!fs::exists(path))
        {
            cout << "Path does not exist." << endl;
            return false;
        }
        if (!fs::is_directory(path))
        {
            cout << "Path is not a directory." << endl;
            return false;
        }
        if (argc > 2)
        {
            if (strlen(argv[2]) == 1 && argv[2][0] == 'd')
            {
                oPath = path.parent_path() / "output";
                if (!fs::exists(oPath))
                    fs::create_directory(oPath);
            }
            else
                oPath = argv[2];
            if ((!fs::exists(oPath) || !fs::is_directory(oPath)))
            {
                cout << "Output path does not exist or is not a directory." << endl;
                return false;
            }
            // exclude files
            for (int i = 3; i < argc; i++)
            {
                excludedFiles[argv[i]] = true;
            }
        }
        else
        {
            oPath = path.parent_path() / "output";
            if (!fs::exists(oPath))
                fs::create_directory(oPath);
        }
        return true;
    }
    return false;
}

// set some arguments
void prompt()
{
    bool flag = true;
    while (flag)
    {
        cout << "Enter a folder path to build the task: " << endl
             << "> ";
        string input;
        getline(cin, input);
        path = input;
        if (flag = !fs::exists(path))
            cout << "Path does not exist." << endl;
        else if (flag = !fs::is_directory(path))
            cout << "Path is not a directory." << endl;
    }
    flag = true;
    while (flag)
    {
        cout << "Enter an output path: " << endl
             << "> ";
        string input;
        getline(cin, input);
        oPath = input;
        if (input.empty())
        {
            oPath = path.parent_path() / "output";
            if (!fs::exists(oPath))
                fs::create_directory(oPath);
            break;
        }
        if (flag = !fs::exists(oPath))
            cout << "Output path does not exist." << endl;
    }
    cout << "any files to exclude? (y/n): " << endl
         << "> ";
    string input;
    getline(cin, input);
    if (input == "y" || input == "Y")
    {
        flag = true;
        while (flag)
        {
            cout << "Enter a file path to exclude (press space to over): " << endl
                 << "> ";
            string input;
            getline(cin, input);
            if (input.empty())
            {
                flag = false;
                break;
            }
            excludedFiles[input] = true;
        }
    }
}

void mergeFile(fs::path file)
{
    if (fs::is_regular_file(file))
    {
        ifstream in(file);
        string line;
        // found the beginning of available parts
        int iPair = 0;
        size_t bImport = 0;
        while (getline(in, line))
            if (line.find("class") != string::npos || line.find("interface") != string::npos)
                break;
            else if ((bImport = line.find("import")) != string::npos)
            {
                string importContent;
                // copy to the end of the line
                for (auto c : line)
                    if (c == ';')
                        break;
                    else
                        importContent += c;
                // check if it is a local import
                importsPaths.insert(importContent);
            }
        do
        {
            // for each line, if it is the largest class or interface, remove the "public" or "private" in the beginning
            // iPair checks if the line is the largest class or interface
            if (iPair == 0 && line.find("class") != string::npos || line.find("interface") != string::npos)
            {
                size_t b = line.find("public");
                if (b != string::npos)
                    line.replace(b, 6, "");
                else
                {
                    b = line.find("private");
                    if (b != string::npos)
                        line.replace(line.find("private"), 7, "");
                }
            }
            // add a new line, check '{' and '}'
            for (auto c : line)
            {
                switch (c)
                {
                case '{':
                    iPair++;
                    break;
                case '}':
                    iPair--;
                    break;
                }
                mergedContent += c;
            }
            mergedContent += "\n";
        } while (getline(in, line));
        in.close();
    }
    else
    {
        cout << "File is not a regular file, cannot process." << endl;
    }
}

// merge all files in the path
void directory(fs::path path)
{
    for (auto &p : fs::directory_iterator(path))
    {
        if (fs::is_directory(p))
        {
            directory(p);
        }
        else
        {
            string fileName = p.path().filename().string();
            if (fileName.substr(fileName.size() - 5, 5) != ".java")
                return;
            fileName = fileName.substr(0, fileName.size() - 5);
            // add it to the local imports
            string importString = p.path().parent_path().string();
            importString = "import " + importString.substr(pathName.size() + 1, importString.size() - pathName.size() - 1);
            // change it as the java import format
            for (auto &c : importString)
                if (c == '\\' || c == '/')
                    c = '.';
            // mark the import
            importsFromLocals[importString + ".*"] = true;
            importsFromLocals[importString + '.' + fileName] = true;
            // check if the file is excluded
            if (excludedFiles.find(fileName) == excludedFiles.end())
            {
                cout << "add to merge: " << p << endl;
                mergeFile(p);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc > 0 && !parsePath(argc, argv))
        prompt();
    cout << "Path: " << path << endl
         << "Output path: " << oPath << endl;
    pathName = path.string();
    directory(path);
    // add imports to the merged content
    for (auto str : importsPaths)
        if (importsFromLocals.find(str) == importsFromLocals.end())
            mergedContent = str + ";\n" + mergedContent;
    mergedContent = "package merged;\n" + mergedContent;
    // save the merged content to the output file
    cout << "saved to: " << oPath << endl;
    ofstream out(oPath / "merged.java", ios::ate);
    out << mergedContent;
    out.close();
    system("pause");
    return 0;
}