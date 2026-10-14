#pragma optimize("y", off)
#pragma optimize("s", on)
// CVtable_0125a5a4::CVtable_0125a5a4(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, draws), this vtable, then the member list at +0x28
// constructed (sentinel node of 0x14 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064D3DA, length 42 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// pass-through `push dword ptr [ebp+0Ch]` / `[ebp+8]`): `optimize("y",off)` + `("s",on)`.
// Callees: CDrawBase::CDrawBase(CPlayerGui*, vector<CDrawBase*>*) @ 0x0064CC65 (landed callers 0x0064D2C5 ..),
// std::list ctor @ 0x00655B18 (manifest "std_list_allocate_sentinel_0x14").
// Vtable 0x0125A5A4 has no RTTI row (its neighbours 0x0125A58C CDrawAutoPickups / 0x0125A598 are the sibling NPlayerGui draw classes), so the class is named by its vtable. The manifest label "CCardPositionsDef" is propagated and rejected.
// Retail std::list (VC6-era Dinkumware, one sentinel-node pointer): the constructor takes the allocator by
// const reference (`explicit list(const allocator&)`), so the caller passes the ADDRESS of an empty allocator
// temporary. The temporary has a user constructor that writes nothing, which is why VC homes it in the high
// byte of the already-consumed argument slot (`lea eax,[ebp+0Bh]`) instead of reserving a stack slot.
struct CRetailAllocator { CRetailAllocator() { } };

struct CPlayerGui;

struct CDrawBase {
    virtual void v00();
    unsigned char _pad_0x04[0x24];               // +0x04 .. +0x27 CDrawBase members
    CDrawBase(CPlayerGui* gui, void* draws);     // 0x0064CC65
};

struct CRetailList_node0x14 {                   // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x14(const CRetailAllocator& al = CRetailAllocator());   // 0x00655B18
};

struct CVtable_0125a5a4 : CDrawBase {
    virtual void v00();                          // own vtable (0x0125A5A4)
    CRetailList_node0x14 m_entries;              // +0x28
    CVtable_0125a5a4(CPlayerGui* gui, void* draws);
};

CVtable_0125a5a4::CVtable_0125a5a4(CPlayerGui* gui, void* draws)
    : CDrawBase(gui, draws), m_entries()
{
}