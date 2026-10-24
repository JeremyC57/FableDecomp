// Behaviour test for 0x0099D3A0 RetrieveStringByLanguage: block (id >> 4) + 1, entry id & 15, converted to ANSI with
// the terminator at min(length, size - 1); a missing block or an empty entry gives 0.
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
static int WideCharToMultiByte(unsigned, unsigned long, const wchar16* wide, int count, char* text, int size, const char*, int*)
{
    int n = count < size ? count : size;
    for (int i = 0; i < n; ++i) text[i] = (char)wide[i];
    return n;
}
static int RetrieveStringByLanguage(HINSTANCE hInstance, unsigned long id, char* buffer, unsigned long bufferSize,
                                    unsigned short language)
{
    unsigned long entry = id & 15;
    SFakeResource* resource = FindResourceExA(hInstance, (const char*)6, (const char*)(unsigned long)(unsigned short)((id >> 4) + 1), language);
    if (resource != 0) {
        const void* data = LoadResource(hInstance, resource);
        if (data != 0) {
            const wchar16* text = (const wchar16*)LockResource(data);
            if (text != 0) {
                for (unsigned long i = 0; i < 16; i++) {
                    unsigned int length = *text++;
                    if (length != 0 && i == entry) {
                        int converted = WideCharToMultiByte(0, 0, text, length, buffer, bufferSize, 0, 0);
                        if (converted == 0)
                            return 0;
                        int last = (int)bufferSize - 1;
                        buffer[converted < last ? converted : last] = 0;
                        return converted;
                    }
                    text += length;
                }
            }
        }
    }
    return 0;
}
int main()
{
    SetupResources();
    char buffer[32];
    HINSTANCE module = 0;
    CHECK(RetrieveStringByLanguage(module, 0x23, buffer, sizeof(buffer), 0x409) == 8 && std::strcmp(buffer, "Continue") == 0);
    CHECK(RetrieveStringByLanguage(module, 0x23, buffer, sizeof(buffer), 0x40C) == 9 && std::strcmp(buffer, "Continuer") == 0);
    CHECK(RetrieveStringByLanguage(module, 0x21, buffer, sizeof(buffer), 0x409) == 2 && std::strcmp(buffer, "xy") == 0);
    char small[5];
    CHECK(RetrieveStringByLanguage(module, 0x23, small, sizeof(small), 0x409) == 5 && std::strcmp(small, "Cont") == 0);
    CHECK(RetrieveStringByLanguage(module, 0x24, buffer, sizeof(buffer), 0x409) == 0);     // empty entry
    CHECK(RetrieveStringByLanguage(module, 0x53, buffer, sizeof(buffer), 0x409) == 0);     // no such block
    CHECK(RetrieveStringByLanguage(module, 0x23, buffer, sizeof(buffer), 0x407) == 0);     // no German block
    if (g_failed) { std::printf("0099d3a0_TEST FAIL\n"); return 1; }
    std::printf("0099d3a0_TEST PASS\n");
    return 0;
}