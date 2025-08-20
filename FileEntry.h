//
// Created by Leo Ricotti on 8/20/25.
//

#ifndef VV_FILEENTRY_H
#define VV_FILEENTRY_H
#include <string>
#include <utility>
#include <vector>
#include <memory>
#include <__filesystem/filesystem_error.h>

namespace fs = std::filesystem;

// FileEntry.h
class FileEntry : public std::enable_shared_from_this<FileEntry>{
public:
    FileEntry(fs::path path, std::string name, bool isDir);
    const std::string& getName() const;
    bool isDir() const;
    fs::path getPath() const;
    const std::vector<std::shared_ptr<FileEntry>>& getChildren() const;
    std::shared_ptr<FileEntry> getParent() const;
    void setParent(std::shared_ptr<FileEntry> parent);
    void addChild(const std::shared_ptr<FileEntry>& child);
    std::string toString() const;

    bool operator==(const FileEntry& other) const;
private:
    fs::path m_path;
    std::string m_name;
    bool m_isDir;
    std::vector<std::shared_ptr<FileEntry>> m_children;
    std::weak_ptr<FileEntry> m_parent; // weak_ptr prevents circular reference
};



#endif //VV_FILEENTRY_H
