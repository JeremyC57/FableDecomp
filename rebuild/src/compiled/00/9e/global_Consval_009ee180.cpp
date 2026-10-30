// _global::_Cons_val(CConsoleCommandLine*, const CConsoleCommandLine&) -- STL construct helper: placement-copy-construct a console command-line record into raw container storage
// Flags: /Gr
// Leaf: _Cons_val
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ee180, 48 bytes.
// Callees: CBase::CBase 0x0099A2D0, CCharString copy ctor 0x0099EC30.
// Shape: link-time __fastcall (/Gr STL TU): destination in ecx (esi), source in edx (edi); the
// placement new's null test guards the inlined user copy constructor of CConsoleCommandLine (the
// out-of-line copy is 0x009EA2E0): base default ctor, vtable 0x0129C400, string copy, the bool as a
// byte, the cursor dword.  Layout as the landed CConsole::ResetCommandLineBuffers 0x009EA5A0.
inline void* operator new(unsigned int, void* where) { return where; }

namespace FableCons_009ee180 {

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                      // 0x0099EC30
};

}   // the retail root is file scope
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
namespace FableCons_009ee180 {

struct CConsoleCommandLine : CBaseClass {                       // vtable 0x0129C400 (16 bytes)
    CCharString CurrentEntry;                                   // +0x04
    bool        UserEntry;                                      // +0x08
    long        CursorPosition;                                 // +0x0c
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CBaseClass(), CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition)
    {
    }
};

void __fastcall _Cons_val(CConsoleCommandLine* ptr, const CConsoleCommandLine& value)
{
    new ((void*)ptr) CConsoleCommandLine(value);
}

}