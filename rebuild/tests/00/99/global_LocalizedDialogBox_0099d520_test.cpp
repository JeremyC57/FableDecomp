// Behaviour test for 0x0099D520 LocalizedDialogBox: module language first, then US English (unless the module
// language is 0x409), then DialogBoxParamA; -1 means "try the next".
#include <cstdio>
#include <cstring>
#include <cstdlib>

typedef unsigned short wchar16;
static int g_failed = 0;
#define CHECK(cond) do { if (!(cond)) { std::printf("check failed at line %d\n", __LINE__); g_failed = 1; } } while (0)

// Test-side value strings (plain C++98 stand-ins for CWideString / CCharString).
static unsigned WLen(const wchar16* s) { unsigned n = 0; if (s) while (s[n]) ++n; return n; }
class WStr {
public:
    wchar16* d;
    WStr() : d(0) { }
    WStr(const char* s) : d(0) { SetA(s); }
    WStr(const wchar16* s) : d(0) { SetW(s); }
    WStr(const WStr& o) : d(0) { SetW(o.d); }
    ~WStr() { std::free(d); }
    WStr& operator=(const WStr& o) { if (this != &o) { wchar16* old = d; d = 0; SetW(o.d); std::free(old); } return *this; }
    WStr& operator=(const wchar16* s) { wchar16* old = d; d = 0; SetW(s); std::free(old); return *this; }
    void SetW(const wchar16* s)
    {
        unsigned n = WLen(s);
        if (n == 0) { d = 0; return; }
        d = (wchar16*)std::malloc((n + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < n; ++i) d[i] = s[i];
        d[n] = 0;
    }
    void SetA(const char* s)
    {
        unsigned n = s ? (unsigned)std::strlen(s) : 0;
        if (n == 0) { d = 0; return; }
        d = (wchar16*)std::malloc((n + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < n; ++i) d[i] = (unsigned char)s[i];
        d[n] = 0;
    }
    const wchar16* Text() const { static const wchar16 empty = 0; return d ? d : &empty; }
    unsigned Length() const { return WLen(d); }
    WStr operator+(const WStr& o) const
    {
        unsigned a = Length(), b = o.Length();
        wchar16* t = (wchar16*)std::malloc((a + b + 1) * sizeof(wchar16));
        for (unsigned i = 0; i < a; ++i) t[i] = d[i];
        for (unsigned j = 0; j < b; ++j) t[a + j] = o.d[j];
        t[a + b] = 0;
        WStr r(t);
        std::free(t);
        return r;
    }
    bool Eq(const char* s) const
    {
        unsigned n = (unsigned)std::strlen(s);
        if (Length() != n) return false;
        for (unsigned i = 0; i < n; ++i) if (d[i] != (unsigned char)s[i]) return false;
        return true;
    }
};
class CStr {
public:
    char* d;
    CStr() : d(0) { }
    CStr(const char* s) : d(0) { Set(s); }
    CStr(const CStr& o) : d(0) { Set(o.d); }
    ~CStr() { std::free(d); }
    CStr& operator=(const CStr& o) { if (this != &o) { char* old = d; d = 0; Set(o.d); std::free(old); } return *this; }
    void Set(const char* s)
    {
        unsigned n = s ? (unsigned)std::strlen(s) : 0;
        if (n == 0) { d = 0; return; }
        d = (char*)std::malloc(n + 1);
        std::memcpy(d, s, n + 1);
    }
    const char* Text() const { return d ? d : ""; }
    bool Eq(const char* s) const { return std::strcmp(Text(), s) == 0; }
};
static WStr Widen(const CStr& s) { return WStr(s.Text()); }
static CStr Narrow(const WStr& s)
{
    unsigned n = s.Length();
    char* t = (char*)std::malloc(n + 1);
    for (unsigned i = 0; i < n; ++i) t[i] = (char)s.d[i];
    t[n] = 0;
    CStr r(t);
    std::free(t);
    return r;
}

// Path stand-ins shared by the CAFile tests: the current path is C:\Game; ExpandPath joins a relative path onto it.
static WStr GetCurrentPath() { return WStr("C:\\Game"); }
static WStr ExpandPath(const WStr& path, const WStr& base)
{
    const wchar16* p = path.Text();
    if (WLen(p) >= 2 && p[1] == ':')
        return path;
    return base + WStr("\\") + path;
}

// Resource stand-ins: one module with an English string-table block 3 (ids 0x20..0x2F) and a French one for id 0x23,
// plus an English dialog template "ABOUT".
struct HINSTANCE__ { int unused; };
typedef HINSTANCE__* HINSTANCE;
struct SFakeResource { unsigned long type; unsigned long name; unsigned short language; const void* data; };
static wchar16 g_englishBlock[64];
static wchar16 g_frenchBlock[64];
static const char g_aboutTemplate[] = "DLGTEMPLATE";
static SFakeResource g_resources[3];
static unsigned BuildBlock(wchar16* block, unsigned entry, const char* text)
{
    unsigned k = 0, i, n = (unsigned)std::strlen(text);
    for (unsigned e = 0; e < 16; ++e) {
        if (e == entry) { block[k++] = (wchar16)n; for (i = 0; i < n; ++i) block[k++] = (unsigned char)text[i]; }
        else if (e == 1) { block[k++] = 2; block[k++] = 'x'; block[k++] = 'y'; }   // another entry, before 3
        else block[k++] = 0;
    }
    return k;
}
static void SetupResources()
{
    BuildBlock(g_englishBlock, 3, "Continue");
    BuildBlock(g_frenchBlock, 3, "Continuer");
    SFakeResource en = { 6, 3, 0x409, g_englishBlock }, fr = { 6, 3, 0x40C, g_frenchBlock },
                  dlg = { 5, 0x1234, 0x409, g_aboutTemplate };
    g_resources[0] = en; g_resources[1] = fr; g_resources[2] = dlg;
}
static SFakeResource* FindResourceExA(HINSTANCE, const char* type, const char* name, unsigned short language)
{
    for (unsigned i = 0; i < 3; ++i)
        if (g_resources[i].type == (unsigned long)type && g_resources[i].name == (unsigned long)name &&
            g_resources[i].language == language)
            return &g_resources[i];
    return 0;
}
static const void* LoadResource(HINSTANCE, SFakeResource* resource) { return resource ? resource->data : 0; }
static const void* LockResource(const void* data) { return data; }
// Dialog stand-ins: DialogBoxIndirectParamA "runs" a located template and returns 100; DialogBoxParamA returns 200.
struct HWND__ { int unused; };
typedef HWND__* HWND;
typedef int (*DLGPROC)(HWND, unsigned int, unsigned int, long);
static const void* g_lastTemplate = 0;
static int g_indirectCalls = 0, g_paramCalls = 0;
static int g_indirectResult = 100;
static int DialogBoxIndirectParamA(HINSTANCE, const void* dialogTemplate, HWND, DLGPROC, long initParam)
{
    ++g_indirectCalls;
    g_lastTemplate = dialogTemplate;
    return initParam == 0 ? g_indirectResult : -2;
}
static int DialogBoxParamA(HINSTANCE, const char*, HWND, DLGPROC, long) { ++g_paramCalls; return 200; }
static int TestProc(HWND, unsigned int, unsigned int, long) { return 0; }
static unsigned short g_ModuleLanguage = 0x409;
static int LocalizedDialogBoxByLanguage(HINSTANCE hInstance, const char* templateName, HWND parent, DLGPROC dialogProc,
                                        unsigned short language)
{
    SFakeResource* resource = FindResourceExA(hInstance, (const char*)5, templateName, language);
    if (resource == 0)
        return -1;
    const void* data = LoadResource(hInstance, resource);
    if (data == 0)
        return -1;
    const void* dialogTemplate = LockResource(data);
    if (dialogTemplate == 0)
        return -1;
    return DialogBoxIndirectParamA(hInstance, dialogTemplate, parent, dialogProc, 0);
}
static int LocalizedDialogBox(HINSTANCE hInstance, const char* templateName, HWND parent, DLGPROC dialogProc)
{
    int result = LocalizedDialogBoxByLanguage(hInstance, templateName, parent, dialogProc, g_ModuleLanguage);
    if (result != -1)
        return result;
    if (g_ModuleLanguage != 0x409) {
        result = LocalizedDialogBoxByLanguage(hInstance, templateName, parent, dialogProc, 0x409);
        if (result != -1)
            return result;
    }
    return DialogBoxParamA(hInstance, templateName, parent, dialogProc, 0);
}
int main()
{
    SetupResources();
    CHECK(LocalizedDialogBox(0, (const char*)0x1234, 0, TestProc) == 100 && g_indirectCalls == 1);
    g_ModuleLanguage = 0x40C;                            // French module: falls back to the English template
    CHECK(LocalizedDialogBox(0, (const char*)0x1234, 0, TestProc) == 100 && g_indirectCalls == 2 && g_paramCalls == 0);
    CHECK(LocalizedDialogBox(0, (const char*)0x7777, 0, TestProc) == 200 && g_paramCalls == 1);
    g_ModuleLanguage = 0x409; g_indirectResult = -1;     // a dialog that itself returns -1 also falls through
    CHECK(LocalizedDialogBox(0, (const char*)0x1234, 0, TestProc) == 200 && g_paramCalls == 2 && g_indirectCalls == 3);
    if (g_failed) { std::printf("0099d520_TEST FAIL\n"); return 1; }
    std::printf("0099d520_TEST PASS\n");
    return 0;
}