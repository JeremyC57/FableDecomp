// CTCBed::CTCBed(CThing* parent) -- CTCBase(parent), own vtable, Empty/Animating = true, GettingIn/GettingOut = false, BedSleepAnim = -1, null def
// Leaf: ??0CTCBed@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCBed@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00746820, length 51 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800.
// Vtable 0x01268964 = CTCBed (rtti-vtables.tsv): the chunk label "CTCEditor" is propagated.
// PDB CTCBed (0x18 bytes): Empty +0x0c, GettingIn +0x0d, GettingOut +0x0e, Animating +0x0f,
// BedSleepAnim +0x10, PDef (CDefPointer<const CBedDef>) +0x14.
// Shape: speed TU; the false/true values live in eax/cl, and the compiler interleaves the init-list
// stores with the derived vtable store (the two zero bytes before it, the rest after).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CBedDef;

template<class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCBed : public CTCBase { public:
    bool                       Empty;                       // +0x0c
    bool                       GettingIn;                   // +0x0d
    bool                       GettingOut;                  // +0x0e
    bool                       Animating;                   // +0x0f
    long                       BedSleepAnim;                // +0x10
    CDefPointer<const CBedDef> PDef;                        // +0x14
    CTCBed(CThing& parent);
};

CTCBed::CTCBed(CThing& parent)
    : CTCBase(parent)
    , Empty(true)
    , GettingIn(false)
    , GettingOut(false)
    , Animating(true)
    , BedSleepAnim(-1)
    , PDef()
{
}