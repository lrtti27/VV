//
// Created by Leo Ricotti on 8/20/25.
//

#ifndef VV_VVUTILS_H
#define VV_VVUTILS_H

#include <string>
#include <vector>

#include "FileEntry.h"

namespace vvUtils{
    void vvPrint(const FileEntry& entry , int currentIndex);
    void vvPrintAll(const FileEntry& root , const FileEntry& current , int currentIndex , bool isCurrent , int xOffset , int& yOffset );
    void vvPrintAll2(const FileEntry& root , const FileEntry& current ,int currentIndex , bool indexMatch ,int xOffset , int& yOffset);
    void populateChildren(FileEntry& entry);
    std::shared_ptr<FileEntry> createFileEntry(const fs::path& path);
    std::shared_ptr<FileEntry> createRootEntry(const fs::path &path);
}


#endif //VV_VVUTILS_H