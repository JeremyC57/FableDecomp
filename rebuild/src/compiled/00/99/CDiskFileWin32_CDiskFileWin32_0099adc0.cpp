// CDiskFileWin32::CDiskFileWin32(const CDiskFileWin32&) -- copy constructor: CBase, own vtable, not opened, empty pathname, then Open the other file's path read-only
// Leaf: ??0CDiskFileWin32@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099ADC0, length 64 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0 on Pathname (+0x18),
// CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) @ 0x0099A6A0 with (other.Pathname, 1, 2).
// Vtable 0x0122D06C = CDiskFileWin32. PDB CDiskFileWin32 (0x1c bytes): bool Opened +0x14, CWideString Pathname +0x18.
// Shape: Opened is cleared from the initialiser list (before the pathname constructor, after the vtable store).
class CBaseClassNonCopyable {
protected:
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                           // 0x0099A2F0
};

class CWideString { public:
    void* PStringData;                 // +0
    CWideString();                     // 0x0099AED0
};

enum FileAccessType { FILE_ACCESS_READ = 1 };

class CDiskFileWin32 : public CBaseClassNonCopyable { public:        // vtable 0x0122D06C
    long        AccessType;            // +0x04 FileAccessType
    long        Position;              // +0x08
    long        Length;                // +0x0c
    void*       WinFileHandle;         // +0x10
    bool        Opened;                // +0x14
    CWideString Pathname;              // +0x18
    CDiskFileWin32(const CDiskFileWin32& other);
    void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
};

CDiskFileWin32::CDiskFileWin32(const CDiskFileWin32& other)
    : Opened(false)
{
    Open(other.Pathname, FILE_ACCESS_READ, 2);
}