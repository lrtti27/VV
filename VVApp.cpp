//
// Created by Leo Ricotti on 8/21/25.
//
#include <ncurses.h>
#include "VVApp.h"

#include "vvUtils.h"

void VVApp::init(const std::string& rootPath) {
    //Init UI
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);

    //Init root entry, set current
    m_rootEntry = vvUtils::createRootEntry(rootPath);
    m_currentEntry = m_rootEntry;

    m_cursorIndex = 0;
    m_yOffset = 0;



}
void VVApp::run() {
    //Print once at start
    vvUtils::vvPrintAll(*m_rootEntry , *m_currentEntry ,m_cursorIndex , false,0 ,m_yOffset);

    //Loop
    while ((m_ch = getch()) != 'q') {
        switch(m_ch) {
            case KEY_UP:
                m_cursorIndex--;
                //We want to be able to traverse up outside of current dir
                if (m_cursorIndex < 0) {
                    m_cursorIndex = vvUtils::findIndexInParentList(*m_currentEntry);
                    //Update currentEntry to be parent, update currentIndex to be size of parents children
                    m_currentEntry = m_currentEntry -> getParent();
                }
                m_cursorIndex = std::max(0, m_cursorIndex);
                break;
            case KEY_DOWN:
                m_cursorIndex++;
                m_cursorIndex = std::min(static_cast<int>(m_currentEntry -> getChildren().size()) - 1, m_cursorIndex);
                break;
            case KEY_ENTER:
            case 10:
                //If the entry is a directory and has no children in the file system, do fucking nothing
                if (vvUtils::isEmptyDirectory(*m_currentEntry -> getChildren()[m_cursorIndex])) {
                    break;
                }

                //Update the current entry to be the selected one
                m_currentEntry = m_currentEntry -> getChildren()[m_cursorIndex];
                //If it is a file, do nothing
                if (!m_currentEntry -> isDir())   {break;}

                //If it doesn't have children, we have not opened it yet
                if (m_currentEntry -> getChildren().empty()) {
                    vvUtils::populateChildren(*m_currentEntry);
                    m_cursorIndex = 0;   //Set cursor to first entry
                }
                //If it does have children, we must close it
                else {
                    m_currentEntry -> clearChildren();

                    m_cursorIndex = vvUtils::findIndexInParentList(*m_currentEntry);
                    //Transfer current entry to be currentEntry's parent IMPORTANT
                    m_currentEntry = m_currentEntry -> getParent();
                }
                break;
            case KEY_BACKSPACE:
            case 127: {
                auto parent = m_currentEntry -> getParent();
                if (parent) {
                    m_currentEntry = parent;
                    m_cursorIndex = 0;
                }
                break;
            }
            default:
                break;
        }
        m_yOffset = 0;

        clear();
        vvUtils::vvPrintAll(*m_rootEntry , *m_currentEntry ,m_cursorIndex , false,0 ,m_yOffset);
        refresh();

    }

}

void VVApp::cleanup() {
    endwin();
}