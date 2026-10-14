// CFontBank::CFontBank() -- base CBaseClassNonCopyable(), own vtable, the Name string at +8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009A4140, length 26 bytes.
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (stores its vtable
// 0x0129A7C4 only), CCharString::CCharString() @ 0x0099E4B0. This class's vtable is 0x0129AB38
// (rtti-vtables.tsv: CFontBank).
// Layout from engine/CFontBank.h: FontHeight +0x04 (not initialised here), Name +0x08, PManager
// +0x0c (not initialised here). The base runs, the vtable is stored, the name is constructed
// out of line and `this` is returned.
#include "fable_string.h"

struct CFontManager;

struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                // 0x0099A2F0
};

struct CFontBank : CBaseClassNonCopyable {  // vtable 0x0129AB38
    unsigned long FontHeight;               // +0x04
    CCharString   Name;                     // +0x08
    CFontManager* PManager;                 // +0x0c
    virtual void v00();
    CFontBank();
};

CFontBank::CFontBank()
    : CBaseClassNonCopyable()
    , Name()
{
}
