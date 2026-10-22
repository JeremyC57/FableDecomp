// Behaviour test for 00a5b260: src/00a5b260.cpp copied verbatim, the path helpers, disk file and string classes stubbed so
// both the open path and the not-found throw are observable. CCachedFile::Open(const CWideString&, FileAccessType, ulong)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CCachedFile::Open(const CWideString& filename, FileAccessType access, unsigned long flags) -- open the cached file: when the full path exists, open it on a local CDiskFileWin32, reset the cache state (BufferValid false, Pathname, Position 0, Opened) and share a clone of the disk file; otherwise throw CBBBFileException("File does not exist: " + path, 10)
// Leaf: ?Open@CCachedFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5B260, length 265 bytes (the int3 VC7.1 emits after the final noreturn call included) (`ret 0Ch`).
// Callees: ConvertPathToFullPath 0x00999110 (__fastcall, result homed in the dead `filename` slot), PathExists 0x00999230,
//          CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80, CWideString::operator= 0x0099B7D0,
//          CDiskFileWin32::Clone 0x0099AE00, CCountedPointer<CAFile>::Reset 0x0050F550, the inline ~CDiskFileWin32 (vtable
//          0x0122D06C, Close 0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300); error path: CWideString::
//          ConvertToCharString 0x0099B2C0, CCharString(const char*, long) 0x0099EBF0, __fastcall operator+(const
//          CCharString&, const CCharString&) 0x0099F570, copy ctor 0x0099EC30, operator const char* 0x0099E4C0, ~CCharString
//          0x0099EAE0, _CxxThrowException 0x00BFEB84 (CBBBFileException ThrowInfo 0x01369380); literal 0x0129C37C.
// Model: the landed CCachedFile::OpenFromFile (0x00A5B210) layout, the landed CMemoryFile::MovePosition (0x009E9140)
//          exception model, the CDiskFileWin32 of CBankFile::PrepareDataForInclusion (0x009CC9F0, this chunk).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    friend CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
};
struct CWideString {
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
bool __fastcall PathExists(const CWideString& path);                     // 0x00999230
class CExceptionBase {
public:
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { FileNotFound = 10 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CAFile;
struct CBase {                                          // the polymorphic root (vptr at +0)
    CBase();                                            // 0x0099A2F0
    virtual ~CBase();                                   // 0x0099A300
};
struct CDiskFileWin32 : CBase {                         // vtable 0x0122D06C
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Close();                               // 0x0099A920
    virtual CAFile* Clone() const;                      // 0x0099AE00
};
struct CCountedPointer_CAFile {                         // {object, count block}
    CAFile* m_pObject;
    void*   m_pCount;
    void Reset(CAFile* file);                           // 0x0050F550
};
struct CCachedFile {
    void*                  _vptr;                       // +0x00
    long                   Position;                    // +0x04
    long                   BufferStartPosition;         // +0x08
    unsigned char          DataBuffer[0x0c];            // +0x0c CArray<unsigned char>
    CWideString            Pathname;                    // +0x18
    CCountedPointer_CAFile PDiskFile;                   // +0x1c
    bool                   Opened;                      // +0x24
    bool                   BufferValid;                 // +0x25
    void Open(const CWideString& filename, FileAccessType access, unsigned long flags);
};

void CCachedFile::Open(const CWideString& filename, FileAccessType access, unsigned long flags)
{
    CWideString path = ConvertPathToFullPath(filename);
    if (PathExists(path)) {
        CDiskFileWin32 file(filename, access, flags);
        BufferValid = false;
        Pathname = filename;
        Position = 0;
        Opened = true;
        PDiskFile.Reset(file.Clone());
        return;
    }
    CCharString message = CCharString("File does not exist: ", -1) + path.ConvertToCharString();
    throw CBBBFileException(message, CBBBFileException::FileNotFound);
}

// ---- stubs ----
static char g_buf[256];
CCharString::CCharString(const char* text, long) { PStringData = (void*)text; }
CCharString::CCharString(const CCharString& o) { PStringData = o.PStringData; }
CCharString::~CCharString() {}
CCharString::operator const char*() const { return (const char*)PStringData; }
CCharString __fastcall operator+(const CCharString& a, const CCharString& b)
{ strcpy(g_buf, (const char*)a.PStringData); strcat(g_buf, (const char*)b.PStringData); return CCharString(g_buf, -1); }
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
CWideString& CWideString::operator=(const CWideString& o) { PStringData = o.PStringData; return *this; }
CCharString CWideString::ConvertToCharString() const { return CCharString("C:/full/path", -1); }
static const void* g_fullOf = 0; static bool g_exists = true;
CWideString __fastcall ConvertPathToFullPath(const CWideString& path) { g_fullOf = path.PStringData; CWideString r; r.PStringData = (void*)"FULL"; return r; }
bool __fastcall PathExists(const CWideString& path) { return g_exists && path.PStringData != 0; }
CBase::CBase() {}
CBase::~CBase() {}
static int g_closes = 0; static long g_access = 0; static unsigned long g_flags = 0; static const void* g_openedName = 0;
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags)
{ g_openedName = pathname.PStringData; g_access = access; g_flags = flags; Opened = true; }
void CDiskFileWin32::Close() { ++g_closes; }
static int g_clones = 0; static CAFile* g_clone = (CAFile*)0x5150;
CAFile* CDiskFileWin32::Clone() const { ++g_clones; return g_clone; }
static CAFile* g_reset = 0;
void CCountedPointer_CAFile::Reset(CAFile* file) { g_reset = file; m_pObject = file; }
int main()
{
    CCachedFile cf; memset(&cf, 0, sizeof(cf)); cf.Position = 99; cf.BufferValid = true;
    CWideString name; name.PStringData = (void*)"data.big";
    cf.Open(name, FILE_ACCESS_READ, 7);
    CHECK(g_fullOf == name.PStringData); CHECK(g_openedName == name.PStringData && g_access == 1 && g_flags == 7);
    CHECK(cf.Opened == true && cf.BufferValid == false && cf.Position == 0 && cf.Pathname.PStringData == name.PStringData);
    CHECK(g_clones == 1 && g_reset == g_clone && cf.PDiskFile.m_pObject == g_clone); CHECK(g_closes == 1);
    // missing file: throws a FileNotFound exception naming the full path
    g_exists = false; bool caught = false;
    try { cf.Open(name, FILE_ACCESS_READ, 0); }
    catch (CBBBFileException& e) { caught = true; CHECK(e.Type == CBBBFileException::FileNotFound); CHECK(strcmp(g_buf, "File does not exist: C:/full/path") == 0); }
    CHECK(caught); CHECK(g_clones == 1);
    if (fails == 0) printf("00a5b260_TEST PASS\n"); else printf("00a5b260_TEST FAIL\n");
    return fails;
}