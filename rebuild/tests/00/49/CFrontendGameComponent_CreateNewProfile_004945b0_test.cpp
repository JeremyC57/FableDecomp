#include <cstdio>
#include <stddef.h>
class CWideString {
public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    CWideString(const CWideString& other);               // 0x0099B720
    ~CWideString();                                      // 0x0099B510
    void __cdecl Format(const wchar_t* format, ...);     // 0x0099BA70
};
CWideString __fastcall operator+(const wchar_t* left, const CWideString& right);   // 0x0099BFF0
inline CWideString NumberToWideString(long number)
{
    CWideString text;
    text.Format(L"%d", number);
    return text;
}
class CUserProfileManager {
public:
    CWideString GetProfileDir(const CWideString& name, bool create);   // 0x004082D0
    void SetProfileName(const CWideString& name, bool load);           // 0x0040D400
    void SetDefaultValues();                                           // 0x0040C790 (stores the defaults; see 0x0059697A)
};
template <class T> class CTBaseSingleton {
public:
    static T& __fastcall Get();                          // 0x0040D2A0 for CUserProfileManager
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
};
}
class CFrontendGameComponent {
public:
    unsigned char            _pad0000[0x1e0c];
    std::vector<CWideString> ProfileNames;               // +0x1e0c
    CWideString CreateNewProfile();
};
CWideString CFrontendGameComponent::CreateNewProfile()
{
    CWideString name = L"NewProfile" + NumberToWideString(ProfileNames.size() + 1);
    CTBaseSingleton<CUserProfileManager>::Get().GetProfileDir(name, true);
    CTBaseSingleton<CUserProfileManager>::Get().SetProfileName(name, false);
    CTBaseSingleton<CUserProfileManager>::Get().SetDefaultValues();
    return name;
}

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
static int g_live;
struct WData { wchar_t s[64]; };
CWideString::CWideString() { WData* d = new WData; d->s[0] = 0; PStringData = d; ++g_live; }
CWideString::CWideString(const CWideString& o) { WData* d = new WData; *d = *(WData*)o.PStringData; PStringData = d; ++g_live; }
CWideString::~CWideString() { delete (WData*)PStringData; --g_live; }
void __cdecl CWideString::Format(const wchar_t* f, ...) { va_list a; va_start(a, f); _vsnwprintf(((WData*)PStringData)->s, 63, f, a); va_end(a); }
CWideString __fastcall operator+(const wchar_t* l, const CWideString& r)
{ CWideString x; wcscpy(((WData*)x.PStringData)->s, l); wcscat(((WData*)x.PStringData)->s, ((WData*)r.PStringData)->s); return x; }
static wchar_t g_dir[64], g_set[64]; static bool g_create, g_load; static int g_saves, g_order[8], g_n;
CWideString CUserProfileManager::GetProfileDir(const CWideString& n, bool c) { g_order[g_n++] = 1; wcscpy(g_dir, ((WData*)n.PStringData)->s); g_create = c; return n; }
void CUserProfileManager::SetProfileName(const CWideString& n, bool l) { g_order[g_n++] = 2; wcscpy(g_set, ((WData*)n.PStringData)->s); g_load = l; }
void CUserProfileManager::SetDefaultValues() { g_order[g_n++] = 3; ++g_saves; }
static CUserProfileManager g_upm;
template <class T> T& __fastcall CTBaseSingleton<T>::Get() { return g_upm; }
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)]; CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    static CWideString* fake = (CWideString*)8;
    c.ProfileNames._M_start = fake; c.ProfileNames._M_finish = fake + 2;
    bool ok;
    {
        CWideString r = c.CreateNewProfile();
        ok = wcscmp(((WData*)r.PStringData)->s, L"NewProfile3") == 0 && wcscmp(g_dir, L"NewProfile3") == 0 && g_create
          && wcscmp(g_set, L"NewProfile3") == 0 && !g_load && g_saves == 1 && g_n == 3 && g_order[0] == 1 && g_order[1] == 2
          && g_order[2] == 3 && g_live == 1;
    }
    ok = ok && g_live == 0;
    std::printf(ok ? "004945b0_TEST PASS\n" : "004945b0_TEST FAIL\n");
    return ok ? 0 : 1;
}