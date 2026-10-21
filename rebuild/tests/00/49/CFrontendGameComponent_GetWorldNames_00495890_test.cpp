#include <cstdio>
#include <stddef.h>
#include <new>
struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const wchar_t* text);           // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    CWideString& operator=(const wchar_t* text);   // 0x0099B800
};
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
struct CFindFileData {
    wchar_t       FileName[260];
    unsigned long Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
CWideString __fastcall GetLevelsDirectory();    // 0x0041A060 (L"Data\\Levels\\")

namespace std {
struct __false_type { __false_type() {} };
template <class T> struct less { less() {} };
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp);   // 0x004977D0 for CWideString*
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                         // 0x00412450 for CWideString
    void _M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00412330
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    void clear() { erase(begin(), end()); }
};
}

class CFrontendGameComponent {
public:
    unsigned char             _pad0000[0x1e30];
    std::vector<CWideString>  WorldNames;               // +0x1e30
    long                      WorldNamesOffset;         // +0x1e3c
    CWideString               WorldToLoad;              // +0x1e40
    void GetWorldNames();
};

void CFrontendGameComponent::GetWorldNames()
{
    WorldToLoad = L"";
    WorldNames.clear();
    CWideString pattern = GetLevelsDirectory() + L"*.wld";
    CFindFileHandle handle;
    CFindFileData data;
    if (FindFirstFile(pattern, handle, data)) {
        do {
            CWideString fileName(data.FileName);
            CWideString name;
            SplitPath(fileName, 0, 0, &name, 0);
            WorldNames.push_back(name);
        } while (FindNextFile(pattern, handle, data));
    }
    std::sort(WorldNames.begin(), WorldNames.end(), std::less<CWideString>());
    WorldNamesOffset = 0;
}

#include <string.h>
#include <stdlib.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { printf("FAIL %s\n", w); g_fail = 1; } }
// CWideString stub: PStringData points at a malloc'd NUL-terminated wide buffer (no sharing; copies duplicate)
static wchar_t* Dup(const wchar_t* s) { size_t n = wcslen(s); wchar_t* d = (wchar_t*)malloc((n + 1) * 2); memcpy(d, s, (n + 1) * 2); return d; }
static const wchar_t* W(const CWideString& s) { return s.PStringData ? (const wchar_t*)s.PStringData : L""; }
static int g_live = 0;
CWideString::CWideString() : PStringData(0) { ++g_live; }
CWideString::CWideString(const wchar_t* text) : PStringData((CWideStringData*)Dup(text)) { ++g_live; }
CWideString::CWideString(const CWideString& other) : PStringData((CWideStringData*)Dup(W(other))) { ++g_live; }
CWideString::~CWideString() { free(PStringData); PStringData = 0; --g_live; }
CWideString& CWideString::operator=(const wchar_t* text) { free(PStringData); PStringData = (CWideStringData*)Dup(text); return *this; }
CWideString __fastcall operator+(const CWideString& a, const wchar_t* b)
{
    wchar_t buf[300]; wcscpy(buf, W(a)); wcscat(buf, b); return CWideString(buf);
}
static wchar_t g_pattern[300];
CWideString __fastcall GetLevelsDirectory() { return CWideString(L"Data\\Levels\\"); }
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    if (drive || directory || extension || !name) { printf("FAIL SplitPath args\n"); g_fail = 1; }
    wchar_t buf[300]; wcscpy(buf, W(path)); wchar_t* dot = wcsrchr(buf, L'.'); if (dot) *dot = 0;
    *name = buf;
}
static const wchar_t* g_files[8]; static int g_fileCount = 0, g_fileIndex = 0, g_findFirst = 0, g_findNext = 0;
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data)
{
    ++g_findFirst; wcscpy(g_pattern, W(pattern)); g_fileIndex = 0;
    if (g_fileCount == 0) return false;
    wcscpy(data.FileName, g_files[g_fileIndex++]); return true;
}
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data)
{
    ++g_findNext;
    if (g_fileIndex >= g_fileCount) return false;
    wcscpy(data.FileName, g_files[g_fileIndex++]); return true;
}
static int g_sorts = 0;
namespace std {
template <class T> T* vector<T>::erase(T* first, T* last)
{
    for (T* p = first; p != last; ++p) p->~T();
    T* d = first; for (T* s = last; s != _M_finish; ++s, ++d) { d->PStringData = s->PStringData; }
    _M_finish = d; return first;
}
template <class T> void vector<T>::_M_insert_overflow(T* pos, const T& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    size_t n = _M_finish - _M_start, cap = n * 2 + 1;
    T* nb = (T*)malloc(cap * sizeof(T));
    for (size_t i = 0; i < n; ++i) nb[i].PStringData = _M_start[i].PStringData;   // move ownership
    new (nb + n) T(x);
    free(_M_start); _M_start = nb; _M_finish = nb + n + 1; _M_end_of_storage = nb + cap;
}
template <class It, class Cmp> void __fastcall sort(It first, It last, Cmp comp)
{
    ++g_sorts;
    for (It i = first; i != last; ++i)
        for (It j = i + 1; j != last; ++j)
            if (wcscmp(W(*j), W(*i)) < 0) { CWideStringData* t = i->PStringData; i->PStringData = j->PStringData; j->PStringData = t; }
}
}
int main()
{
    static CFrontendGameComponent c;
    c.WorldToLoad.PStringData = (CWideStringData*)Dup(L"Old");
    c.WorldNamesOffset = 7;
    // no files: list cleared, sorted (empty), offset reset
    check((c.GetWorldNames(), true), "run");
    check(wcscmp(W(c.WorldToLoad), L"") == 0, "selection cleared");
    check(wcscmp(g_pattern, L"Data\\Levels\\*.wld") == 0, "pattern");
    check(c.WorldNames._M_finish == c.WorldNames._M_start && g_findNext == 0 && g_sorts == 1 && c.WorldNamesOffset == 0, "empty");
    // three files, unsorted; second run clears the previous list first
    g_files[0] = L"Zeta.wld"; g_files[1] = L"Alpha.wld"; g_files[2] = L"Mid.wld"; g_fileCount = 3;
    c.WorldNamesOffset = 5;
    c.GetWorldNames();
    c.GetWorldNames();
    check(c.WorldNames._M_finish - c.WorldNames._M_start == 3, "three names after rerun");
    check(wcscmp(W(c.WorldNames._M_start[0]), L"Alpha") == 0 && wcscmp(W(c.WorldNames._M_start[1]), L"Mid") == 0
          && wcscmp(W(c.WorldNames._M_start[2]), L"Zeta") == 0, "sorted, extensions stripped");
    check(g_findFirst == 3 && g_findNext == 6 && g_sorts == 3 && c.WorldNamesOffset == 0, "find calls");
    check(g_live == 4, "no leaked temporaries (WorldToLoad + three names)");
    if (!g_fail) printf("00495890_TEST PASS\n");
    return g_fail;
}