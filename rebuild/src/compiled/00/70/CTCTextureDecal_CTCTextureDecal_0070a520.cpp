// CTCTextureDecal::CTCTextureDecal(CThing&) -- CTCBase(thing), own vtable, an empty decal list (STLport sentinel node malloc'd and self-linked), an empty engine-decal vector, the next allocation entry id 1, not dirty
// Leaf: ??0CTCTextureDecal@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCTextureDecal@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070A520, length 69 bytes (+ int3 padding).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (the manifest label "CEngineSubPrimitive2DClockSprite"
// is propagated), malloc @ 0x00BFEA0E (the 0x20-byte list node). Vtable 0x01264FAC = CTCTextureDecal
// (rtti-vtables.tsv): the chunk label "CTCCreatureFlyOnSpline::CTCCreatureFlyOnSpline" is propagated.
// Layout (retail, below the PDB): Decals +0xc is a 4-byte STLport list (just the sentinel node pointer;
// the PDB list is 12 bytes), EngineDecals +0x10 a 12-byte vector, CurrentAllocEntryID +0x1c, Dirty +0x20.
// Shape: the list's inline constructor zeroes the node pointer, allocates the sentinel, links it to
// itself and then stores it (STLport _List_base); the vector is a member object with an inline
// constructor; the zeroes share ebx; malloc's `add esp,4` is delayed past the member stores.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
#include <stdlib.h>
class CThing;
class CEngineTextureDecal;

struct SDecalNode {                                         // STLport _List_node<CTCTextureDecal::CEntry>: 0x20 bytes
    SDecalNode*   Next;                                     // +0x00
    SDecalNode*   Prev;                                     // +0x04
    unsigned char Entry[0x18];                              // +0x08 CTCTextureDecal::CEntry
};

struct CRetailList_DecalEntry {                             // STLport list<CEntry>: the sentinel pointer only
    SDecalNode* Node;
    CRetailList_DecalEntry() : Node(0)
    {
        SDecalNode* sentinel = static_cast<SDecalNode*>(malloc(sizeof(SDecalNode)));
        sentinel->Next = sentinel;
        sentinel->Prev = sentinel;
        Node = sentinel;
    }
};

struct CRetailVector_EngineDecal {                          // retail std::vector<CEngineTextureDecal>: 12 bytes
    CEngineTextureDecal* First;
    CEngineTextureDecal* Last;
    CEngineTextureDecal* End;
    CRetailVector_EngineDecal() : First(0), Last(0), End(0) { }
};

class CTCTextureDecal : public CTCBase { public:                    // vtable 0x01264FAC
    CRetailList_DecalEntry    Decals;                       // +0x0c
    CRetailVector_EngineDecal EngineDecals;                 // +0x10
    long                      CurrentAllocEntryID;          // +0x1c
    bool                      Dirty;                        // +0x20
    CTCTextureDecal(CThing& thing);
};

CTCTextureDecal::CTCTextureDecal(CThing& thing)
    : CTCBase(thing)
    , Decals()
    , EngineDecals()
    , CurrentAllocEntryID(1)
    , Dirty(false)
{
}