#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>

#include "FileEntry.h"
#include "vvUtils.h"

namespace fs = std::filesystem;

int main() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);

    std::shared_ptr<FileEntry> rootEntry = vvUtils::createRootEntry("../");
    std::shared_ptr<FileEntry> currentEntry = rootEntry;

    int currentIndex = 0;
    int ch;

    //vvUtils::vvPrint(*currentEntry, currentIndex);
    int y = 0;
    vvUtils::vvPrintAll(*rootEntry , *currentEntry ,currentIndex , 0,0 ,y);
    while ((ch = getch()) != 'q') {
        switch(ch) {
            case KEY_UP:
                currentIndex--;
                //We want to be able to traverse up outside of current dir
                if (currentIndex < 0) {
                    currentIndex = vvUtils::findIndexInParentList(*currentEntry);
                    //Update currentEntry to be parent, update currentIndex to be size of parents children
                    currentEntry = currentEntry -> getParent();
                }
                currentIndex = std::max(0, currentIndex);
                break;
            case KEY_DOWN:
                currentIndex++;
                currentIndex = std::min(static_cast<int>(currentEntry -> getChildren().size()) - 1, currentIndex);
                break;
            case KEY_ENTER:
            case 10:
                //If the entry is a directory and has no children in the file system, do fucking nothing
                if (vvUtils::isEmptyDirectory(*currentEntry -> getChildren()[currentIndex])) {
                    break;
                }

                //Update the current entry to be the selected one
                currentEntry = currentEntry -> getChildren()[currentIndex];
                //If it is a file, do nothing
                if (!currentEntry -> isDir())   break;

                //If it doesn't have children, we have not opened it yet
                if (currentEntry -> getChildren().empty()) {
                    vvUtils::populateChildren(*currentEntry);
                    currentIndex = 0;   //Set cursor to first entry
                }
                //If it does have children, we must close it
                else {
                    currentEntry -> clearChildren();

                    currentIndex = vvUtils::findIndexInParentList(*currentEntry);
                    //Transfer current entry to be currentEntry's parent IMPORTANT
                    currentEntry = currentEntry -> getParent();
                }
                break;
            case KEY_BACKSPACE:
            case 127: {
                auto parent = currentEntry -> getParent();
                if (parent) {
                    currentEntry = parent;
                    currentIndex = 0;
                }
                break;
            }
            default:
                break;
        }

        //vvUtils::vvPrint(*currentEntry, currentIndex);
        clear();
        y = 0;
        vvUtils::vvPrintAll(*rootEntry , *currentEntry , currentIndex ,0, 0 , y);
        refresh();
    }

    endwin();
    return 0;
}
