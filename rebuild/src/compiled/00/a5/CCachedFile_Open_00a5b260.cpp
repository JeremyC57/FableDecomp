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
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);         // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    operator const char*() const;                       // 0x0099E4C0
    friend CCharString __fastcall operator+(const CCharString& a, const CCharString& b);   // 0x0099F570
};
class CWideString { public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
class CAFile {
public:
    static CWideString __fastcall ConvertPathToFullPath(const CWideString&);   // 0x00999110

public:
    static bool __fastcall PathExists(const CWideString&);   // 0x00999230
};   // 0x00999110
                     // 0x00999230
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
class CAFile;
struct CBase {                                          // the polymorphic root (vptr at +0)
    CBase();                                            // 0x0099A2F0
    virtual ~CBase();                                   // 0x0099A300
};
class CDiskFileWin32 : public CBase { public:                         // vtable 0x0122D06C
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
class CCachedFile { public:
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
    CWideString path = CAFile::ConvertPathToFullPath(filename);
    if (CAFile::PathExists(path)) {
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