// CMemoryFile::CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags) -- the CBaseClassNonCopyable base (0x0099A2F0), this class's vtable (0x0122E0FC), the byte vector at +0x10 cleared, the pathname at +0x20 default-constructed (0x0099AED0), then Open (0x009E94A0, virtual, called directly from the constructor)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E97B0, length 64 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0, CWideString::CWideString @ 0x0099AED0, CMemoryFile::Open @ 0x009E94A0. Shape: `this` in esi, the three arguments reloaded and pushed right to left, `ret 0Ch`.
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                       // 0x0099A2F0
};
struct CWideString {
    void* PStringData;
    CWideString();                                 // 0x0099AED0
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
struct CRetailVector_009e97b0 {
    unsigned char* m_first;
    unsigned char* m_last;
    unsigned char* m_end;
    CRetailVector_009e97b0() : m_first(0), m_last(0), m_end(0) { }
};

struct CMemoryFile : CBaseClassNonCopyable {      // vtable 0x0122E0FC
    virtual void v00();
    long                    AccessType;            // +0x04
    long                    m_8;
    long                    Length;                // +0x0c
    CRetailVector_009e97b0  m_buffer;              // +0x10
    bool                    Opened;                // +0x1c
    CWideString             Pathname;              // +0x20
    CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags);
    void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x009E94A0
};

CMemoryFile::CMemoryFile(const CWideString& pathname, FileAccessType access, unsigned long flags)
    : CBaseClassNonCopyable(), m_buffer(), Pathname()
{
    Open(pathname, access, flags);
}
