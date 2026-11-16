// CTCGatherPointBase::CTCGatherPointBase(CThing* parent) -- CTCBase(parent), own vtable, empty Users vector, MaxOccupation = 1000
// Leaf: ??0CTCGatherPointBase@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCGatherPointBase@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E0AAB, length 42 bytes.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (vtable 0x0125BE0C = CTCBase; manifest label propagated).
// Vtable 0x012421E4 = CTCGatherPointBase (rtti-vtables.tsv): the chunk label "CTCDHeroSuit" is propagated
// from the byte-similar 0x004E079C. PDB CTCGatherPointBase (0x20 bytes): Users
// (std::vector<CIntelligentPointer<const CThing>>) +0x0c, MaxOccupation +0x1c; the retail vector is 12
// bytes, so MaxOccupation sits at +0x18 in the retail layout.
// Shape: /Os pass-through base argument (`push dword ptr [esp+8]`), `xor eax,eax`, the vtable store,
// the three vector zero stores (inline member constructor) and the 1000 immediate.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
struct CThingConst;

template<class T> struct CRetailVector {                    // retail Dinkumware vector: 3 pointers
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { }
};

struct CIntelligentPointerThing {                           // CIntelligentPointer<const CThing>, 8 bytes
    void* __vftable;
    void* PData;
};

class CTCGatherPointBase : public CTCBase { public:
    CRetailVector<CIntelligentPointerThing> Users;          // +0x0c
    long                                    MaxOccupation;  // +0x18
    CTCGatherPointBase(CThing& parent);
};

CTCGatherPointBase::CTCGatherPointBase(CThing& parent)
    : CTCBase(parent)
    , Users()
    , MaxOccupation(1000)
{
}