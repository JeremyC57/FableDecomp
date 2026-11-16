// CTCDecapitation::CTCDecapitation(CThing&) -- base CTCBase(thing), own vtable, Decapitated = false, null def pointer
// Leaf: ??0CTCDecapitation@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCDecapitation@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DB207, length 32 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (vtable 0x0125BE0C = CTCBase; manifest label propagated).
// Vtable 0x01240534 = CTCDecapitation (rtti-vtables.tsv). PDB CTCDecapitation (0x14 bytes):
// Decapitated +0xc, PDef (CDefPointer<const CDecapitationDef>, one pointer) +0x10. The def pointer
// is nulled with `and dword ptr [esi+10h],0`, the size-optimised zero store, so the TU is /Os.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
struct CDecapitationDef_Calls;

template<class T> class CDefPointer { public:
    T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCDecapitation : public CTCBase { public:
    bool Decapitated;                                       // +0x0c
    CDefPointer<const CDecapitationDef_Calls> PDef;         // +0x10
    CTCDecapitation(CThing& parent);
};

CTCDecapitation::CTCDecapitation(CThing& parent)
    : CTCBase(parent)
    , Decapitated(false)
    , PDef()
{
}