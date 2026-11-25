// Behaviour test for 0x004A05C0 CWorld::SaveGameState(const CWideString& name, bool questScreens, bool flag) (NF415: linked): links
// the row object (the function is declared here, not defined) and defines its callees under the row's spellings. A bare file name
// (no directory) is resolved through the profile path (kept as-is when the profile lookup is empty); the memory pool is shut down
// around the save only when the getter returns one and BeginShutdown succeeded; quest-screen saves raise the two flags around the
// hero-state save, other saves delete a stale "<path>.hs" when it exists. Literal checks: the empty-directory tests and the
// profile-path test read retail's pooled wide L"" (0x0122D70C) and the companion extension is L".hs" (0x01236A74).
#include <cstdio>
#include <cstring>

typedef unsigned short wchar16;
extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b);   // 0x00BFE94C, defined here
namespace std {
template <class _CharT, class _IntT> class __char_traits_base { public:
    static int __cdecl compare(const _CharT* __s1, const _CharT* __s2, unsigned int __n);   // 0x00403C60, defined here
};
}
double __fastcall GFGetTime();                          // 0x009E1BC0, defined here

class CWideStringData { public:
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
    long            NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
    CWideString();
    CWideString(const CWideString& other);
    ~CWideString();
    const CWideString& operator=(const CWideString& other);
    bool operator!=(const wchar16* text) const;
};
CWideString __fastcall operator+(const CWideString& left, const wchar16* right);
class CAFile { public: static bool __fastcall PathExists(const CWideString& path);
    static void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name,
                                     CWideString* extension);
};
class CUserProfileManager {
public:
    static CUserProfileManager& __fastcall Get();
    CWideString GetPathNameFromFileName(const CWideString& file, bool b) const;
};
enum EDriveReturn;
class CDriveManager {
public:
    EDriveReturn DeleteFileW(const CWideString& path);
};
struct CDriveManagerPointer {
    CDriveManager* Object;
    void*          Info;
    CDriveManager* operator->() const { return Object; }
};
extern CDriveManagerPointer g_pDriveManager;
class CHandleBasedMemoryPool {
public:
    bool BeginShutdown(float timeout);
    void EndShutdown();
};
#define V6(a, b, c, d, e, f) virtual void a() {} virtual void b() {} virtual void c() {} virtual void d() {} virtual void e() {} virtual void f() {}
class CTestPoolOwner {                                  // the row reads vtable slot 72 (+0x120) of component->+0x28->+0x2C
public:
    V6(v00, v01, v02, v03, v04, v05) V6(v06, v07, v08, v09, v10, v11) V6(v12, v13, v14, v15, v16, v17) V6(v18, v19, v20, v21, v22, v23)
    V6(v24, v25, v26, v27, v28, v29) V6(v30, v31, v32, v33, v34, v35) V6(v36, v37, v38, v39, v40, v41) V6(v42, v43, v44, v45, v46, v47)
    V6(v48, v49, v50, v51, v52, v53) V6(v54, v55, v56, v57, v58, v59) V6(v60, v61, v62, v63, v64, v65) V6(v66, v67, v68, v69, v70, v71)
    virtual CHandleBasedMemoryPool* GetMemoryPool();    // slot 72
};
struct CTestPoolOwnerHolder { unsigned char _pad[0x2c]; CTestPoolOwner* Owner; };
struct CTestMainGameComponent { unsigned char _pad[0x28]; CTestPoolOwnerHolder* Holder; };
bool g_SavingQuestScreenState;                          // 0x013B89D0
bool g_SavingHeroState;                                 // 0x013B89D1
class CWorld {
public:
    void*                   __vftable;
    unsigned char           _pad04[4];
    CTestMainGameComponent* Component;                  // +0x08
    protected: bool SaveGameStateInternal(const CWideString& path, bool flag); public:
protected:
    bool SaveHeroStateInternal(const CWideString& name, bool flag);
public:
    void SaveGameState(const CWideString& name, bool questScreens, bool flag);   // the row (linked)
};

