// CTCHeroGuide::CTCHeroGuide(CThing* parent) -- CTCBase(parent), own vtable, the QuestRewards list (list<CTCHeroGuide::CQuestReward>, 0x14-byte nodes) default-constructed
// Leaf: ??0CTCHeroGuide@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCHeroGuide@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe @ 0x00780070, 50 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, malloc @ 0x00BFEA0E (the STLport node allocator).
// Label: the manifest's "CTextContainer::CTextContainer" is propagated -- the vtable stored, 0x0126B0F4, is CTCHeroGuide
// in rtti-vtables.tsv; PDB CTCHeroGuide holds only QuestRewards at +0xc.
// Shape: identical to CTCRegionFollower's constructor (0x006AED80) with a 0xc-byte element.
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

struct CQuestReward { unsigned char _body[0xc]; };  // CTCHeroGuide::CQuestReward (retail 0xc bytes)

class CTCHeroGuide : public CTCBase { public:              // vtable 0x0126B0F4
    CRetailList<CQuestReward> QuestRewards;        // +0x0c
    CTCHeroGuide(CThing& parent);
};

CTCHeroGuide::CTCHeroGuide(CThing& parent)
    : CTCBase(parent)
{
}