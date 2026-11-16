// CTCFollowed::CTCFollowed(CThing* parent) -- CTCBase(parent), own vtable, two empty STLport follower lists, the idle/moved frame counters zeroed
// Leaf: ??0CTCFollowed@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as the PDBs spell it (??0CTCFollowed@@QAE@AAVCThing@@@Z).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D4E10, length 74 bytes (+ int3 padding), `ret 4`. Single caller
// 0x004D5257 (the component factory). The manifest label "CEngineSubPrimitive2DClockSprite_Constructor" is
// propagated from the base constructor's wrong label: the vtable stored is 0x0125F644 = CTCFollowed (rtti-vtables.tsv,
// slots 9/21/22 = the landed CTCFollowed FrameUpdate/GetClassTypeName/scalar deleting destructor).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (catalog "CEngineSubPrimitive2DClockSprite" is propagated);
// malloc @ 0x00BFEA0E (STLport node allocator, one 0x10-byte sentinel per list).
// Layout: Ego_r.pdb CTCFollowed with the retail 4-byte STLport lists (landed CTCFollowed::AddFollower 0x006D50C0):
// Followers +0x0C, AIFollowers +0x10, FrameLastIdleReaction +0x14, FramesSincePlayerMoved +0x18, LastPlayerPosition
// +0x1C (C3DVector, left unset by its empty default constructor).
// Shape: the list constructor (landed std::list 0x006C28A0 model): `head(0)` before the allocation, the sentinel
// self-linked through a local before it is stored; the two counters share the zero register with the head inits.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;

struct CRetailList_node0x10 {                               // STLport list<CIntelligentPointer<T>>: one sentinel pointer
    struct node { node* next; node* prev; unsigned char value[0x10 - 8]; };
    node* head;
    CRetailList_node0x10()
        : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

#include "engine/C3DVector.h"

class CTCFollowed : public CTCBase { public:
    CRetailList_node0x10 Followers;                         // +0x0C list<CIntelligentPointer<CThing>>
    CRetailList_node0x10 AIFollowers;                       // +0x10 list<CIntelligentPointer<CThingAICreature>>
    long                 FrameLastIdleReaction;             // +0x14
    long                 FramesSincePlayerMoved;            // +0x18
    C3DVector            LastPlayerPosition;                // +0x1C
    CTCFollowed(CThing& parent);
};

CTCFollowed::CTCFollowed(CThing& parent)
    : CTCBase(parent)
    , FrameLastIdleReaction(0)
    , FramesSincePlayerMoved(0)
{
}