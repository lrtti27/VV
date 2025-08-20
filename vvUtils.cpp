//
// Created by Leo Ricotti on 8/20/25.
//


#include <filesystem>
#include "vvUtils.h"

#include <iostream>
#include <ncurses.h>
#include "FileEntry.h"

namespace fs = std::filesystem;

namespace vvUtils {
    void vvPrint(const FileEntry& entry , int currentIndex) {
        clear();
        attron(A_BOLD);
        //First, print the name of the current file, if none it is root
        if (entry.getName() == "") {
            std::string absPath = fs::absolute(entry.getPath());
            //std::cout << "|ROOT| : " + absPath << std::endl;

            mvprintw(0, 0, "%s", absPath.c_str());
        }
        else {
            //std::cout << "|" + entry.getName() + "|" << std::endl;

            mvprintw(0, 0, "%s", entry.getName().c_str());
        }
        attroff(A_BOLD);

        //Now, print the children
        for (int i = 0; i < entry.getChildren().size(); ++i) {
            if (i == currentIndex) {
                //Highlight is
                attron(COLOR_PAIR(1));
            }
            mvprintw(i + 1 , 0 , entry.getChildren()[i] -> getName().c_str());
            if (i == currentIndex) {
                attroff(COLOR_PAIR(1));
            }

        }
        refresh();
    }

    void populateChildren(FileEntry& entry) {
        if (entry.isDir() && entry.getChildren().empty()) {
            for (const auto& file : fs::directory_iterator(entry.getPath())) {
                auto child = createFileEntry(file.path());
                entry.addChild(child);
            }
        }
    }


    std::shared_ptr<FileEntry> createFileEntry(const fs::path &path) {
        //Using given path, extract name , whether it is dir
        std::string filename = path.filename().string();
        bool isDir = fs::is_directory(path);
        auto entry = std::make_shared<FileEntry>(path , filename , isDir);
        return entry;
    }

    std::shared_ptr<FileEntry> createRootEntry(const fs::path &path) {
        auto root = createFileEntry(path);
        //Populate children with the files and directories that are immediate
        for (const auto& entry : fs::directory_iterator(path)) {
            auto child = createFileEntry(entry.path());
            root -> addChild(child);
        }

        return root;
    }




}