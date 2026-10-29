#include <cstdio>
#include <string.h>
class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString();                              // 0x0099AED0
    CWideString(const unsigned short* text);    // 0x0099B6B0
    CWideString(const CWideString& other);      // 0x0099B720
    ~CWideString();                             // 0x0099B510
    const CWideString& operator+=(const unsigned short* text);   // 0x0099B940
    class CCharString ConvertToCharString() const;          // 0x0099B2C0
};
class CCharStringData { public:
    char* Text;
    long  Length;
};
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    __forceinline long Length() const { return PStringData == 0 ? 0 : PStringData->Length; }
    __forceinline const char* c_str() const { return PStringData == 0 ? "" : PStringData->Text; }
    __forceinline bool EndsWith(const CCharString& suffix) const
    {
        long offset = Length() - suffix.Length();
        if (offset >= 0)
            return strcmp(c_str() + offset, suffix.c_str()) == 0;
        return false;
    }
};
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b);       // 0x0099BF30
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension);   // 0x00997620
bool __fastcall IsPathFile(const CWideString& path);   // 0x00999A30
struct CFindFileData {
    unsigned short FileName[260];
    unsigned long  Attributes;
};
struct CFindFileHandle {
    long _body[2];
};
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);   // 0x00999760
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data);    // 0x00999850
namespace NResourceDirectoryNames {
CWideString __fastcall GetSaveDir();            // 0x0041A540
}
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    T* erase(T* first, T* last);                // 0x00412450 for CWideString
    void push_back(const T& x);                 // 0x004128A0 for CWideString
    void clear() { erase(begin(), end()); }
};
}
namespace NUISystem {
class CFrontEndManager {
public:
    void GetProfileNames(std::vector<CWideString>& names);
};
void CFrontEndManager::GetProfileNames(std::vector<CWideString>& names)
{
    names.clear();
    CWideString pattern = NResourceDirectoryNames::GetSaveDir() + (const unsigned short*)L"*";
    CFindFileHandle handle;
    CFindFileData data;
    bool found = FindFirstFile(pattern, handle, data);
    while (found) {
        CWideString fileName(data.FileName);
        bool dots;
        {
            CCharString dot(".");
            dots = fileName.ConvertToCharString().EndsWith(dot);
        }
        if (!dots && !IsPathFile(pattern)) {
            fileName += (const unsigned short*)L"\\";
            CWideString dir;
            SplitPath(fileName, 0, &dir, 0, 0);
            CWideString files = CWideString(fileName) + (const unsigned short*)L"*.bin";
            CFindFileHandle fileHandle;
            CFindFileData fileData;
            bool fileFound = FindFirstFile(files, fileHandle, fileData);
            while (fileFound) {
                CWideString name;
                {
                    CWideString entry(data.FileName);
                    SplitPath(entry, 0, 0, &name, 0);
                }
                names.push_back(name);
                fileFound = FindNextFile(files, fileHandle, fileData);
            }
        }
        found = FindNextFile(pattern, handle, data);
    }
}
}

