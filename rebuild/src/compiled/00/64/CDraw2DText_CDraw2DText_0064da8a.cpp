#pragma optimize("y", off)
#pragma optimize("s", on)
// CDraw2DText::CDraw2DText(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawBase(gui, draws), this vtable, then the member list at +0x28
// constructed (sentinel node of 0x34 bytes).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0064DA8A, length 42 bytes; frame-pointer size TU (`push ebp ; mov ebp,esp`,
// pass-through `push dword ptr [ebp+0Ch]` / `[ebp+8]`): `optimize("y",off)` + `("s",on)`.
// Callees: CDrawBase::CDrawBase(CPlayerGui*, vector<CDrawBase*>*) @ 0x0064CC65 (landed callers 0x0064D2C5 ..),
// std::list ctor @ 0x00655B4E (manifest "std_list_allocate_sentinel_0x34").
// Vtable 0x0125A5D0 = NPlayerGui::CDraw2DText (rtti-vtables.tsv). The manifest label "CCardPositionsDef" is propagated and rejected.
// Retail std::list (VC6-era Dinkumware, one sentinel-node pointer): the constructor takes the allocator by
// const reference (`explicit list(const allocator&)`), so the caller passes the ADDRESS of an empty allocator
// temporary. The temporary has a user constructor that writes nothing, which is why VC homes it in the high
// byte of the already-consumed argument slot (`lea eax,[ebp+0Bh]`) instead of reserving a stack slot.
struct CRetailAllocator { CRetailAllocator() { } };

class CPlayerGui;

struct CDrawBase {
    virtual void v00();
    unsigned char _pad_0x04[0x24];               // +0x04 .. +0x27 CDrawBase members
    CDrawBase(CPlayerGui* gui, void* draws);     // 0x0064CC65
};

struct CRetailList_node0x34 {                   // retail std::list, one sentinel-node pointer
    void* head;
    CRetailList_node0x34(const CRetailAllocator& al = CRetailAllocator());   // 0x00655B4E
};

struct CDraw2DText : CDrawBase {
    virtual void v00();                          // own vtable (0x0125A5D0)
    CRetailList_node0x34 m_entries;              // +0x28
    CDraw2DText(CPlayerGui* gui, void* draws);
};

CDraw2DText::CDraw2DText(CPlayerGui* gui, void* draws)
    : CDrawBase(gui, draws), m_entries()
{
}