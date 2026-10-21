#include <cstdio>
#include <cstring>
#include <string>
// CWideString stand-in: the test keeps the text narrow (g++'s wchar_t is 4 bytes); every operation the function uses
// (Contains, concatenation, equality, ToCharString) behaves as on the 16-bit original
struct CCharString {
    std::string S;
    CCharString() { }
    CCharString(const char* text) : S(text) { }
    long GetLength() const { return (long)S.size(); }
    operator const char*() const { return S.c_str(); }
};
struct CWideString {
    std::string S;
    CWideString() { }
    CWideString(const char* text) : S(text) { }
    bool Contains(const CWideString& part) const { return S.find(part.S) != std::string::npos; }
    CCharString ToCharString() const { return CCharString(S.c_str()); }
    bool operator==(const CWideString& other) const { return S == other.S; }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r; r.S = a.S + b.S; return r; }
static int Fail(const char* addr, const char* what) { std::printf("%s_TEST FAIL %s\n", addr, what); return 1; }
#define CHECK(addr, cond) do { if (!(cond)) return Fail(addr, #cond); } while (0)
// behaviour test: CUserProfileManager::GetPathNameFromFileName puts quick/checkpoint/rolling saves (.qs .cp .rl) in the
// profile directory; any other file goes under the save-game container opened (create = disposition 4, else 3) on U:\,
// and an empty path comes back when the container cannot be opened
static long g_disposition = 0;
static long g_createResult = 0;
static long CreateSaveGame(const CCharString& root, const CWideString& name, long disposition, long flags, char* path, long size)
{
    g_disposition = disposition;
    if (g_createResult != 0) return g_createResult;
    std::sprintf(path, "%s%s\\", root.S.c_str(), name.S.c_str());
    (void)flags; (void)size;
    return 0;
}
struct CUserProfileManager {
    CWideString ProfileName;
    CWideString GetDirectoryNameFromProfileName() const { return CWideString("D:\\Profiles\\") + ProfileName + CWideString("\\"); }
    CWideString GetPathNameFromFileName(const CWideString& fileName, bool create) const;
};
CWideString CUserProfileManager::GetPathNameFromFileName(const CWideString& fileName, bool create) const
{
    char path[260];
    bool isSave = fileName.Contains(".qs") || fileName.Contains(".cp") || fileName.Contains(".rl");
    if (isSave) {
        CWideString directory = GetDirectoryNameFromProfileName();
        return directory + fileName;
    }
    long disposition = 3;
    if (create)
        disposition = 4;
    bool opened = CreateSaveGame("U:\\", ProfileName, disposition, 0, path, 260) == 0;
    if (opened) {
        CWideString full = CWideString(path) + fileName;
        return full;
    }
    return "";
}
int main()
{
    CUserProfileManager manager;
    manager.ProfileName = CWideString("Hero");
    g_disposition = 0;
    CHECK("00408120", manager.GetPathNameFromFileName("Save1.qs", true).S == "D:\\Profiles\\Hero\\Save1.qs" && g_disposition == 0);
    CHECK("00408120", manager.GetPathNameFromFileName("a.cp", false).S == "D:\\Profiles\\Hero\\a.cp");
    CHECK("00408120", manager.GetPathNameFromFileName("b.rl", false).S == "D:\\Profiles\\Hero\\b.rl");
    g_createResult = 0;
    CHECK("00408120", manager.GetPathNameFromFileName("Options.dat", true).S == "U:\\Hero\\Options.dat" && g_disposition == 4);
    CHECK("00408120", manager.GetPathNameFromFileName("Options.dat", false).S == "U:\\Hero\\Options.dat" && g_disposition == 3);
    g_createResult = 5;
    CHECK("00408120", manager.GetPathNameFromFileName("Options.dat", false).S == "");
    std::printf("00408120_TEST PASS\n");
    return 0;
}