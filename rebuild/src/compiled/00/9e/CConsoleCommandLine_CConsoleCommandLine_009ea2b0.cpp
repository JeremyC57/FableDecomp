// CConsoleCommandLine::CConsoleCommandLine() -- the CBase base (0x0099A2D0), this class's vtable (0x0129C400), the line CCharString at +4 built from the empty literal @ 0x0122D70E (CCharString(const char*, long) @ 0x0099EBF0), then the byte at +8 and the dword at +0xc cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EA2B0, length 48 bytes.
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString(const char*, long) @ 0x0099EBF0. Vtable 0x0129C400 names the class; the chunk label is propagated. Shape: `this` in esi, the vtable stored before the string constructor call, one zero register, `ret`.
namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2D0
}; } using N_0099a2d0::CBase;
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length);    // 0x0099EBF0
};

struct CConsoleCommandLine : CBase {               // vtable 0x0129C400
    virtual void v00();
    CCharString m_line;                            // +0x04
    bool        UserEntry;                               // +0x08
    long        CursorPosition;                               // +0x0c
    CConsoleCommandLine();
};

CConsoleCommandLine::CConsoleCommandLine()
    : CBase(), m_line("", -1), UserEntry(false), CursorPosition(0)
{
}