#include <stdlib.h>
static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
class CWideStringData { public: unsigned short s[300]; };
static void wset(unsigned short* d, const unsigned short* s) { int i = 0; while (s[i]) { d[i] = s[i]; ++i; } d[i] = 0; }
static void wsetn(unsigned short* d, const char* s) { int i = 0; while (s[i]) { d[i] = (unsigned short)s[i]; ++i; } d[i] = 0; }
static int wlen(const unsigned short* s) { int i = 0; while (s[i]) ++i; return i; }
static void wcat(unsigned short* d, const unsigned short* s) { wset(d + wlen(d), s); }
static void narrow(char* d, const unsigned short* s) { int i = 0; while (s[i]) { d[i] = (char)s[i]; ++i; } d[i] = 0; }
static int g_live;
CWideString::CWideString() { PStringData = new CWideStringData; PStringData->s[0] = 0; ++g_live; }
CWideString::CWideString(const unsigned short* t) { PStringData = new CWideStringData; wset(PStringData->s, t); ++g_live; }
CWideString::CWideString(const CWideString& o) { PStringData = new CWideStringData; wset(PStringData->s, o.PStringData->s); ++g_live; }
CWideString::~CWideString() { delete PStringData; --g_live; }
const CWideString& CWideString::operator+=(const unsigned short* t) { wcat(PStringData->s, t); return *this; }
static int g_clive;
CCharString::CCharString(const char* text, long length)
{
    check(length == -1, "length -1");
    PStringData = new CCharStringData; PStringData->Length = (long)strlen(text);
    PStringData->Text = (char*)malloc(PStringData->Length + 1); strcpy(PStringData->Text, text); ++g_clive;
}
CCharString::~CCharString() { free(PStringData->Text); delete PStringData; --g_clive; }
CCharString CWideString::ConvertToCharString() const
{
    char buf[300]; narrow(buf, PStringData->s);
    return CCharString(buf);
}
CWideString __fastcall operator+(const CWideString& a, const unsigned short* b) { CWideString r(a); r += b; return r; }
static int g_dirSplits;
void __fastcall SplitPath(const CWideString& path, CWideString* drive, CWideString* directory, CWideString* name, CWideString* extension)
{
    check(drive == 0 && extension == 0, "split drive/ext unused");
    const unsigned short* s = path.PStringData->s; int n = wlen(s), cut = -1;
    for (int i = 0; i < n; ++i) if (s[i] == '\\') cut = i;
    if (directory) { ++g_dirSplits; check(name == 0 && s[n - 1] == '\\', "dir split of entry + backslash"); wset(directory->PStringData->s, s); directory->PStringData->s[cut + 1] = 0; }
    if (name) { check(directory == 0, "name split"); wset(name->PStringData->s, s + cut + 1); int m = wlen(name->PStringData->s);
        for (int i = m - 1; i >= 0; --i) if (name->PStringData->s[i] == '.') { name->PStringData->s[i] = 0; break; } }
}
static bool g_isFile; static int g_isFileCalls;
bool __fastcall IsPathFile(const CWideString& path)
{
    char buf[300]; narrow(buf, path.PStringData->s); check(strcmp(buf, "C:\\Saves\\*") == 0, "IsPathFile on the save pattern"); ++g_isFileCalls; return g_isFile;
}
static const char* g_root[] = { ".", "..", "ProfA", "ProfB", "x.txt", 0 };
static bool lookup(const CWideString& pattern, long index, CFindFileData& data)
{
    char buf[300]; narrow(buf, pattern.PStringData->s);
    if (strcmp(buf, "C:\\Saves\\*") == 0) { if (!g_root[index]) return false; wsetn(data.FileName, g_root[index]); return true; }
    if (strcmp(buf, "ProfA\\*.bin") == 0) { static const char* f[] = { "s1.bin", "s2.bin", 0 }; if (!f[index]) return false; wsetn(data.FileName, f[index]); return true; }
    check(strcmp(buf, "ProfB\\*.bin") == 0 || strcmp(buf, "x.txt\\*.bin") == 0, "inner pattern");
    return false;
}
bool __fastcall FindFirstFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data) { handle._body[0] = 0; return lookup(pattern, 0, data); }
bool __fastcall FindNextFile(const CWideString& pattern, CFindFileHandle& handle, CFindFileData& data) { return lookup(pattern, ++handle._body[0], data); }
CWideString __fastcall NResourceDirectoryNames::GetSaveDir() { CWideString r; wsetn(r.PStringData->s, "C:\\Saves\\"); return r; }
static int g_erase; static char g_pushed[8][32]; static int g_pushes;
template <> CWideString* std::vector<CWideString>::erase(CWideString* first, CWideString* last) { ++g_erase; check(first == _M_start && last == _M_finish, "erase all"); _M_finish = _M_start; return first; }
template <> void std::vector<CWideString>::push_back(const CWideString& x) { if (g_pushes < 8) narrow(g_pushed[g_pushes], x.PStringData->s); ++g_pushes; }
int main()
{
    static NUISystem::CFrontEndManager fem;
    static CWideString* store[4];
    std::vector<CWideString> names; names._M_start = (CWideString*)store; names._M_finish = (CWideString*)store + 2; names._M_end_of_storage = (CWideString*)store + 4;
    fem.GetProfileNames(names);
    check(g_erase == 1 && names._M_finish == names._M_start, "cleared");
    check(g_pushes == 2 && strcmp(g_pushed[0], "ProfA") == 0 && strcmp(g_pushed[1], "ProfA") == 0, "ProfA once per bin file");
    check(g_isFileCalls == 3 && g_dirSplits == 3, "dots skipped before IsPathFile; three entries searched");
    check(g_live == 0 && g_clive == 0, "all strings destroyed");
    g_isFile = true; g_pushes = 0; g_isFileCalls = 0; g_dirSplits = 0;
    fem.GetProfileNames(names);
    check(g_pushes == 0 && g_isFileCalls == 3 && g_dirSplits == 0 && g_live == 0, "file pattern skips every entry");
    if (!g_fail) std::printf("005955ab_TEST PASS\n");
    return g_fail;
}