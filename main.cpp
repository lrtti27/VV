#include <ncurses.h>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <iostream>

#include "FileEntry.h"
#include "VVApp.h"
#include "vvUtils.h"
#include "VVApp.h"
namespace fs = std::filesystem;

int main() {

    VVApp app;
    app.init("../");
    app.run();
    app.cleanup();

    return 0;
}
