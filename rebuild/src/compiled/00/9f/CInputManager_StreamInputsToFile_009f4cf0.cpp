// CInputManager::StreamInputsToFile(const CVersion& version, const CWideString& fileName) -- open the package file for writing, write the 12-byte version header, rewind the package index and raise Saving
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F4CF0, length 56 bytes.
// Callees: CDiskFileWin32::Open @ 0x0099A6A0 (vtable 0x0122D06C slot 1), CDiskFileWin32::Write @ 0x0099AA90 (slot 4).
// Same body as OpenPackageFileForSaving (0x009F4AD0) plus the Saving flag (+0x39) store at the end; PackageFile is the
// embedded CDiskFileWin32 at +0x60, CurrentFilePackageIndex at +0x58 (PDB CInputManager with 12-byte vectors).
struct CWideString;
struct CVersion;
struct CASuspendableProcess;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

struct CDiskFileWin32 {                             // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0 (slot 1)
    virtual void Close();                           // 0x0099A920 (slot 2)
    virtual void v03();
    virtual void Write(const void* data, long length, CASuspendableProcess* process);   // 0x0099AA90 (slot 4)
    unsigned char _body[0x18];
};

struct CInputManager {
    virtual void v00();
    unsigned char  _pad04[0x34];                    // +0x04 vectors, counters, devices
    bool           Loading;                         // +0x38
    bool           Saving;                          // +0x39
    unsigned char  _pad3a[0x1e];                    // +0x3a checksum, loaded packages
    long           CurrentFilePackageIndex;         // +0x58
    long           TimeStamp;                       // +0x5c
    CDiskFileWin32 PackageFile;                     // +0x60
    void StreamInputsToFile(const CVersion& version, const CWideString& fileName);
};

void CInputManager::StreamInputsToFile(const CVersion& version, const CWideString& fileName)
{
    PackageFile.Open(fileName, FILE_ACCESS_WRITE, 2);
    PackageFile.Write(&version, 12, 0);
    CurrentFilePackageIndex = 0;
    Saving = true;
}