// Behaviour test for 0x00998C90 CAFile::ExpandPath: drive-rooted and UNC paths are returned as-is, a root-relative
// path gets the base's drive letter, ".\x" drops the dot, plain relative paths are appended to the base (which gets a
// trailing backslash), and ".." paths go to GetAbsoluteFilePath (modelled here as a simple parent walk).
#include <cstdio>
#include <string>

typedef std::string Str;

static Str GetAbsoluteFilePath(const Str& directory, const Str& path)
{
    Str dir = directory, rest = path;
    while (rest.compare(0, 3, "..\\") == 0) {
        Str::size_type cut = dir.find_last_of('\\', dir.size() - 2);
        dir = cut == Str::npos ? Str() : dir.substr(0, cut + 1);
        rest = rest.substr(3);
    }
    return dir + rest;
}

static Str ExpandPath(const Str& path, const Str& base)
{
    Str result;
    const char* text = path.c_str();
    Str directory(base);
    if (directory.size() > 0) {
        long last = (long)directory.size() - 1;
        if (directory[last] != '\\')
            directory += "\\";
    }
    if ((path.size() >= 3 && text[1] == ':' && text[2] == '\\') ||
        (path.size() >= 2 && text[0] == '\\' && text[1] == '\\'))
        result = path;
    else if (path.size() > 0 && text[0] == '\\') {
        Str drive;
        drive += base[0];
        result = drive + ":" + path;
    } else if (path.size() > 2 && text[0] == '.' && text[1] == '.')
        result = GetAbsoluteFilePath(directory, path);
    else if (path.size() > 1 && text[0] == '.')
        result = directory + path.substr(2);
    else
        result = directory + path;
    return result;
}

int main()
{
    int ok = 1;
    ok &= ExpandPath("D:\\x\\y.txt", "C:\\game") == "D:\\x\\y.txt";
    ok &= ExpandPath("\\\\server\\share", "C:\\game") == "\\\\server\\share";
    ok &= ExpandPath("\\data\\a.txt", "E:\\game") == "E:\\data\\a.txt";
    ok &= ExpandPath(".\\save\\a.sav", "C:\\game") == "C:\\game\\save\\a.sav";
    ok &= ExpandPath("save\\a.sav", "C:\\game\\") == "C:\\game\\save\\a.sav";
    ok &= ExpandPath("..\\other\\b", "C:\\game\\data") == "C:\\game\\other\\b";
    ok &= ExpandPath("x", "") == "x";
    std::printf("00998c90_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}