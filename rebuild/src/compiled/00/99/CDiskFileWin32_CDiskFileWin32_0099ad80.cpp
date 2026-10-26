// CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) -- CBase, own vtable, empty pathname, then Open
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099AD80, length 64 bytes.
// Callees: CBase::CBase() @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0 on Pathname (+0x18),
// CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) @ 0x0099A6A0 (virtual, called directly from the
// constructor). Vtable 0x0122D06C = CDiskFileWin32. PDB CDiskFileWin32 (0x1c bytes): bool Opened +0x14, CWideString Pathname +0x18.
// Shape: the three arguments are reloaded into eax/ecx/edx and pushed right to left; `this` returns in eax.
struct CBase {
    virtual void v00();
    CBase();                           // 0x0099A2F0
};

class CWideString { public:
    void* PStringData;                 // +0
    CWideString();                     // 0x0099AED0
};

enum FileAccessType { FILE_ACCESS_READ = 1 };

class CDiskFileWin32 : public CBase { public:        // vtable 0x0122D06C
    virtual void v00();
    long        AccessType;            // +0x04 FileAccessType
    long        Position;              // +0x08
    long        Length;                // +0x0c
    void*       WinFileHandle;         // +0x10
    bool        Opened;                // +0x14
    CWideString Pathname;              // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);
    void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
};

CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags)
{
    Open(pathname, access, flags);
}