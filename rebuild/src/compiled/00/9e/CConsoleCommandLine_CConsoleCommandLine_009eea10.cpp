// CConsoleCommandLine::CConsoleCommandLine(const CCharString& entry) -- a command line seeded with
// a string: CBase(), own vtable, the entry copied, UserEntry cleared and the cursor at the end.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009EEA10, length 61 bytes (+ int3 padding).
// Callees: CBase::CBase @ 0x0099A2D0, CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Vtable 0x0129C400 = CConsoleCommandLine (rtti-vtables.tsv).
// Layout from the PDB (16 bytes): CCharString CurrentEntry +0x04, bool UserEntry +0x08,
// long CursorPosition +0x0c. The cursor is `entry.GetLength()` inlined: a null string block
// reads as length 0, otherwise the block's StringLength at +4 (CBasicString<char> layout as in
// the landed CCharString::operator= @ 0x0099EFE0), which is why the body forks on the block
// pointer with two copies of the epilogue.
struct CCharStringData {
    char*         PData;            // +0x00
    unsigned long StringLength;     // +0x04
};

struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);     // 0x0099EC30
    long GetLength() const
    {
        if (PStringData == 0)
            return 0;
        return PStringData->StringLength;
    }
};

namespace N_0099a2d0 { struct CBase {
    virtual void v00();
    CBase();                                   // 0x0099A2D0
}; } using N_0099a2d0::CBase;

struct CConsoleCommandLine : CBase {           // vtable 0x0129C400
    virtual void v00();
    CCharString CurrentEntry;       // +0x04
    bool        UserEntry;          // +0x08
    long        CursorPosition;     // +0x0c
    CConsoleCommandLine(const CCharString& entry);
};

CConsoleCommandLine::CConsoleCommandLine(const CCharString& entry)
    : CBase()
    , CurrentEntry(entry)
    , UserEntry(false)
    , CursorPosition(entry.GetLength())
{
}