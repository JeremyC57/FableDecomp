// CInputManager::OpenPackageFileForSaving(const CVersion& version, const CWideString& fileName) -- open the package file for writing (access 4, flags 2), write the 12-byte version header, rewind the package index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4AD0, length 52 bytes (+ int3 padding).
// Callees: CDiskFileWin32::Open @ 0x0099A6A0 (vtable 0x0122D06C slot 1, virtual), CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) @ 0x0099AA90 (slot 4).
// PackageFile is the embedded CDiskFileWin32 at +0x60 (PDB CInputManager with 12-byte vectors), CurrentFilePackageIndex at +0x58.
// Shape: the file object's address is held in esi for both virtual calls; the file name is loaded before the Open pushes, the
// version address before the Write pushes (right to left), the index cleared last, `ret 8`.
struct CWideString;
struct CVersion;                                    // 12 bytes written as the file header
struct CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

struct CDiskFileWin32 {                             // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0 (slot 1)
    virtual void Close();                           // 0x0099A920 (slot 2)
    virtual void v03();
    virtual void Write(const void* data, long length, CASuspendableProcess* process);   // 0x0099AA90 (slot 4)
    unsigned char _body[0x18];                      // +0x04 AccessType, Position, Length, WinFileHandle, Opened, Pathname
};

struct CInputManager {
    virtual void v00();
    unsigned char  _pad04[0x54];                    // +0x04 vectors, counters, devices, checksum
    long           CurrentFilePackageIndex;         // +0x58
    long           TimeStamp;                       // +0x5c
    CDiskFileWin32 PackageFile;                     // +0x60
    void OpenPackageFileForSaving(const CVersion& version, const CWideString& fileName);
};

void CInputManager::OpenPackageFileForSaving(const CVersion& version, const CWideString& fileName)
{
    PackageFile.Open(fileName, FILE_ACCESS_WRITE, 2);
    PackageFile.Write(&version, 12, 0);
    CurrentFilePackageIndex = 0;
}
