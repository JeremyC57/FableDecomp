// CDiskFileWin32::CDiskFileWin32() -- CBase, own vtable, an empty pathname, not opened
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00415C30, length 30 bytes.
// Callees: CBase::CBase() @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0 on Pathname (+0x18; the manifest label
// "CCharString::CCharString" is propagated across the identical default constructors). Vtable 0x0122D06C = CDiskFileWin32.
// PDB CDiskFileWin32 (0x1c bytes): AccessType +4, Position +8, Length +0xc, WinFileHandle +0x10, bool Opened +0x14,
// CWideString Pathname +0x18. Shape: the Opened byte is cleared after the pathname constructor, so it is a body
// assignment (the copy constructor @ 0x0099ADC0 clears it from the initialiser list, before the string).
struct CBase {
    virtual void v00();
    CBase();                           // 0x0099A2F0
};

struct CWideString {
    void* PStringData;                 // +0
    CWideString();                     // 0x0099AED0
};

struct CDiskFileWin32 : CBase {        // vtable 0x0122D06C
    virtual void v00();
    long        AccessType;            // +0x04 FileAccessType
    long        Position;              // +0x08
    long        Length;                // +0x0c
    void*       WinFileHandle;         // +0x10
    bool        Opened;                // +0x14
    CWideString Pathname;              // +0x18
    CDiskFileWin32();
};

CDiskFileWin32::CDiskFileWin32()
{
    Opened = false;
}