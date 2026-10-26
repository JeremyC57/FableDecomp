// CMemoryFile::Open(const CWideString& pathname, FileAccessType access, unsigned long flags) -- open a memory file: when the file exists on disk read it whole into the buffer, otherwise (read-only access) throw a "File does not exist" file exception; then record the name, rewind and mark it open
// Leaf: ?Open@CMemoryFile
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E94A0, length 324 bytes (padded to 336), `ret 0Ch`.
// Callees: ConvertPathToFullPath @ 0x00999110 and PathExists @ 0x00999230 (both `__fastcall`; the first returns a CWideString through
//   ecx with the path in edx), CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) @ 0x0099AD80 / Read @ 0x0099A960 /
//   the inlined ~CDiskFileWin32 (vtable 0x0122D06C, Close 0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300),
//   vector<unsigned char>::resize(size_type, const unsigned char&) @ 0x00486189, CWideString::ConvertToCharString @ 0x0099B2C0,
//   CCharString(const char*, long) @ 0x0099EBF0, `__fastcall` operator+(const CCharString&, const CCharString&) @ 0x0099F570, the
//   CCharString copy ctor 0x0099EC30 / operator const char* 0x0099E4C0 / dtor 0x0099EAE0, _CxxThrowException @ 0x00BFEB84 with the
//   CBBBFileException ThrowInfo 0x01369380, CWideString::operator= @ 0x0099B7D0.
// Layout: as the landed CMemoryFile::Read 0x009E9220 / MovePosition: AccessType +4, Position +8, Length +0xc, DataBuffer +0x10 (here
//   the 12-byte STLport vector<unsigned char>), Opened +0x1c, Pathname +0x20 -- engine/CMemoryFile.h is the PDB layout (16-byte
//   container), so a local model is used. String "File does not exist: " @ 0x0129C37C.
// Shape: the exception path follows the landed Read (no EH frame: the TU has no /EHsc, the throw leaves locals undestroyed).
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
    ~CWideString();                                     // 0x0099B510
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
    CCharString ConvertToCharString() const;            // 0x0099B2C0
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
bool __fastcall PathExists(const CWideString& path);                    // 0x00999230

class CExceptionBase {
public:
    CExceptionBase(const char*) { }
};
class CBBBFileException : public CExceptionBase {
public:
    enum EFileExceptionType { NullParameter = 1, NotOpenForWriting = 2, ReadPastEnd = 3, FileNotFound = 10, SeekOutOfRange = 11 };
    EFileExceptionType Type;
    CBBBFileException(CCharString text, EFileExceptionType type) : CExceptionBase(static_cast<const char*>(text)), Type(type) { }
};

class CASuspendableProcess;
namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                           // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

class CDiskFileWin32 : public CBase { public:                         // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                               // 0x0099A920 (slot 2)
    long        AccessType;                             // +0x04
    long        Position;                               // +0x08
    long        Length;                                 // +0x0c
    void*       WinFileHandle;                          // +0x10
    bool        Opened;                                 // +0x14
    CWideString Pathname;                               // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void Read(void* buffer, long size, CASuspendableProcess* process);   // 0x0099A960
};

template <class T> struct vector {                      // STLport vector, 12 bytes
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return _M_finish - _M_start; }
    void resize(unsigned long n, const T& value);       // 0x00486189
};

class CMemoryFile {
public:
    virtual ~CMemoryFile();
    long                  AccessType;                   // +0x04 FileAccessType
    long                  Position;                     // +0x08
    long                  Length;                       // +0x0c
    vector<unsigned char> DataBuffer;                   // +0x10
    bool                  Opened;                       // +0x1c
    CWideString           Pathname;                     // +0x20
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);
};

void CMemoryFile::Open(const CWideString& pathname, FileAccessType access, unsigned long flags)
{
    CWideString fullPath = ConvertPathToFullPath(pathname);
    if (PathExists(fullPath)) {
        CDiskFileWin32 file(pathname, access, flags);
        Length = file.Length;
        DataBuffer.resize(file.Length, 0);
        if (Length > 0)
            file.Read(DataBuffer.begin(), file.Length, 0);
        AccessType = access;
    } else {
        if (access == FILE_ACCESS_READ) {
            CCharString message = CCharString("File does not exist: ", -1) + fullPath.ConvertToCharString();
            throw CBBBFileException(message, CBBBFileException::FileNotFound);
        }
        AccessType = access;
    }
    Pathname = pathname;
    Position = 0;
    Length = DataBuffer.size();
    Opened = true;
}