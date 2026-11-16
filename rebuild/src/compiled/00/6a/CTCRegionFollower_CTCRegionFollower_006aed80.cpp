// CTCRegionFollower::CTCRegionFollower(CThing* parent) -- CTCBase(parent), own vtable, the Followers list (list<CIntelligentPointer<CThing>>) default-constructed: a self-linked 0x10-byte sentinel node
// Leaf: ??0CTCRegionFollower@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCRegionFollower@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe @ 0x006AED80, 50 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x0125D234, is
// CTCRegionFollower in rtti-vtables.tsv; PDB CTCRegionFollower holds only Followers at +0xc.
// Shape: the inline list constructor runs after the derived vtable store (member init): the head is nulled, the
// sentinel allocated and linked to itself, then stored. The retail list is the 4-byte STLport one (PDB: 12 bytes).
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

struct CIntelligentPointer_CThing {                 // CIntelligentPointer<CThing>, 8 bytes (vptr + PData)
    void* __vftable;
    void* PData;
};

class CTCRegionFollower : public CTCBase { public:         // vtable 0x0125D234
    CRetailList<CIntelligentPointer_CThing> Followers;   // +0x0c
    CTCRegionFollower(CThing& parent);
};

CTCRegionFollower::CTCRegionFollower(CThing& parent)
    : CTCBase(parent)
{
}