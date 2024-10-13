#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <windows.h>

namespace fs = std::filesystem;
using namespace std;

void SetClipboardText(const std::string& text) {
    if (OpenClipboard(nullptr)) {
        EmptyClipboard();

        HGLOBAL hGlob = GlobalAlloc(GMEM_MOVEABLE, text.size() + 1);
        if (hGlob) {
            char* pBuf = static_cast<char*>(GlobalLock(hGlob));
            if (pBuf) {
                strcpy_s(pBuf, text.size() + 1, text.c_str());
                GlobalUnlock(hGlob);

                SetClipboardData(CF_TEXT, hGlob);
            }
        }
        CloseClipboard();
    }
}

string join(vector<string>& items, const string separator) {
    std::ostringstream oss;
    auto it = items.begin();
    if (it != items.end()) {
        oss << *it;
        ++it;
    }
    for (; it != items.end(); ++it) {
        oss << separator << *it;
    }
    return oss.str();
}

int main(int argc, const char* argv[]) {
    if (argc != 3) {
        return 0;
    }

    string mode = argv[2];
    string path = argv[1];
    vector<string> files;

    if (mode == "r") {
        for (const auto& entry : fs::directory_iterator(fs::path(path.c_str()))) {
            files.emplace_back(entry.path().lexically_relative(fs::path(path.c_str())).string());
        }
    }
    else {
        for (const auto& entry : fs::directory_iterator(fs::path(path.c_str()))) {
            files.emplace_back(entry.path().string());
        }
    }

    auto result = join(files, ";\n");
    SetClipboardText(result);
    return 0;
}
