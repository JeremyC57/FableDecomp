// CTCAISummon::CTCAISummon(CThing* parent) -- CTCBase(parent), this vtable, the summon def pointer cleared, the summoned-creature list constructed (sentinel node), LastSummonTick cleared
// Leaf: ??0CTCAISummon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004DF2D4, length 47 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through, `and dword ptr [..],0` zero stores): `optimize("y",off)` + `("s",on)`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800 (every CTC constructor of this module calls it; manifest
// "CEngineSubPrimitive2DClockSprite" is propagated), std::list ctor (const allocator&) @ 0x004B8920 (node 0x10).
// Vtable 0x01241C8C = CTCAISummon (rtti-vtables.tsv), so the manifest label "CTCFishingRod::CTCFishingRod" is propagated
// and wrong (the landed factory 0x004E181D "Create_CTCFishingRod" allocates 0x18 bytes for THIS class).
// Layout: PDB CTCAISummon (32 bytes): PSummonDef +0xc (CDefPointer<CSummonDef const>), MySummonedCreatures +0x10
// (PDB 12 bytes, retail 4: the sentinel pointer), LastSummonTick +0x1c in the PDB = +0x14 retail (sizeof 0x18).
// Shape: as the landed CTCThingOwner 0x004DDAED: the allocator temporary's address is the high byte of the consumed
// argument slot (`lea eax,[ebp+0Bh]`, scheduled before the vtable store); the def pointer's inline constructor zeroes
// it after the vtable store; LastSummonTick follows the list constructor in declaration order.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CRetailAllocator { CRetailAllocator() { } };

struct CThing;

struct CTCBase {
    virtual void v00();
    CThing*       ParentThing;                   // +0x04
    bool          ToBeRemoved;                   // +0x08
    bool          IsInGlobalUpdateTCs;           // +0x09
    CTCBase(CThing* parent);                     // 0x00686800
};

struct CDefPointeeBase;

struct CDefPointer {                             // CDefPointer<CSummonDef const>
    CDefPointeeBase* Object;
    CDefPointer() : Object(0) { }
};

struct CRetailList_node0x10 {                   // retail std::list<CIntelligentPointer<CThing>>, one sentinel-node pointer
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator());   // 0x004B8920
};

struct CTCAISummon : CTCBase {
    virtual void v00();                          // own vtable (0x01241C8C)
    CDefPointer          PSummonDef;             // +0x0c
    CRetailList_node0x10 MySummonedCreatures;    // +0x10
    long                 LastSummonTick;         // +0x14
    CTCAISummon(CThing* parent);
};

CTCAISummon::CTCAISummon(CThing* parent)
    : CTCBase(parent), PSummonDef(), MySummonedCreatures(), LastSummonTick(0)
{
}
