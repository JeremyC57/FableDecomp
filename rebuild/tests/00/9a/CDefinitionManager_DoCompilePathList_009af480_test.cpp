// Behaviour test for CDefinitionManager::DoCompilePathList (0x009AF480): model and body as src/009af480.cpp (the same-TU
// _M_insert_overflow stand-in included), with the string, file-scan, disk-file, comment-strip, sort and compile callees given small
// in-memory bodies. Checks that every directory's matches are compiled in sorted lower-case order, comments are stripped before the
// compile, the current file name is the file's pathname, every file is closed, and no symbol table is touched.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#endif

class CWideString;
class CCharString { public:
    char m_Text[128];
    CCharString(const char* text, long length = -1) { (void)length; strcpy(m_Text, text); }
    CCharString(const CCharString& other) { strcpy(m_Text, other.m_Text); }
    ~CCharString() {}
    const CCharString& operator=(const CCharString& other) { strcpy(m_Text, other.m_Text); return *this; }
    CCharString ToLower() const
    {
        CCharString r(m_Text);
        for (char* p = r.m_Text; *p; ++p)
            if (*p >= 'A' && *p <= 'Z') *p = (char)(*p - 'A' + 'a');
        return r;
    }
    CWideString ToWideString() const;
};
class CWideString { public:
    char m_Text[128];
    CWideString() { m_Text[0] = 0; }
    CWideString(const char* text) { strcpy(m_Text, text); }
    CWideString(const CWideString& other) { strcpy(m_Text, other.m_Text); }
    ~CWideString() {}
    const CWideString& operator=(const CWideString& other) { strcpy(m_Text, other.m_Text); return *this; }
    CCharString ConvertToCharString() const { return CCharString(m_Text); }
};
CWideString CCharString::ToWideString() const { return CWideString(m_Text); }

namespace N_0099a300 { struct CBase {
    virtual void v00() {}
    ~CBase() {}
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;

// In-memory "disk": directory spec -> files with content.
struct SFakeFile { const char* Dir; const char* Name; const char* Text; };
static const SFakeFile g_Files[] = {
    { "defs\\*.def", "Zeta.DEF",  "zeta // trailing\nZ" },
    { "defs\\*.def", "alpha.def", "a/* block */b" },
    { "defs\\*.def", "Mid.def",   "m\r" },
    { "more\\*.def", "Only.def",  "only//x\ry" },
};
static const int g_FileCount = sizeof(g_Files) / sizeof(g_Files[0]);
static int g_OpenFiles = 0;

class CDiskFileWin32 : public CBase { public:
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags) { (void)pathname; (void)access; (void)flags; }
    virtual void Close() { if (Opened) { Opened = false; --g_OpenFiles; } }
    virtual void Read(void* data, long size, CASuspendableProcess* process)
    {
        (void)process;
        memcpy(data, Content, size);
        Position += size;
    }
    long        AccessType;
    long        Position;
    long        Length;
    void*       WinFileHandle;
    bool        Opened;
    CWideString Pathname;
    const char* Content;
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags)
        : AccessType(access), Position(0), Length(0), WinFileHandle(0), Opened(true), Pathname(pathname), Content("")
    {
        (void)flags;
        ++g_OpenFiles;
        for (int i = 0; i < g_FileCount; ++i) {
            CCharString lower = CCharString(g_Files[i].Name).ToLower();
            if (strcmp(lower.m_Text, pathname.m_Text) == 0) {
                Content = g_Files[i].Text;
                Length = (long)strlen(Content);
            }
        }
    }
    ~CDiskFileWin32() { Close(); }
    CWideString GetPathname() const { return Pathname; }
};
struct CFileScanState { long _body[2]; };
static bool ScanFrom(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    for (int i = (int)state._body[0]; i < g_FileCount; ++i)
        if (strcmp(g_Files[i].Dir, directory.m_Text) == 0) {
            fileName = CWideString(g_Files[i].Name);
            state._body[0] = i + 1;
            return true;
        }
    state._body[0] = g_FileCount;
    return false;
}
bool __fastcall ScanFirstFile(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    state._body[0] = 0;
    return ScanFrom(directory, state, fileName);
}
bool __fastcall ScanNextFile(const CWideString& directory, CFileScanState& state, CWideString& fileName)
{
    return ScanFrom(directory, state, fileName);
}
void __fastcall GFRemoveComments(char* text, const char* start, const char* end)
{
    size_t startLen = strlen(start), endLen = strlen(end);
    for (char* p = strstr(text, start); p; p = strstr(text, start)) {
        char* q = strstr(p + startLen, end);
        if (!q)
            break;
        char* after = q + endLen;
        memmove(p, after, strlen(after) + 1);
    }
}
struct __false_type { __false_type() {} };
void __fastcall SortWideStrings(CWideString* first, CWideString* last)
{
    for (CWideString* i = first; i != last; ++i)
        for (CWideString* j = i + 1; j != last; ++j)
            if (strcmp(j->m_Text, i->m_Text) < 0) {
                CWideString t(*i); *i = *j; *j = t;
            }
}
class CWideStringVector {
public:
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    ~CWideStringVector()
    {
        for (CWideString* p = _M_start; p != _M_finish; ++p)
            p->~CWideString();
        if (_M_start)
            free(_M_start);
    }
    CWideString* begin() { return _M_start; }
    CWideString* end() { return _M_finish; }
    const CWideString* begin() const { return _M_start; }
    const CWideString* end() const { return _M_finish; }
    unsigned int size() const { return _M_finish - _M_start; }
    CWideString& operator[](unsigned int n) { return *(_M_start + n); }
    void _M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type&, unsigned int fillLen, bool atEnd);
    void push_back(const CWideString& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) CWideString(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
};
void CWideStringVector::_M_insert_overflow(CWideString* pos, const CWideString& x, const __false_type&, unsigned int fillLen, bool atEnd)
{
    unsigned int oldSize = _M_finish - _M_start;
    unsigned int len = oldSize + (oldSize > fillLen ? oldSize : fillLen);
    CWideString* newStart = (CWideString*)malloc(len * sizeof(CWideString));
    CWideString* newFinish = newStart;
    CWideString* p;
    for (p = _M_start; p != pos; ++p, ++newFinish)
        new (newFinish) CWideString(*p);
    for (unsigned int n = 0; n < fillLen; ++n, ++newFinish)
        new (newFinish) CWideString(x);
    if (!atEnd)
        for (p = pos; p != _M_finish; ++p, ++newFinish)
            new (newFinish) CWideString(*p);
    for (p = _M_start; p != _M_finish; ++p)
        p->~CWideString();
    if (_M_start)
        free(_M_start);
    _M_start = newStart;
    _M_finish = newFinish;
    _M_end_of_storage = newStart + len;
}
static int g_Finalised = 0;
struct CSymbolTable { void Finalise() { ++g_Finalised; } };

