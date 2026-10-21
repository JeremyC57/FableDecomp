#include <cstdio>
#include <stddef.h>
class CWideString {
public:
    void* PData;
    ~CWideString();                                      // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right);   // 0x0099BF30
class CUserProfileManager {
public:
    CWideString GetPathNameFromFileName(const CWideString& file_name, bool create_folder) const;   // 0x00408120
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
class CAFile {
public:
    static bool __fastcall PathExists(const CWideString& path);   // 0x00999230
};
class CWorld {
public:
    bool IsHeroStateFileValid(const CWideString& file_name) const;
};
bool CWorld::IsHeroStateFileValid(const CWideString& file_name) const
{
    const CWideString path = CTBaseSingleton<CUserProfileManager>::Get().GetPathNameFromFileName(file_name + L".hs", false);
    return CAFile::PathExists(path);
}


#include <cwchar>
static int g_dtors; static wchar_t g_pool[8][64]; static int g_next; static wchar_t g_last[64];
CWideString::~CWideString() { ++g_dtors; }
static wchar_t* Make(const wchar_t* a, const wchar_t* b) { wchar_t* p = g_pool[g_next++ & 7]; wcscpy(p, a); wcscat(p, b); return p; }
CWideString __fastcall operator+(const CWideString& left, const wchar_t* right) { CWideString r; r.PData = Make((const wchar_t*)left.PData, right); return r; }
CWideString CUserProfileManager::GetPathNameFromFileName(const CWideString& file_name, bool create_folder) const
{ CWideString r; r.PData = Make(create_folder ? L"X:" : L"P:", (const wchar_t*)file_name.PData); return r; }
static CUserProfileManager g_upm;
template <> CUserProfileManager& __fastcall CTBaseSingleton<CUserProfileManager>::Get() { return g_upm; }
bool __fastcall CAFile::PathExists(const CWideString& path) { wcscpy(g_last, (const wchar_t*)path.PData); return wcscmp(g_last, L"P:hero.hs") == 0; }
int main()
{
    CWorld w;
    static wchar_t hero[] = L"hero"; static wchar_t other[] = L"other";
    CWideString* a = new CWideString; a->PData = hero;
    CWideString* b = new CWideString; b->PData = other;
    bool ok = w.IsHeroStateFileValid(*a) && wcscmp(g_last, L"P:hero.hs") == 0 && !w.IsHeroStateFileValid(*b) && wcscmp(g_last, L"P:other.hs") == 0 && g_dtors >= 4;
    if (ok) std::printf("0049d4e0_TEST PASS\n");
    return 0;
}