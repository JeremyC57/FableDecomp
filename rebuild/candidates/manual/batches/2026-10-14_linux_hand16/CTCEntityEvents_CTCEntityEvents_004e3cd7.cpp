#pragma optimize("y", off)
#pragma optimize("s", on)
// CTCEntityEvents::CTCEntityEvents(CThing*) -- CThingComponent(owner), this vtable, the member list constructed (sentinel node), nothing else.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3CD7, length 39 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// `push dword ptr [ebp+8]` pass-through): `optimize("y",off)` + `("s",on)`.
// Callees: CThingComponent::CThingComponent(CThing*) @ 0x00686800 (manifest "CEngineSubPrimitive2DClockSprite", a
// propagated label; every CTC constructor in this module calls it), std::list ctor @ 0x004DB60E (node 0x10).
// Vtable 0x01242FDC = CTCEntityEvents (rtti-vtables.tsv); the manifest label "CCardPositionsDef" is propagated and rejected.
// PDB CTCEntityEvents: the list member at +0x0C (12 bytes in the PDB, 4 in retail: the head pointer only)
// Retail std::list (VC6-era Dinkumware, one sentinel-node pointer): the constructor takes the allocator by
// const reference (`explicit list(const allocator&)`), so the caller passes the ADDRESS of an empty allocator
// temporary. The temporary has a user constructor that writes nothing, which is why VC homes it in the high
// byte of the already-consumed argument slot (`lea eax,[ebp+0Bh]`) instead of reserving a stack slot.
struct CRetailAllocator { CRetailAllocator() { } };

struct CThing;

struct CThingComponent {
    virtual void v00();
    unsigned char _pad_0x04[0x8];                // +0x04 .. +0x0B
    CThingComponent(CThing* owner);              // 0x00686800
};

struct CRetailList_node0x10 {                   // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x10(const CRetailAllocator& al = CRetailAllocator());   // 0x004DB60E
};

struct CTCEntityEvents : CThingComponent {
    virtual void v00();                          // own vtable (0x01242FDC)
    CRetailList_node0x10 Events;                  // +0x0C
    CTCEntityEvents(CThing* owner);
};

CTCEntityEvents::CTCEntityEvents(CThing* owner)
    : CThingComponent(owner), Events()
{
}