static char g_Log[512];
class CDefinitionManager {
public:
    unsigned char _pad000[0x58];
    CSymbolTable* Symbols58;
    unsigned char _pad05c[4];
    CSymbolTable* Symbols60;
    CCharString   CurrentFileName;
    CDefinitionManager() : Symbols58(0), Symbols60(0), CurrentFileName("") {}
    void CompileString(char* text, long flags)
    {
        char entry[160];
        sprintf(entry, "%s:%s:%d|", CurrentFileName.m_Text, text, (int)flags);
        strcat(g_Log, entry);
    }
    void DoCompilePathList(const CWideStringVector& paths, long flags);
};

void CDefinitionManager::DoCompilePathList(const CWideStringVector& paths, long flags)
{
    for (const CWideString* path = paths.begin(); path != paths.end(); ++path) {
        CWideString fileName;
        CCharString functionName("DefinitionManager : DoCompilePathList");
        CFileScanState scan;
        CWideStringVector files;
        if (ScanFirstFile(*path, scan, fileName)) {
            do {
                files.push_back(fileName.ConvertToCharString().ToLower().ToWideString());
            } while (ScanNextFile(*path, scan, fileName));
        }
        long count = files.size();
        SortWideStrings(files.begin(), files.end());
        for (long i = 0; i < count; ++i) {
            CDiskFileWin32 file(files[i], FILE_ACCESS_READ, 2);
            char* buffer = new char[file.Length + 1];
            file.CDiskFileWin32::Read(buffer, file.Length, 0);
            buffer[file.Length] = 0;
            GFRemoveComments(buffer, "//", "\n");
            GFRemoveComments(buffer, "//", "\r");
            GFRemoveComments(buffer, "/*", "*/");
            CurrentFileName = file.GetPathname().ConvertToCharString();
            CompileString(buffer, flags);
            delete[] buffer;
        }
    }
}

int main()
{
    int fails = 0;
    CSymbolTable tableA, tableB;
    CDefinitionManager manager;
    manager.Symbols58 = &tableA;
    manager.Symbols60 = &tableB;

    CWideStringVector paths;
    paths.push_back(CWideString("defs\\*.def"));
    paths.push_back(CWideString("none\\*.def"));
    paths.push_back(CWideString("more\\*.def"));
    g_Log[0] = 0;
    manager.DoCompilePathList(paths, 7);
    const char* expected = "alpha.def:ab:7|mid.def:m\r:7|zeta.def:zeta Z:7|only.def:onlyy:7|";
    if (strcmp(g_Log, expected) != 0) { printf("log mismatch: [%s]\n", g_Log); ++fails; }
    if (g_Finalised != 0) { printf("finalised %d times\n", g_Finalised); ++fails; }
    if (g_OpenFiles != 0) { printf("%d files left open\n", g_OpenFiles); ++fails; }
    if (strcmp(manager.CurrentFileName.m_Text, "only.def") != 0) { printf("current file %s\n", manager.CurrentFileName.m_Text); ++fails; }

    // Empty path list: nothing compiled, no table finalised.
    CWideStringVector none;
    g_Log[0] = 0;
    g_Finalised = 0;
    manager.DoCompilePathList(none, 1);
    if (g_Log[0] != 0 || g_Finalised != 0) { printf("empty list: log [%s] finalised %d\n", g_Log, g_Finalised); ++fails; }

    if (fails == 0)
        printf("009af480_TEST PASS\n");
    return fails;
}