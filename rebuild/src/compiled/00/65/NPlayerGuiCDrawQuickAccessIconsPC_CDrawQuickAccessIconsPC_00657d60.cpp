// NPlayerGui::CDrawQuickAccessIconsPC::CDrawQuickAccessIconsPC(CPlayerGui*, std::vector<CDrawBase*>*) -- CDrawQuickAccessIcons(gui, list), own vtable, then the primitive-handle map at +0x94 is given its entry for key 2
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00657D60, length 43 bytes.
// Callees: NPlayerGui::CDrawQuickAccessIcons::CDrawQuickAccessIcons(CPlayerGui*, vector<CDrawBase*>*) @ 0x00652F0A, map<long, CEnginePrimitiveHandle>::operator[](long)
// @ 0x00485D30 (the key by value: `push 2`; its result is dropped -- the call only creates the entry). Vtable 0x0125A804 = NPlayerGui::CDrawQuickAccessIconsPC
// (rtti-vtables.tsv); the chunk label "CDrawBettingHUD::CDrawBettingHUD" is propagated. Shape: a speed body (arguments through registers), the vtable store
// sits between the key push and the member call.
struct CPlayerGui;
struct CEnginePrimitiveHandle;

namespace NPlayerGui {

struct CRetailHandleMap {                          // VC6 Dinkumware map<long, CEnginePrimitiveHandle>
    unsigned char _tree[8];
    CEnginePrimitiveHandle* operator[](long key);  // 0x00485D30
};

struct CDrawQuickAccessIcons {
    virtual void v00();
    unsigned char _body[0x94 - 4];                 // +0x04 .. +0x94: CDrawBase and the icon state
    CDrawQuickAccessIcons(CPlayerGui* gui, void* drawList);   // 0x00652F0A
};

struct CDrawQuickAccessIconsPC : CDrawQuickAccessIcons {   // vtable 0x0125A804
    virtual void v00();
    CRetailHandleMap m_handles;                    // +0x94
    CDrawQuickAccessIconsPC(CPlayerGui* gui, void* drawList);
};

CDrawQuickAccessIconsPC::CDrawQuickAccessIconsPC(CPlayerGui* gui, void* drawList)
    : CDrawQuickAccessIcons(gui, drawList)
{
    m_handles[2];
}

}