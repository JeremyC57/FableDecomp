// CTCIdleScheduler::CTCIdleScheduler(CThing* parent) -- CTCBase(parent), own vtable, the two idle-state maps default-constructed, then Reset() clears the maps and zeroes Preferred / FrameLastChecked
// Leaf: ??0CTCIdleScheduler@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CTCBase_Calls (which bound ??0CTCBase_Calls@@QAE@PAVCThing@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCIdleScheduler@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe @ 0x004E1537, 47 bytes (unaligned, no padding: a size-optimised module).
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800, map<EIdleStateGroup, CAIStateGroupBase*>::map() @ 0x004DFAA7,
// map<EIdleStateGroup, long>::map() @ 0x004DFA89 (both out-of-line frame-pointer template constructors, labelled
// "CDisplayManager::CopyBackBufferToTexture" by propagation), CTCIdleScheduler::Reset @ 0x008018B0 (labelled
// "LTextBinTree_RebuildWalkerArray"; it clears both maps through their erase helpers and zeroes +0x24 / +0x28).
// Label: the manifest's "CTattooDef::CTattooDef" is wrong -- the vtable stored, 0x012426C4, is CTCIdleScheduler in
// rtti-vtables.tsv, and the PDB CTCIdleScheduler (IdleStates +0xc, IdleStatesWeight, Preferred, FrameLastChecked)
// fits the member constructors at +0xc / +0x18 and the zeroed +0x24 / +0x28 (retail maps are 0xc bytes, PDB 0x10).
// Pragma: #pragma optimize("s", on) -- `push dword ptr [esp+8]` for the forwarded argument is the /Os shape.
// Shape: the derived vtable store follows the base call and precedes the member constructors; the Reset call is the body.
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CThing;
class CAIStateGroupBase;

struct CIdleStateMap {                             // map<EIdleStateGroup, CAIStateGroupBase*> (retail 0xc bytes)
    void*         _Myhead;                         // +0x00
    unsigned long _Mysize;                         // +0x04
    unsigned long _Extra;                          // +0x08
    CIdleStateMap();                               // 0x004DFAA7
};

struct CIdleWeightMap {                            // map<EIdleStateGroup, long> (retail 0xc bytes)
    void*         _Myhead;                         // +0x00
    unsigned long _Mysize;                         // +0x04
    unsigned long _Extra;                          // +0x08
    CIdleWeightMap();                              // 0x004DFA89
};

class CTCIdleScheduler : public CTCBase { public:          // vtable 0x012426C4
    CIdleStateMap  IdleStates;                     // +0x0c
    CIdleWeightMap IdleStatesWeight;               // +0x18
    long           Preferred;                      // +0x24 EIdleStateGroup
    long           FrameLastChecked;               // +0x28
    CTCIdleScheduler(CThing& parent);
    void Clear();                                  // 0x008018B0; NF296: PDB name, was Reset
};

CTCIdleScheduler::CTCIdleScheduler(CThing& parent)
    : CTCBase(parent)
{
    Clear();
}