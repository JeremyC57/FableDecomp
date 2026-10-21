#include <cstdio>
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count);   // 0x00403C60
struct CWideStringData {                                // STLport basic_string<wchar_t> + NoRefs
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                     // 0x0099B510
    __forceinline bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        return PStringData->size() == other.PStringData->size()
            && MemCmp_Unsigned16(PStringData->_M_start, other.PStringData->_M_start, PStringData->size()) == 0;
    }
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30
bool __fastcall PathExists(const CWideString& path);   // 0x00999230

struct CUserProfileManager {
    static CUserProfileManager& __fastcall Get();       // 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;   // 0x00408120
    CWideString GetAutoSaveRegionLeavingPathName() const;   // 0x004080B0
};
struct CDriveManager {
    long DeleteFile(const CWideString& path);           // 0x009E4BC0
};
struct CDriveManagerPointer {                           // static CCountedPointer<CDriveManager>
    CDriveManager* Object;
    void*          Info;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;            // 0x013CAA28

struct CWorld {
    unsigned char _pad[0x102];
    bool          SerialisingHeroState;                 // +0x102 (PDB +0x10E)
    bool LoadGameStateInternal(const CWideString& path, bool b);   // 0x004A21F0
    bool LoadHeroStateInternal(const CWideString& name);
};

bool CWorld::LoadHeroStateInternal(const CWideString& name)
{
    CWideString file = name + L".hs";
    CWideString path = CUserProfileManager::Get().GetPathNameFromFileName(file, false);
    if (PathExists(path))
    {
        SerialisingHeroState = true;
        bool loaded = LoadGameStateInternal(path, false);
        SerialisingHeroState = false;
        if (loaded)
        {
            CWideString autosave = CUserProfileManager::Get().GetAutoSaveRegionLeavingPathName() + L".hs";
            if (path == autosave)
                g_pDriveManager->DeleteFile(autosave);
        }
        return loaded;
    }
    return true;
}

#include <string.h>
#include <stdlib.h>
static int g_fail = 0;
static int g_dtors = 0, g_memcmp = 0, g_loads = 0, g_deletes = 0;
static void check(bool c, const char* what) { if (!c) { printf("FAIL %s (dtors %d loads %d deletes %d memcmp %d)\n", what, g_dtors, g_loads, g_deletes, g_memcmp); g_fail = 1; } }
static CWideStringData* MakeData(const unsigned short* a, unsigned long na, const unsigned short* b, unsigned long nb)
{
    CWideStringData* d = (CWideStringData*)malloc(sizeof(CWideStringData));
    d->_M_start = (unsigned short*)malloc((na + nb + 1) * 2);
    memcpy(d->_M_start, a, na * 2);
    memcpy(d->_M_start + na, b, nb * 2);
    d->_M_finish = d->_M_start + na + nb;
    *d->_M_finish = 0;
    d->_M_end_of_storage = d->_M_finish + 1;
    d->NoRefs = 0;
    return d;
}
static unsigned long WLen(const unsigned short* s) { unsigned long n = 0; while (s[n]) ++n; return n; }
static CWideString Make(const char* s)
{
    unsigned short buf[128]; unsigned long n = 0;
    while (s[n]) { buf[n] = (unsigned char)s[n]; ++n; }
    CWideString w; w.PStringData = MakeData(buf, n, buf, 0); return w;
}
static bool Equals(const CWideString& w, const char* s)
{
    unsigned long n = strlen(s);
    if ((unsigned long)(w.PStringData->_M_finish - w.PStringData->_M_start) != n) return false;
    for (unsigned long i = 0; i < n; ++i) if (w.PStringData->_M_start[i] != (unsigned char)s[i]) return false;
    return true;
}
//
static bool g_exists = true, g_loadResult = true, g_latchDuringLoad = false;
static const char* g_autosave = "P/AutoSave.rl";
static char g_deleted[128];
CWideString::~CWideString() { ++g_dtors; }
int __cdecl MemCmp_Unsigned16(const unsigned short* lhs, const unsigned short* rhs, unsigned long count)
{
    ++g_memcmp;
    for (unsigned long i = 0; i < count; ++i) if (lhs[i] != rhs[i]) return lhs[i] < rhs[i] ? -1 : 1;
    return 0;
}
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right)
{
    CWideString r;
    r.PStringData = MakeData(left.PStringData->_M_start, left.PStringData->_M_finish - left.PStringData->_M_start,
                             (const unsigned short*)right, WLen((const unsigned short*)right));
    return r;
}
static CWideString g_lastPathChecked;
bool __fastcall PathExists(const CWideString& path) { g_lastPathChecked = path; return g_exists; }
static CUserProfileManager g_profiles;
CUserProfileManager& __fastcall CUserProfileManager::Get() { return g_profiles; }
CWideString CUserProfileManager::GetPathNameFromFileName(const CWideString& file, bool b) const
{
    check(!b, "GetPathNameFromFileName flag false");
    static const unsigned short prefix[] = { 'P', '/' };
    CWideString r;
    r.PStringData = MakeData(prefix, 2, file.PStringData->_M_start, file.PStringData->_M_finish - file.PStringData->_M_start);
    return r;
}
CWideString CUserProfileManager::GetAutoSaveRegionLeavingPathName() const { return Make(g_autosave); }
long CDriveManager::DeleteFile(const CWideString& path)
{
    ++g_deletes;
    unsigned long n = path.PStringData->_M_finish - path.PStringData->_M_start;
    for (unsigned long i = 0; i < n; ++i) g_deleted[i] = (char)path.PStringData->_M_start[i];
    g_deleted[n] = 0;
    return 1;
}
static CDriveManager g_drive;
CDriveManagerPointer g_pDriveManager = { &g_drive, 0 };
static CWorld* g_world;
bool CWorld::LoadGameStateInternal(const CWideString& path, bool b)
{
    ++g_loads;
    check(!b, "LoadGameStateInternal flag false");
    check(Equals(path, "P/AutoSave.rl.hs") || Equals(path, "P/Hero1.hs"), "load path");
    g_latchDuringLoad = SerialisingHeroState;
    return g_loadResult;
}
static void Reset() { g_dtors = g_memcmp = g_loads = g_deletes = 0; g_deleted[0] = 0; g_latchDuringLoad = false; }
int main()
{
    static CWorld world;
    g_world = &world;
    // 1. missing companion -> true, no load
    Reset(); g_exists = false;
    CWideString n1 = Make("Hero1");
    check(world.LoadHeroStateInternal(n1) == true, "missing returns true");
    check(Equals(g_lastPathChecked, "P/Hero1.hs"), "path built");
    check(g_loads == 0 && g_dtors == 5, "missing: no load, file+path dtors");   // + 3 stub return copies (Make, operator+, GetPathNameFromFileName; no NRVO in the test build)
    // 2. present, load fails -> false, no autosave check
    Reset(); g_exists = true; g_loadResult = false;
    check(world.LoadHeroStateInternal(n1) == false, "load fail returns false");
    check(g_loads == 1 && g_latchDuringLoad && !world.SerialisingHeroState, "latch set during load only");
    check(g_deletes == 0 && g_dtors == 4, "fail: no delete");   // file + path + 2 stub copies
    // 3. loaded, not the autosave (length differs) -> true, no delete, no memcmp
    Reset(); g_loadResult = true;
    check(world.LoadHeroStateInternal(n1) == true, "load ok returns true");
    check(g_deletes == 0 && g_memcmp == 0 && g_dtors == 8, "non-autosave: no delete");   // + autosave temp and autosave + 2 stub copies
    // 4. loaded, same length but different text -> memcmp, no delete
    Reset(); g_autosave = "P/AutoSave.xx";
    CWideString n2 = Make("AutoSave.rl");
    check(world.LoadHeroStateInternal(n2) == true, "same-length returns true");
    check(g_memcmp == 1 && g_deletes == 0, "same-length: compared, no delete");
    // 5. loaded autosave companion -> deleted
    Reset(); g_autosave = "P/AutoSave.rl";
    check(world.LoadHeroStateInternal(n2) == true, "autosave returns true");
    check(g_memcmp == 1 && g_deletes == 1 && strcmp(g_deleted, "P/AutoSave.rl.hs") == 0, "autosave deleted");
    check(!world.SerialisingHeroState, "latch cleared");
    if (!g_fail) printf("004a2d70_TEST PASS\n");
    return g_fail;
}