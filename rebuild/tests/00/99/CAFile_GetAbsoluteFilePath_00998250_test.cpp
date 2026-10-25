// Behaviour test for 0x00998250 CAFile::GetAbsoluteFilePath(const CCharString&, const CCharString&): an empty relative
// path returns the base unchanged; otherwise the base loses its last component, each relative folder is appended or
// (for "..") pops one more component, and the relative file name is appended last.
#include <cstdio>
#include <string>

static void DropLastComponent(std::string& path)
{
    for (long i = (long)path.size() - 1; i >= 0; --i) {
        if (path[i] == '\\') {
            path.erase(i);
            break;
        }
    }
}

static std::string GetAbsoluteFilePath(const std::string& basePath, const std::string& relativePath)
{
    if (relativePath == "")
        return basePath;
    std::string path(basePath);
    DropLastComponent(path);
    std::string::size_type pos = 0;
    for (;;) {
        std::string::size_type at = relativePath.find('\\', pos);
        if (at == std::string::npos)
            break;
        std::string folder = relativePath.substr(pos, at - pos);
        if (folder == "..")
            DropLastComponent(path);
        else
            path += "\\" + folder;
        pos = at + 1;
    }
    std::string fileName(relativePath);
    for (long i = (long)relativePath.size() - 1; i >= 0; --i) {
        if (relativePath[i] == '\\') {
            fileName = relativePath.substr(i + 1);
            break;
        }
    }
    path += "\\" + fileName;
    return path;
}

int main()
{
    int ok = 1;
    ok &= GetAbsoluteFilePath("data\\defs\\main.def", "") == "data\\defs\\main.def";
    ok &= GetAbsoluteFilePath("data\\defs\\main.def", "other.def") == "data\\defs\\other.def";
    ok &= GetAbsoluteFilePath("data\\defs\\main.def", "sub\\x.def") == "data\\defs\\sub\\x.def";
    ok &= GetAbsoluteFilePath("data\\defs\\main.def", "..\\x.def") == "data\\x.def";
    ok &= GetAbsoluteFilePath("data\\defs\\", "..\\..\\levels\\y.lev") == "data\\levels\\y.lev";   // a ".." past the root is a no-op
    ok &= GetAbsoluteFilePath("data\\defs\\", "..\\z.txt") == "data\\z.txt";
    ok &= GetAbsoluteFilePath("main.def", "a\\b.txt") == "main.def\\a\\b.txt";   // no backslash: nothing dropped
    std::printf("00998250_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}