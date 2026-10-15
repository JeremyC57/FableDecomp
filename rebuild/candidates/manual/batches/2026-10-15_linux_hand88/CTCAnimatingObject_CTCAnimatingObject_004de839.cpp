// CTCAnimatingObject::CTCAnimatingObject(CThing* owner) -- thing-component base, not animating, priority -1, the queued-animation list built
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DE839, length 47 bytes. Callees: CTCBase::CTCBase(CThing*) @ 0x00686800
// (manifest label "CEngineSubPrimitive2DClockSprite", propagated), the std::list<CAnimInit> constructor @ 0x004DB674
// (landed as ListNode_InitSelfLinked_34: one 0x34-byte sentinel node).
// Label correction: the manifest calls this a CTCGraphicAppearanceAnimatedMesh constructor; the vtable it stores,
// 0x012417E4, is CTCAnimatingObject's (rtti-vtables.tsv) and the PDB CTCAnimatingObject (32 bytes) fits every store.
// Its only caller is the factory @ 0x004E0C1E (operator new(0x18) -- the retail list is one sentinel pointer).
// Layout from Ego_r.pdb: the CTCBase part +0x00..+0x0b, Animating (bool) +0x0c, CurrentAnimationPriority (long) +0x10,
// QueuedAnims (std::list<CAnimInit>) +0x14.
// Shape: frame-pointer size TU (`push ebp ; mov ebp,esp`, `push dword ptr [ebp+8]`, `or dword ptr [..],-1`):
// `optimize("y",off)` + `("s",on)`. The list takes its allocator by const reference: the empty allocator temporary
// (user constructor, writes nothing) is homed in the high byte of the dead `owner` slot (`lea eax,[ebp+0Bh]`).
// The engine header CTCBase.h is a flattened POD (`void* __vftable`), unusable as a polymorphic base: local model.
// Leaf: ??0CTCAnimatingObject
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CThing;

struct CTCBase {                                        // 0x0c bytes: vptr, ParentThing, ToBeRemoved, IsInGlobalUpdateTCs
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    CTCBase(CThing* owner);                             // 0x00686800
};

struct CRetailAllocator { CRetailAllocator() { } };

struct CRetailList_CAnimInit {                          // retail std::list<CAnimInit>, one sentinel-node pointer
    void* head;
    CRetailList_CAnimInit(const CRetailAllocator& al = CRetailAllocator());   // 0x004DB674
};

struct CTCAnimatingObject : CTCBase {
    bool                  Animating;                    // +0x0c
    long                  CurrentAnimationPriority;     // +0x10
    CRetailList_CAnimInit QueuedAnims;                  // +0x14
    virtual void v00();
    CTCAnimatingObject(CThing* owner);
};

CTCAnimatingObject::CTCAnimatingObject(CThing* owner)
    : CTCBase(owner), Animating(false), CurrentAnimationPriority(-1), QueuedAnims()
{
}
