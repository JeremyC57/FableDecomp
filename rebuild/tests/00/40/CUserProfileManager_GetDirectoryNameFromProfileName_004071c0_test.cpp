// Behaviour test for 0x004071C0 (CUserProfileManager::GetDirectoryNameFromProfileName): failure gives the root marker,
// a directory under "./<save dir>" loses its 7-character prefix, any other directory is returned as the full path.
// Host model: narrow strings, '/' for the engine's backslash.
#include <cstdio>
#include <cstring>
class CWideString { public:
    char s[128];
    CWideString() { s[0] = 0; }
    CWideString(const char* t) { std::strcpy(s, t); }
    long GetLength() const { return (long)std::strlen(s); }
    const char* GetAsCharArray() const { return s; }
    CWideString GetSubString(unsigned long start, unsigned long end) const
    {
        CWideString r; unsigned long n = 0;
        for (unsigned long i = start; i < end && s[i]; ++i) r.s[n++] = s[i];
        r.s[n] = 0; return r;
    }
};
static CWideString operator+(const char* a, const CWideString& b) { CWideString r(a); std::strcat(r.s, b.s); return r; }
static long g_result; static const char* g_path; static char g_name[64]; static long g_disp, g_flags, g_size;
static long CreateSaveGame(const char* root, const CWideString& name, long disp, long flags, char* buffer, long size)
{
    if (std::strcmp(root, "U:/") != 0) return 99;
    std::strcpy(g_name, name.s); g_disp = disp; g_flags = flags; g_size = size;
    std::strcpy(buffer, g_path); return g_result;
}
static CWideString GetSaveDirWithoutTrailingBlackslash() { return CWideString("Saves"); }
static void SplitPath(const CWideString& path, CWideString*, CWideString* dir, CWideString*, CWideString*)
{
    const char* slash = std::strrchr(path.s, '/');
    unsigned long n = slash ? (unsigned long)(slash - path.s + 1) : 0;
    std::memcpy(dir->s, path.s, n); dir->s[n] = 0;
}
struct Manager {
    CWideString ProfileName;
    CWideString GetDirectoryNameFromProfileName() const
    {
        char path[260];
        bool opened = CreateSaveGame("U:/", ProfileName, 4, 0, path, 260) == 0;
        if (opened) {
            CWideString directory;
            SplitPath(CWideString(path), 0, &directory, 0, 0);
            CWideString prefix = "./" + GetSaveDirWithoutTrailingBlackslash();
            if (std::strncmp(directory.GetAsCharArray(), prefix.GetAsCharArray(), prefix.GetLength()) == 0) {
                CWideString name = directory.GetSubString(7, directory.GetLength());
                return name;
            }
            return path;
        }
        return "/";
    }
};
int main()
{
    int bad = 0;
    Manager m; std::strcpy(m.ProfileName.s, "Hero");
    g_result = 0; g_path = "./Saves/Hero01/save.dat";
    if (std::strcmp(m.GetDirectoryNameFromProfileName().s, "/Hero01/") != 0) bad = 1;
    if (std::strcmp(g_name, "Hero") != 0 || g_disp != 4 || g_flags != 0 || g_size != 260) bad = 1;
    g_path = "D:/Other/Hero01/save.dat";
    if (std::strcmp(m.GetDirectoryNameFromProfileName().s, "D:/Other/Hero01/save.dat") != 0) bad = 1;
    g_result = 5;
    if (std::strcmp(m.GetDirectoryNameFromProfileName().s, "/") != 0) bad = 1;
    if (bad) { std::printf("004071c0_TEST FAIL\n"); return 1; }
    std::printf("004071c0_TEST PASS\n");
    return 0;
}