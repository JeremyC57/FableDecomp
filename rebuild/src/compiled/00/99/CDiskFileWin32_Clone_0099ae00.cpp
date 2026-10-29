// CDiskFileWin32::Clone() const -- a new CDiskFileWin32 copy-constructed from this one (the copy constructor inlined)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0099AE00, length 80 bytes.
// Callees: operator new @ 0x00BFEA1A (0x1c bytes), CBaseClassNonCopyable::CBaseClassNonCopyable() @ 0x0099A2F0, CWideString::CWideString() @ 0x0099AED0,
// CDiskFileWin32::Open @ 0x0099A6A0 with (other.Pathname, 1, 2). Vtable 0x0122D06C = CDiskFileWin32. Virtual (slot 1).
// Shape: `add esp,4` after operator new (the result is used), the placement null test, then the inline copy constructor
// (the out-of-line copy @ 0x0099ADC0 is the same body); a null allocation returns null.
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

class CAFile;

class CDiskFileWin32 : public CBaseClassNonCopyable { public:        // vtable 0x0122D06C
    virtual CAFile* Clone() const;
    long        AccessType;            // +0x04 FileAccessType
    long        Position;              // +0x08
    long        Length;                // +0x0c
    void*       WinFileHandle;         // +0x10
    bool        Opened;                // +0x14
    CWideString Pathname;              // +0x18
    CDiskFileWin32(const CDiskFileWin32& other) : Opened(false) { Open(other.Pathname, FILE_ACCESS_READ, 2); }
    void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
};

CAFile* CDiskFileWin32::Clone() const
{
    return (CAFile*)new CDiskFileWin32(*this);
}