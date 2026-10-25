// Behaviour test for 0x00999AB0 CAFile::IsRoomForFile: the directory part of the path is created when missing, a failed
// creation or a failed free-space query is "no room", and the free byte count is compared unsigned 64-bit against the
// sign-extended size (a negative size asks for 2^64 - n bytes).
#include <cstdio>
#include <string>
#include <set>

static std::set<std::string> g_dirs;
static bool g_canCreate = true, g_queryOk = true;
static unsigned long long g_free = 0;

static bool PathExists(const std::string& p) { return g_dirs.count(p) != 0; }
static void CreateDirectoryW(const std::string& p) { if (g_canCreate) g_dirs.insert(p); }
static std::string DirectoryOf(const std::string& path)
{
    std::string::size_type slash = path.find_last_of("\\/");
    return slash == std::string::npos ? std::string() : path.substr(0, slash + 1);
}

static bool IsRoomForFile(long size, const std::string& pathname)
{
    std::string root = DirectoryOf(pathname);
    if (!PathExists(root)) {
        CreateDirectoryW(root);
        if (!PathExists(root))
            return false;
    }
    if (!g_queryOk)
        return false;
    unsigned long long freeBytes = g_free;
    if (freeBytes < (unsigned long long)(long long)size)
        return false;
    return true;
}

int main()
{
    int ok = 1;
    g_free = 5000;
    ok &= IsRoomForFile(4000, "C:\\save\\a.sav");
    ok &= PathExists("C:\\save\\");                       // created on the way
    ok &= IsRoomForFile(5000, "C:\\save\\a.sav");          // exactly enough
    ok &= !IsRoomForFile(5001, "C:\\save\\a.sav");
    ok &= !IsRoomForFile(-1, "C:\\save\\a.sav");           // sign-extended: 2^64 - 1 bytes
    g_free = 0x100000000ULL;                               // > 4 GB: the high word decides
    ok &= IsRoomForFile(0x7FFFFFFF, "C:\\save\\a.sav");
    g_canCreate = false;
    ok &= !IsRoomForFile(1, "D:\\new\\b.sav");             // directory cannot be created
    g_canCreate = true; g_queryOk = false;
    ok &= !IsRoomForFile(1, "C:\\save\\a.sav");            // free-space query failed
    std::printf("00999ab0_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}