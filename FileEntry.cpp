//
// Created by Leo Ricotti on 8/20/25.
//

#include "FileEntry.h"


FileEntry::FileEntry(fs::path path, std::string name, bool isDir)
    : m_path(std::move(path)), m_name(std::move(name)), m_isDir(isDir) {}

const std::string& FileEntry::getName() const {
    return m_name;
}
bool FileEntry::isDir() const {
    return m_isDir;
}
fs::path FileEntry::getPath() const {
    return m_path;
}
const std::vector<std::shared_ptr<FileEntry>>& FileEntry::getChildren() const {
    return m_children;
};
std::shared_ptr<FileEntry> FileEntry::getParent() const {
    return m_parent.lock();
}

void FileEntry::setParent(std::shared_ptr<FileEntry> parent) {
    m_parent = parent;
}

void FileEntry::addChild(const std::shared_ptr<FileEntry>& child) {
    child -> setParent(shared_from_this());
    m_children.push_back(child);
}

void FileEntry::clearChildren() {
    m_children.clear();
}

int FileEntry::getRelativeIndex() const {
    return m_relativeIndex;
}

void FileEntry::setRelativeIndex(int index) {
    m_relativeIndex = index;
}

std::string FileEntry::toString() const {
    std::string res;
    res += "FileEntry: " + m_name + "\n";
    res += "Path: " + m_path.string() + "\n";
    res += "IsDir: " + std::to_string(m_isDir) + "\n";
    res += "Children: " + std::to_string(m_children.size()) + "\n";
    return res;
}

bool FileEntry::operator==(const FileEntry& other) const {
    return m_path == other.m_path;
}