// ---- a small CWideString model: PStringData == 0 is the unassigned string ----
static int g_live, g_badText, g_wcscmpCalls, g_compareCalls, g_neCalls, g_plusCalls;
static bool IsWideEmpty(const unsigned short* s) { return s != 0 && s[0] == 0; }
static unsigned int WLen(const unsigned short* s) { unsigned int n = 0; while (s[n]) ++n; return n; }
static CWideStringData* NewData(const unsigned short* s, unsigned int n)
{
    CWideStringData* d = new CWideStringData; ++g_live;
    d->_M_start = new unsigned short[n + 1]; for (unsigned int i = 0; i < n; ++i) d->_M_start[i] = s[i]; d->_M_start[n] = 0;
    d->_M_finish = d->_M_end_of_storage = d->_M_start + n; d->NoRefs = 1; return d;
}
static void FreeData(CWideStringData* d) { if (d) { delete[] d->_M_start; delete d; --g_live; } }
static CWideString FromAscii(const char* a)
{
    unsigned short w[96]; unsigned int n = 0; while (a[n]) { w[n] = (unsigned char)a[n]; ++n; }
    CWideString s; s.PStringData = NewData(w, n); return s;
}
static void ToAscii(const CWideString& s, char* out)
{
    unsigned int n = 0;
    if (s.PStringData) for (const unsigned short* p = s.PStringData->_M_start; p != s.PStringData->_M_finish; ++p) out[n++] = (char)*p;
    out[n] = 0;
}
CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const CWideString& o) : PStringData(o.PStringData ? NewData(o.PStringData->_M_start, (unsigned int)(o.PStringData->_M_finish - o.PStringData->_M_start)) : 0) {}
CWideString::~CWideString() { FreeData(PStringData); }
const CWideString& CWideString::operator=(const CWideString& o)
{
    if (this != &o) { FreeData(PStringData); PStringData = o.PStringData ? NewData(o.PStringData->_M_start, (unsigned int)(o.PStringData->_M_finish - o.PStringData->_M_start)) : 0; }
    return *this;
}
bool CWideString::operator!=(const wchar16* text) const
{
    ++g_neCalls;
    if (!IsWideEmpty(text)) ++g_badText;                // the row compares the profile path with L""
    unsigned int n = PStringData ? (unsigned int)(PStringData->_M_finish - PStringData->_M_start) : 0;
    if (n != WLen(text)) return true;
    for (unsigned int i = 0; i < n; ++i) if (PStringData->_M_start[i] != text[i]) return true;
    return false;
}
CWideString __fastcall operator+(const CWideString& left, const wchar16* right)
{
    ++g_plusCalls;
    static const unsigned short hs[] = { '.', 'h', 's', 0 };
    for (unsigned int i = 0; i < 4; ++i) if (right[i] != hs[i]) { ++g_badText; break; }   // L".hs"
    unsigned short w[192]; unsigned int n = 0;
    if (left.PStringData) for (const unsigned short* p = left.PStringData->_M_start; p != left.PStringData->_M_finish; ++p) w[n++] = *p;
    for (const unsigned short* p = right; *p; ++p) w[n++] = *p;
    CWideString r; r.PStringData = NewData(w, n); return r;
}
extern "C" int __cdecl wcscmp(const unsigned short* a, const unsigned short* b)
{
    ++g_wcscmpCalls;
    if (!IsWideEmpty(a) || !IsWideEmpty(b) || a != b) ++g_badText;   // retail passes the one pooled L"" for both
    while (*a && *a == *b) { ++a; ++b; }
    return (int)*a - (int)*b;
}
template <> int __cdecl std::__char_traits_base<unsigned short, unsigned short>::compare(const unsigned short* l, const unsigned short* r, unsigned int n)
{
    ++g_compareCalls;
    if (!IsWideEmpty(r) || n != 0) ++g_badText;
    for (unsigned int i = 0; i < n; ++i) if (l[i] != r[i]) return l[i] < r[i] ? -1 : 1;
    return 0;
}
static int g_timeCalls;
double __fastcall GFGetTime() { ++g_timeCalls; return 0.0; }
static int g_dirMode;                                   // 0: bare name -> directory unassigned; 1: "D:\\Saves\\"; 2: assigned but empty
void __fastcall CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    if (extension != 0) ++g_badText;                    // the row passes no extension
    CWideString none; *drive = none;
    if (g_dirMode == 1) *directory = FromAscii("D:\\Saves\\");
    else if (g_dirMode == 2) { CWideString e = FromAscii(""); *directory = e; }
    *name = path;
}
static bool g_hsExists = true; static int g_deleted; static char g_deletedPath[96];
bool __fastcall CAFile::PathExists(const CWideString& path) { char a[96]; ToAscii(path, a); return g_hsExists && std::strlen(a) > 3; }
static CUserProfileManager g_profiles; static const char* g_profileDir = "C:\\Profile\\";
CUserProfileManager& __fastcall CUserProfileManager::Get() { return g_profiles; }
CWideString CUserProfileManager::GetPathNameFromFileName(const CWideString& file, bool b) const
{
    if (!b) ++g_badText;
    if (!g_profileDir) return FromAscii("");
    char f[96], p[192]; ToAscii(file, f); std::strcpy(p, g_profileDir); std::strcat(p, f); return FromAscii(p);
}
EDriveReturn CDriveManager::DeleteFileW(const CWideString& path) { ++g_deleted; ToAscii(path, g_deletedPath); return (EDriveReturn)0; }
static CDriveManager g_drives;
CDriveManagerPointer g_pDriveManager = { &g_drives, 0 };
static int g_beginCalls, g_endCalls; static bool g_beginResult = true;
bool CHandleBasedMemoryPool::BeginShutdown(float timeout) { ++g_beginCalls; return timeout == 2.0f && g_beginResult; }
void CHandleBasedMemoryPool::EndShutdown() { ++g_endCalls; }
static CHandleBasedMemoryPool g_pool; static CHandleBasedMemoryPool* g_poolOrNull = &g_pool;
CHandleBasedMemoryPool* CTestPoolOwner::GetMemoryPool() { return g_poolOrNull; }
static char g_savedPath[192], g_heroName[192]; static bool g_flagsAtSave[2], g_flagsAtHero[2];
bool CWorld::SaveGameStateInternal(const CWideString& path, bool)
{
    ToAscii(path, g_savedPath); g_flagsAtSave[0] = g_SavingQuestScreenState; g_flagsAtSave[1] = g_SavingHeroState; return true;
}
bool CWorld::SaveHeroStateInternal(const CWideString& name, bool)
{
    ToAscii(name, g_heroName); g_flagsAtHero[0] = g_SavingQuestScreenState; g_flagsAtHero[1] = g_SavingHeroState; return true;
}

