// CInputManager::StreamInputsFromFile(const CVersion& version, const CWideString& fileName) -- open the file read-only (access 1, flags 2) in a stack CDiskFileWin32, load every event package from it, raise Loading (+0x38); the file closes with the scope
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F5E10, length 87 bytes.
// Callees: CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) @ 0x0099AD80,
// CInputManager::LoadAllEventPackages(const CVersion&, CAFile&) @ 0x009F5D70, CDiskFileWin32::Close @ 0x0099A920 (devirtualised in the
// inlined destructor after the vtable 0x0122D06C store), CWideString::~CWideString @ 0x0099B510 (Pathname at +0x18 of the file),
// CBase::~CBase @ 0x0099A300 (the root base).
// Shape: a 0x1c-byte frame for the file; the file name is read before the constructor pushes, the version after it; the Loading
// byte is stored between the load call and the inlined `~CDiskFileWin32() { Close(); }` sequence.
class CWideString { public:
    void* PStringData;                              // +0x00
    ~CWideString();                                 // 0x0099B510
};
class CVersion;
namespace N_0099a300 { struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
}; } using N_0099a300::CBase;
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };

class CDiskFileWin32 : public CBase { public:                     // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0 (slot 1)
    virtual void Close();                           // 0x0099A920 (slot 2)
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};

class CInputManager { public:
    virtual void v00();
    unsigned char _pad04[0x34];                     // +0x04 vectors, counters, devices
    bool          Loading;                          // +0x38
    bool          Saving;                           // +0x39
    void LoadAllEventPackages(const CVersion& version, CDiskFileWin32& file);   // 0x009F5D70
    void StreamInputsFromFile(const CVersion& version, const CWideString& fileName);
};

void CInputManager::StreamInputsFromFile(const CVersion& version, const CWideString& fileName)
{
    CDiskFileWin32 file(fileName, FILE_ACCESS_READ, 2);
    LoadAllEventPackages(version, file);
    Loading = true;
}