#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <cerrno>

#ifdef _WIN32
#include <direct.h>
#define mkdir _mkdir
#endif

using namespace std;

void list_directory(const string& path) {
    DIR* dir;
    struct dirent* entry;

    if ((dir = opendir(path.c_str())) != NULL) {
        cout << "Directory contents of " << path << ":\n";
        while ((entry = readdir(dir)) != NULL) {
            cout << entry->d_name << "\n";
        }
        closedir(dir);
    } else {
        perror("opendir");
    }
}

void view_file(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << '\n';
        }
        file.close();
    } else {
        cerr << "Unable to open file " << filename << "\n";
    }
}

void create_directory(const string& dirname) {
    if (mkdir(dirname.c_str()) == -1) {
        cerr << "Error creating directory " << dirname << ": " << strerror(errno) << "\n";
    } else {
        cout << "Directory " << dirname << " created successfully.\n";
    }
}

void copy_file(const string& src, const string& dest) {
    ifstream source(src, ios::binary);
    ofstream destination(dest, ios::binary);

    if (!source.is_open() || !destination.is_open()) {
        cerr << "Error opening files.\n";
        return;
    }

    destination << source.rdbuf();

    source.close();
    destination.close();

    cout << "File copied from " << src << " to " << dest << "\n";
}

void move_file(const string& src, const string& dest) {
    if (rename(src.c_str(), dest.c_str()) != 0) {
        cerr << "Error moving file from " << src << " to " << dest << ": " << strerror(errno) << "\n";
    } else {
        cout << "File moved from " << src << " to " << dest << "\n";
    }
}

int main() {
    string command, path;

    while (true) {
        cout << "Enter command (ls/view/cd/mkdir/cp/mv/exit): ";
        cin >> command;

        if (command == "ls") {
            cout << "Enter directory path: ";
            cin >> path;
            list_directory(path);
        } else if (command == "view") {
            cout << "Enter file name: ";
            cin >> path;
            view_file(path);
        } else if (command == "cd") {
            cout << "Enter directory path: ";
            cin >> path;
            if (chdir(path.c_str()) != 0) {
                cerr << "Error changing directory: " << strerror(errno) << "\n";
            }
        } else if (command == "mkdir") {
            cout << "Enter directory name: ";
            cin >> path;
            create_directory(path);
        } else if (command == "cp") {
            string dest;
            cout << "Enter source file: ";
            cin >> path;
            cout << "Enter destination file: ";
            cin >> dest;
            copy_file(path, dest);
        } else if (command == "mv") {
            string dest;
            cout << "Enter source file: ";
            cin >> path;
            cout << "Enter destination file: ";
            cin >> dest;
            move_file(path, dest);
        } else if (command == "exit") {
            break;
        } else {
            cerr << "Unknown command.\n";
        }
    }

    return 0;
}
