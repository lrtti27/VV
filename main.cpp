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
    vvUtils::vvPrintAll2(*rootEntry , *currentEntry ,currentIndex , 0,0 ,y);
    while ((ch = getch()) != 'q') {
        switch(ch) {
            case KEY_UP:
                currentIndex--;
                //We want to be able to traverse up outside of current dir
                if (currentIndex < 0) {
                    //Update currentEntry to be parent, update currentIndex to be size of parents children
                    currentEntry = currentEntry -> getParent();
                    //We somehow need the previous index, fo now zero
                    currentIndex = 0;
                }
                currentIndex = std::max(0, currentIndex);
                break;
            case KEY_DOWN:
                currentIndex++;
                currentIndex = std::min(static_cast<int>(currentEntry -> getChildren().size()) - 1, currentIndex);
                break;
            case KEY_ENTER:
            case 10:
                //Update the current entry to be the selected one
                currentEntry = currentEntry -> getChildren()[currentIndex];
                //Now, we need to populate the children of this entry
                //Check if we already have children from previous traversal
                if (currentEntry -> getChildren().empty()) {
                    vvUtils::populateChildren(*currentEntry);
                }
                currentIndex = 0;
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
        vvUtils::vvPrintAll2(*rootEntry , *currentEntry , currentIndex ,0, 0 , y);
        refresh();
    }

    endwin();
    return 0;
}
