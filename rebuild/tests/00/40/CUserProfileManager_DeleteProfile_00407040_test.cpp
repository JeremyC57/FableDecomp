// Behaviour test for 0x00407040 (CUserProfileManager::DeleteProfile): over a scripted directory listing, only entries
// that exist and are files are deleted, then the profile directory "<save dir><valid name>\" itself; the find pattern is
// the directory plus "*" (the host model uses '/' for the engine's backslash).
#include <cstdio>
#include <cstring>
struct CWideString {                                   // narrow host stand-in for the engine's wide string
    char s[128];
    CWideString(const char* t) { std::strcpy(s, t); }
};
static CWideString operator+(const CWideString& a, const CWideString& b) { CWideString r(a.s); std::strcat(r.s, b.s); return r; }
static CWideString operator+(const CWideString& a, const char* b) { CWideString r(a.s); std::strcat(r.s, b); return r; }
static CWideString GetValidProfileName(const CWideString& n) { CWideString r(n.s); for (char* p = r.s; *p; ++p) if (*p == ' ') *p = '_'; return r; }
static CWideString GetSaveDir() { return CWideString("Saves/"); }
struct CFindFileData { char FileName[64]; unsigned long Attributes; };
struct CFindFileHandle { long index; long pad; };
static const char* g_entries[] = { "a.sav", "missing.sav", "subdir", "b.sav" };
static const bool g_exists[] = { true, false, true, true };
static const bool g_isFile[] = { true, true, false, true };
static int g_count = 4;
static char g_pattern[128];
static bool FindFirstFile(const CWideString& pattern, CFindFileHandle& h, CFindFileData& d)
{
    std::strcpy(g_pattern, pattern.s); h.index = 0;
    if (g_count == 0) return false;
    std::strcpy(d.FileName, g_entries[0]); return true;
}
static bool FindNextFile(const CWideString&, CFindFileHandle& h, CFindFileData& d)
{
    if (++h.index >= g_count) return false;
    std::strcpy(d.FileName, g_entries[h.index]); return true;
}
static int Index(const char* n) { for (int i = 0; i < 4; ++i) if (std::strcmp(n, g_entries[i]) == 0) return i; return -1; }
static bool PathExists(const CWideString& p) { int i = Index(p.s); return i >= 0 && g_exists[i]; }
static bool IsPathFile(const CWideString& p) { int i = Index(p.s); return i >= 0 && g_isFile[i]; }
static char g_log[256];
struct CDriveManager {
    long DeleteFile(const CWideString& p) { std::strcat(g_log, "F:"); std::strcat(g_log, p.s); std::strcat(g_log, ";"); return 1; }
    long DeleteDirectory(const CWideString& p) { std::strcat(g_log, "D:"); std::strcat(g_log, p.s); std::strcat(g_log, ";"); return 1; }
};
static CDriveManager g_drive;
static CDriveManager* g_pDriveManager = &g_drive;
static void DeleteProfile(const CWideString& profileName)
{
    CWideString directory = GetSaveDir() + GetValidProfileName(profileName) + "/";
    CWideString pattern = directory + "*";
    CFindFileHandle handle;
    CFindFileData data;
    if (FindFirstFile(pattern, handle, data)) {
        do {
            bool isFile = PathExists(data.FileName) && IsPathFile(data.FileName);
            if (isFile)
                g_pDriveManager->DeleteFile(data.FileName);
        } while (FindNextFile(pattern, handle, data));
    }
    g_pDriveManager->DeleteDirectory(directory);
}
int main()
{
    int bad = 0;
    g_log[0] = 0; DeleteProfile(CWideString("My Hero"));
    if (std::strcmp(g_pattern, "Saves/My_Hero/*") != 0) bad = 1;
    if (std::strcmp(g_log, "F:a.sav;F:b.sav;D:Saves/My_Hero/;") != 0) bad = 1;
    g_count = 0; g_log[0] = 0; DeleteProfile(CWideString("x"));
    if (std::strcmp(g_log, "D:Saves/x/;") != 0) bad = 1;
    if (bad) { std::printf("00407040_TEST FAIL\n"); return 1; }
    std::printf("00407040_TEST PASS\n");
    return 0;
}