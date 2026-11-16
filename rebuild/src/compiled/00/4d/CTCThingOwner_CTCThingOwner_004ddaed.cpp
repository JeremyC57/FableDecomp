#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCThingOwner::CTCThingOwner(CThing&) -- CTCBase(owner), this vtable, the member list constructed (sentinel node), then CallOnDie cleared.
// Leaf: ??0CTCThingOwner@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model (CThingComponent, which bound a made-up constructor name to
//   0x00686800) is gone; the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DDAED, length 43 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through): `optimize("y",off)` + `("s",on)`.
// Callees: CTCBase::CTCBase(CThing&) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", a
// propagated label; every CTC constructor in this module calls it), std::list ctor @ 0x004B8920 (node 0x10).
// Vtable 0x012412A4 = CTCThingOwner (rtti-vtables.tsv); the manifest label "CCardPositionsDef" is propagated and rejected.
// PDB CTCThingOwner: the list member at +0x0C (12 bytes in the PDB, 4 in retail: the head pointer only), the bool after it at +0x10
// Retail std::list (VC6-era Dinkumware, one sentinel-node pointer): the constructor takes the allocator by
// const reference (`explicit list(const allocator&)`), so the caller passes the ADDRESS of an empty allocator
// temporary. The temporary has a user constructor that writes nothing, which is why VC homes it in the high
// byte of the already-consumed argument slot (`lea eax,[ebp+0Bh]`) instead of reserving a stack slot.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
struct CRetailAllocator { CRetailAllocator() { } };

class CThing;

struct CRetailList_node0x10 {                   // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator());   // 0x004B8920
};

class CTCThingOwner : public CTCBase { public:
    CRetailList_node0x10 ObjectList;                  // +0x0C
    bool CallOnDie;                              // +0x10
    CTCThingOwner(CThing& owner);
};

CTCThingOwner::CTCThingOwner(CThing& owner)
    : CTCBase(owner), ObjectList(), CallOnDie(false)
{
}