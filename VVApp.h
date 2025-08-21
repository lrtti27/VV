//
// Created by Leo Ricotti on 8/21/25.
//

#ifndef VV_VVAPP_H
#define VV_VVAPP_H
#include <memory>

#include "FileEntry.h"


class VVApp {
public:
    void init(const std::string& rootPath);
    void run();
    void cleanup();

private:
    std::shared_ptr<FileEntry> m_rootEntry;
    std::shared_ptr<FileEntry> m_currentEntry;
    int m_cursorIndex{};
    int m_ch{};
    int m_yOffset{};
};


#endif //VV_VVAPP_H