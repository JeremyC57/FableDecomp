#include <cstdio>
#include <cstring>

// Behaviour model of CWorld::DeleteCheckPointSave (0x0049E740): delete the check-point save and its ".hs" companion.
class CWideString { public:
    char s[128];
    CWideString(const char* p) { std::strcpy(s, p); }
    CWideString(const CWideString& o) { std::strcpy(s, o.s); }
};
static CWideString operator+(const CWideString& a, const char* b) { CWideString r(a); std::strcat(r.s, b); return r; }

static char g_files[8][128];
static int g_nFiles = 0;
static int g_deletes = 0;

static int Find(const char* p) { for (int i = 0; i < g_nFiles; ++i) if (std::strcmp(g_files[i], p) == 0) return i; return -1; }

class CAFile { public: static bool PathExists(const CWideString& path) { return Find(path.s) >= 0; } };
class CUserProfileManager { public: CWideString GetAutoSaveCheckPointPathName() const { return CWideString("Saves\\Profile1\\CheckPoint.sav"); } };
static CUserProfileManager g_profiles;
class CDriveManager { public:
    int DeleteFile(const CWideString& path)
    {
        ++g_deletes;
        int i = Find(path.s);
        if (i < 0) return 1;
        --g_nFiles;
        if (i != g_nFiles) std::strcpy(g_files[i], g_files[g_nFiles]);
        return 0;
    }
};
static CDriveManager g_drives;

static void DeleteCheckPointSave()
{
    CWideString path = g_profiles.GetAutoSaveCheckPointPathName();
    if (CAFile::PathExists(path)) {
        g_drives.DeleteFile(path);
        CWideString hsPath = path + ".hs";
        if (CAFile::PathExists(hsPath))
            g_drives.DeleteFile(hsPath);
    }
}

static void Add(const char* p) { std::strcpy(g_files[g_nFiles++], p); }

int main()
{
    // nothing saved: no delete attempted
    g_nFiles = 0; g_deletes = 0; Add("Saves\\Profile1\\CheckPoint.sav.hs");
    DeleteCheckPointSave();
    if (g_deletes != 0 || g_nFiles != 1) { std::printf("0049e740_TEST FAIL\n"); return 1; }
    // save without companion
    g_nFiles = 0; g_deletes = 0; Add("Saves\\Profile1\\CheckPoint.sav"); Add("Other.sav");
    DeleteCheckPointSave();
    if (g_deletes != 1 || g_nFiles != 1 || Find("Other.sav") != 0) { std::printf("0049e740_TEST FAIL\n"); return 1; }
    // save and companion
    g_nFiles = 0; g_deletes = 0; Add("Saves\\Profile1\\CheckPoint.sav.hs"); Add("Saves\\Profile1\\CheckPoint.sav");
    DeleteCheckPointSave();
    if (g_deletes != 2 || g_nFiles != 0) { std::printf("0049e740_TEST FAIL\n"); return 1; }
    std::printf("0049e740_TEST PASS\n");
    return 0;
}