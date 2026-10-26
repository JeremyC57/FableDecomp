// CConsoleCommandLine::CConsoleCommandLine(const CConsoleCommandLine&) -- copy constructor of the console's command-line record: base CBase, then the current entry text, the user-entry flag and the cursor position
// Leaf: ??0CConsoleCommandLine
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ea2e0, 50 bytes (+ int3 padding) (+ int3 padding; `ret 4`, returns this).
// Callees: CBase::CBase 0x0099A2D0 (out of line; stores the CBase vtable 0x01231710), CCharString
// copy ctor 0x0099EC30.
// Identity: the catalog label `_Cons_val<allocator<CConsoleCommandLine>,..>` is the construct helper
// (0x009EE180, which inlines this same body); this one is the out-of-line thiscall copy constructor.
// Layout: Ego_r.pdb CConsoleCommandLine (0x10) = vptr +0 (vtable 0x0129C400, RTTI), CCharString
// CurrentEntry +4, bool UserEntry +8, long CursorPosition +0x0C -- the model of the landed
// CConsole::ResetCommandLineBuffers 0x009EA5A0.
// Shape: the base is DEFAULT-constructed through its out-of-line ctor (a compiler-generated copy
// constructor would inline the base's trivial copy instead), so the copy constructor is user-written
// with member initialisers; then the derived vtable store and the members in order.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
};

class CBaseClass { public:
    virtual void v00();                                         // vtable 0x01231710
    CBaseClass();                                               // 0x0099A2D0
};

class CConsoleCommandLine : public CBaseClass { public:                       // vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;                                   // +0x04
    bool        UserEntry;                                      // +0x08
    long        CursorPosition;                                 // +0x0c
    virtual void v00();
    CConsoleCommandLine(const CConsoleCommandLine& other);
};

CConsoleCommandLine::CConsoleCommandLine(const CConsoleCommandLine& other)
    : CBaseClass(), CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition)
{
}