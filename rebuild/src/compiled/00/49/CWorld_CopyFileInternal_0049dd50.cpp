// CWorld::CopyFileInternal(const CWideString&, const CWideString&) const -- copy one disk file to another through two CDiskFileWin32 objects
// Retail: Fable.exe @ 0x0049DD50, 124 bytes. PDB ?CopyFileInternal@CWorld@@IBEXABVCWideString@@0@Z (protected const)
// Callees: CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80 (twice: access 1 = read for the
// source, 4 for the destination, both with flags 2), the global __fastcall CopyFile(CAFile&, CAFile&) 0x00998E70,
// and the inlined virtual ~CDiskFileWin32: vtable 0x0122D06C store, CDiskFileWin32::Close() 0x0099A920 (direct, as in
// any destructor), ~CWideString 0x0099B510 on Pathname, ~CAFile 0x0099A300 (manifest label "CBase").
// CDiskFileWin32 (28 bytes, PDB = retail): CAFile vptr +0, AccessType +4, Position +8, Length +0xC, WinFileHandle +0x10,
// Opened +0x14, CWideString Pathname +0x18. No EH frame in retail (the TU builds without /EHsc).
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                              // 0x0099B510
};

enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

struct CAFile {
    virtual ~CAFile();                           // 0x0099A300
};

struct CDiskFileWin32 : CAFile {                 // vtable 0x0122D06C
    FileAccessType AccessType;                   // +0x04
    long           Position;                     // +0x08
    long           Length;                       // +0x0C
    void*          WinFileHandle;                // +0x10
    bool           Opened;                       // +0x14
    CWideString    Pathname;                     // +0x18
    CDiskFileWin32(const CWideString& name, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual void Close();                        // 0x0099A920
    virtual ~CDiskFileWin32() { Close(); }
};

void __fastcall CopyFile(CAFile& source, CAFile& destination);   // 0x00998E70

struct CWorld {
    void CopyFileInternal(const CWideString& source, const CWideString& destination) const;
};

void CWorld::CopyFileInternal(const CWideString& source, const CWideString& destination) const
{
    CDiskFileWin32 sourceFile(source, FILE_ACCESS_READ, 2);
    CDiskFileWin32 destinationFile(destination, FILE_ACCESS_WRITE, 2);
    CopyFile(sourceFile, destinationFile);
}