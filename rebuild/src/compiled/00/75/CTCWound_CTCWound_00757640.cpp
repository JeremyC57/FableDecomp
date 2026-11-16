// CTCWound::CTCWound(CThing* parent) -- CTCBase(parent), own vtable, LastFrameChanged = 0 and the Wounds list (list<CTCWound::CWound>, 0x4c-byte nodes) default-constructed
// Leaf: ??0CTCWound@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCWound@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe @ 0x00757640, 51 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x012698B4, is CTCWound in
// rtti-vtables.tsv; PDB CTCWound: LastFrameChanged +0xc, Wounds +0x10.
// Shape: LastFrameChanged and the list head share the zero register (`xor eax,eax`) around the `push 4Ch`; the
// sentinel is linked to itself before it is stored. CWound is 0x44 bytes (node 0x4c).
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
extern "C" void* __cdecl malloc(unsigned int size);  // 0x00BFEA0E

// The retail (STLport) std::list: one sentinel-node pointer, nulled by the alloc-proxy initialiser, then a node is
// allocated (allocate(1) -> malloc) and linked to itself before it is stored (see the landed 0x00835370 shape).
template <class T> struct CRetailList {
    struct node { node* next; node* prev; T value; };
    node* head;                                    // +0x00
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct CWound { unsigned char _body[0x44]; };      // CTCWound::CWound (retail 0x44 bytes)

class CTCWound : public CTCBase { public:                  // vtable 0x012698B4
    unsigned long        LastFrameChanged;         // +0x0c
    CRetailList<CWound>  Wounds;                   // +0x10
    CTCWound(CThing& parent);
};

CTCWound::CTCWound(CThing& parent)
    : CTCBase(parent)
    , LastFrameChanged(0)
{
}