int main()
{
    static CTestPoolOwner owner; static CTestPoolOwnerHolder holder; static CTestMainGameComponent comp;
    holder.Owner = &owner; comp.Holder = &holder;
    CWorld world; world.__vftable = 0; world.Component = &comp;
    int ok = 1;
    { CWideString n = FromAscii("Save1"); g_dirMode = 0; world.SaveGameState(n, false, false); }   // bare name -> profile path, stale .hs deleted
    if (std::strcmp(g_savedPath, "C:\\Profile\\Save1") != 0 || g_deleted != 1 || std::strcmp(g_deletedPath, "C:\\Profile\\Save1.hs") != 0) ok = 0;
    if (g_beginCalls != 1 || g_endCalls != 1 || g_flagsAtSave[0] || g_flagsAtSave[1] || g_wcscmpCalls != 1 || g_neCalls != 1 || g_plusCalls != 1) ok = 0;
    { CWideString n = FromAscii("D:\\Saves\\Save2"); g_dirMode = 1; world.SaveGameState(n, true, true); }   // explicit directory kept
    if (std::strcmp(g_savedPath, "D:\\Saves\\Save2") != 0 || std::strcmp(g_heroName, "D:\\Saves\\Save2") != 0) ok = 0;
    if (!g_flagsAtSave[0] || g_flagsAtSave[1] || g_flagsAtHero[0] || !g_flagsAtHero[1]) ok = 0;
    if (g_SavingQuestScreenState || g_SavingHeroState || g_deleted != 1 || g_wcscmpCalls != 1 || g_compareCalls != 0 || g_plusCalls != 1) ok = 0;
    g_profileDir = 0; g_beginResult = false; g_hsExists = false;   // empty profile path; shutdown refused; directory assigned but empty
    { CWideString n = FromAscii("Save3"); g_dirMode = 2; world.SaveGameState(n, false, false); }
    if (std::strcmp(g_savedPath, "Save3") != 0 || g_deleted != 1 || g_beginCalls != 3 || g_endCalls != 2) ok = 0;
    if (g_compareCalls != 1 || g_neCalls != 2 || g_plusCalls != 2) ok = 0;
    g_poolOrNull = 0; g_beginResult = true;                         // no pool: neither call
    { CWideString n = FromAscii("Save4"); g_dirMode = 0; world.SaveGameState(n, false, false); }
    if (g_beginCalls != 3 || g_endCalls != 2 || g_timeCalls != 8) ok = 0;
    if (g_badText != 0 || g_live != 0) ok = 0;                      // every literal as retail; no string leaked
    if (!ok) { std::printf("004a05c0_TEST FAIL\n"); return 1; }
    std::printf("004a05c0_TEST PASS\n");
    return 0;
}
