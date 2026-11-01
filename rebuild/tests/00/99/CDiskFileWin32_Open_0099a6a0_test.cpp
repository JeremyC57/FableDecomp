// Behaviour test for CDiskFileWin32::Open (0x0099A6A0): the pathname is kept, an empty full path throws exception 1,
// each access type maps to its CreateFile mode / share / disposition / stored AccessType, flag bit 2 asks for
// FILE_FLAG_SEQUENTIAL_SCAN, a failed open throws exception 2 with the access type in the text, and a good open
// records the size, rewinds and marks the file opened.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
class CCharString { public:
    char* text;
    CCharString() : text(strdup("")) {}
    CCharString(const char* t) : text(strdup(t)) {}
    CCharString(const CCharString& o) : text(strdup(o.text)) {}
    ~CCharString() { free(text); }
    CCharString& operator=(const CCharString& o) { char* t = strdup(o.text); free(text); text = t; return *this; }
};
static CCharString cat(const char* a, const char* b) { char* t = (char*)malloc(strlen(a) + strlen(b) + 1); strcpy(t, a); strcat(t, b); CCharString r(t); free(t); return r; }
CCharString operator+(const char* l, const CCharString& r) { return cat(l, r.text); }
CCharString operator+(const CCharString& l, const char* r) { return cat(l.text, r); }
CCharString operator+(const CCharString& l, const CCharString& r) { return cat(l.text, r.text); }
CCharString GFIntToCharString(long v) { char b[16]; sprintf(b, "%ld", v); return CCharString(b); }
static const char* g_lastError = "no error";
CCharString GFGetLastWindowsError() { return CCharString(g_lastError); }
class CWideString { public:
    CCharString narrow;
    CWideString() {}
    CWideString(const char* t) : narrow(t) {}
    long GetLength() const { return (long)strlen(narrow.text); }
    CCharString ToCharString() const { return narrow; }
};
static const char* g_cwd = "C:\\Fable\\";
class CAFile { public:
    static int validated, dirsCreated;
    static bool ConvertPathToFullPath(const CWideString& path, CWideString& full) { if (path.narrow.text[0] == 0) { full = CWideString(""); return true; } full = CWideString(cat(g_cwd, path.narrow.text).text); return true; }
    static void ValidatePathnameIsOnWriteableDrive(const CWideString&) { ++validated; }
    static void CreateDirectoryForFile(const CWideString&) { ++dirsCreated; }
};
int CAFile::validated = 0, CAFile::dirsCreated = 0;
struct CBBBFileException { CCharString message; long type; CBBBFileException(const CCharString& m, long t) : message(m), type(t) {} };
// a tiny CreateFile model: records the last request, fails when the name holds "missing"
struct SOpen { char name[128]; unsigned long access, share, disposition, attributes; } g_last;
static void* g_handle = (void*)0x1234;
static long g_size = 777;
static void* CreateFileW(const char* name, unsigned long access, unsigned long share, void*, unsigned long disposition, unsigned long attributes, void*)
{
    strcpy(g_last.name, name); g_last.access = access; g_last.share = share; g_last.disposition = disposition; g_last.attributes = attributes;
    if (strstr(name, "missing")) { g_lastError = "The system cannot find the file specified."; return (void*)-1; }
    return g_handle;
}
static int g_pointerMoves = 0;
static unsigned long GetFileSize(void* h, unsigned long*) { return h == g_handle ? g_size : 0; }
static unsigned long SetFilePointer(void* h, long d, long* hi, unsigned long method) { (void)h; (void)hi; if (d == 0 && method <= 1) ++g_pointerMoves; return 0; }
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_READ_WRITE = 2, FILE_ACCESS_READ_WRITE_OPEN_ALWAYS = 3, FILE_ACCESS_CREATE = 4 };
class CDiskFileWin32 {
public:
    long AccessType; long Position; long Length; void* WinFileHandle; bool Opened; CWideString Pathname;
    CDiskFileWin32() : AccessType(0), Position(-1), Length(-1), WinFileHandle(0), Opened(false) {}
    void Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags);
};
void CDiskFileWin32::Open(const CWideString& pathname, FileAccessType accessType, unsigned long flags)
{
    CWideString fullPath;
    unsigned long attributes = 0;
    Pathname = pathname;
    WinFileHandle = 0;
    if (flags & 2)
        attributes = 0x08000000;
    CAFile::ConvertPathToFullPath(pathname, fullPath);
    if (fullPath.GetLength() <= 0) {
        CCharString message = "Bad filename " + fullPath.ToCharString();
        throw CBBBFileException(message, 1);
    }
    void* handle;
    switch (accessType) {
    case FILE_ACCESS_READ:
        handle = CreateFileW(fullPath.narrow.text, 0x80000000, 1, 0, 3, attributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        handle = CreateFileW(fullPath.narrow.text, 0xC0000000, 1, 0, 3, attributes, 0);
        AccessType = accessType;
        break;
    case FILE_ACCESS_READ_WRITE_OPEN_ALWAYS:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        handle = CreateFileW(fullPath.narrow.text, 0xC0000000, 0, 0, 4, attributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    case FILE_ACCESS_CREATE:
        CAFile::ValidatePathnameIsOnWriteableDrive(fullPath);
        CAFile::CreateDirectoryForFile(fullPath);
        handle = CreateFileW(fullPath.narrow.text, 0xC0000000, 0, 0, 2, attributes, 0);
        AccessType = FILE_ACCESS_READ_WRITE;
        break;
    default:
        handle = 0;
        break;
    }
    if (handle == (void*)-1) {
        CCharString error = GFGetLastWindowsError();
        CCharString message = "Cannot open file '" + fullPath.ToCharString() + "' (access type " + GFIntToCharString(accessType) + ") - " + error;
        throw CBBBFileException(message, 2);
    }
    Length = GetFileSize(handle, 0);
    SetFilePointer(handle, 0, 0, 0);
    WinFileHandle = handle;
    Position = 0;
    Opened = true;
    SetFilePointer(handle, 0, 0, 1);
}
int main()
{
    CDiskFileWin32 f;
    f.Open(CWideString("data.bin"), FILE_ACCESS_READ, 2);
    if (!f.Opened || f.WinFileHandle != g_handle || f.Length != 777 || f.Position != 0 || f.AccessType != 1 || g_pointerMoves != 2 ||
        strcmp(f.Pathname.narrow.text, "data.bin") || strcmp(g_last.name, "C:\\Fable\\data.bin") || g_last.access != 0x80000000 || g_last.share != 1 ||
        g_last.disposition != 3 || g_last.attributes != 0x08000000) { printf("0099a6a0_TEST FAIL read\n"); return 1; }
    f.Open(CWideString("save.dat"), FILE_ACCESS_READ_WRITE, 0);
    if (f.AccessType != 2 || CAFile::validated != 1 || g_last.access != 0xC0000000 || g_last.share != 1 || g_last.disposition != 3 || g_last.attributes != 0) { printf("0099a6a0_TEST FAIL rw\n"); return 1; }
    f.Open(CWideString("log.txt"), FILE_ACCESS_READ_WRITE_OPEN_ALWAYS, 0);
    if (f.AccessType != 2 || CAFile::validated != 2 || g_last.share != 0 || g_last.disposition != 4) { printf("0099a6a0_TEST FAIL open-always\n"); return 1; }
    f.Open(CWideString("new\\file.bin"), FILE_ACCESS_CREATE, 0);
    if (f.AccessType != 2 || CAFile::validated != 3 || CAFile::dirsCreated != 1 || g_last.share != 0 || g_last.disposition != 2) { printf("0099a6a0_TEST FAIL create\n"); return 1; }
    int caught = 0;
    try { f.Open(CWideString(""), FILE_ACCESS_READ, 0); } catch (CBBBFileException& e) { if (e.type == 1 && strcmp(e.message.text, "Bad filename ") == 0) caught = 1; }
    if (caught != 1) { printf("0099a6a0_TEST FAIL bad filename\n"); return 1; }
    try { f.Open(CWideString("missing.bin"), FILE_ACCESS_READ_WRITE, 0); } catch (CBBBFileException& e) {
        if (e.type == 2 && strcmp(e.message.text, "Cannot open file 'C:\\Fable\\missing.bin' (access type 2) - The system cannot find the file specified.") == 0) caught = 2; }
    if (caught != 2 || f.WinFileHandle != 0) { printf("0099a6a0_TEST FAIL cannot open\n"); return 1; }
    printf("0099a6a0_TEST PASS\n");
    return 0;
}