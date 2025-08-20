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
    void vvPrintAll2(const FileEntry& root , const FileEntry& current ,int currentIndex , bool indexMatch ,int xOffset , int& yOffset) {
        //Enable attributes
        if (root.isDir()) {
            attron(A_BOLD);
        }
        if (indexMatch) {
            attron(COLOR_PAIR(1));
        }

        //Print the current entry
        std::string entryStr = root.getName();
        mvprintw(yOffset, xOffset, "%s", entryStr.c_str());

        //Disable attributes
        if (indexMatch) {
            attroff(COLOR_PAIR(1));
        }

        if (root.isDir()) {
            attroff(A_BOLD);
        }

        //Update yOffset
        yOffset++;


        //Now recurse over each of the children , each of which should be printed with new xOffset corresp to parent
        auto children = root.getChildren();
        if (children.size() > 0) {
            int idx = 0;
            for (const auto& child : root.getChildren()) {
                if (idx == currentIndex && root == current) {
                    vvPrintAll2(*child , current , currentIndex , true,xOffset + 3 , yOffset);
                }
                else {
                    vvPrintAll2(*child , current , currentIndex , false,xOffset + 3 , yOffset);
                }
                idx++;
            }
        }


    }
    void vvPrintAll(const FileEntry& root , const FileEntry& current , int currentIndex , bool isCurrent , int xOffset , int& yOffset) {

        //Print root in bold
        attron(A_BOLD);
        std::string absPath = root.getName();
        mvprintw(yOffset, xOffset, "%s", absPath.c_str());
        attroff(A_BOLD);

        //Update y offset after print
        yOffset++;

        //Recursively go through children and do the same
        for (int i = 0; i < root.getChildren().size(); ++i) {
            bool isLast = (i == root.getChildren().size() - 1);
            if (i == currentIndex && current == root) {
                attron(COLOR_PAIR(1));
            }
            else {
                attroff(COLOR_PAIR(1));
            }
            //Check if this child is a directory, if so, recurse and print
            if (root.getChildren()[i] -> isDir()) {
                //Call printall with this as root and a +1 x offset
                vvPrintAll(*root.getChildren()[i] , current , currentIndex, false , xOffset + 3 , yOffset);
            }
            else {
                //Check if the root is equal to current

                mvprintw(yOffset, xOffset + 3 , root.getChildren()[i] -> getName().c_str());

                yOffset++;
            }

            if (i == currentIndex && current == root) {
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