// NUISystem::CRedefinerList::OnDeactivate() -- leave the key-redefinition list: clear the active flag, hide the list over 0.2 s, reset the fade time and the second flag, and push control states 0x25 and 0x24
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556580, length 61 bytes. Vtable slot 141 of NUISystem::CRedefinerList
// (0x0124A994, rtti-vtables.tsv). Callees: CScrollableList::HideList(float) @ 0x005559D0, the control base's virtual slot 4
// (+0x10; the landed CKeyRedefiner::CancelSelection @ 0x00557BD0 names it SetState) twice.
// Layout: descriptive (no engine header; the PDB class is all padding): the list's active flag +0x17c, a second flag
// +0x17d, a float fade time +0x180; the control base (second vptr) sits at +4 as in CKeyRedefiner.
// Shape: the 0.2f literal is pushed as an immediate and stored as one; the first flag store is scheduled between the push
// and the call; `this` is converted to the control base once (`add esi,4`) and the second virtual call re-reads its vtable.
// Leaf: OnDeactivate
namespace NUISystem {

struct CListRoot_00556580 {                             // primary base (vptr at +0)
    virtual void v000();
};
struct CGuiControl_00556580 {                           // the control base (vptr at +4)
    virtual void v000();
    virtual void v001();
    virtual void v002();
    virtual void SetMode(long mode);                    // slot 3 (+0x0c)
    virtual void SetState(long state);                  // slot 4 (+0x10)
};

struct CScrollableList : CListRoot_00556580, CGuiControl_00556580 {
    unsigned char _body[0x174];                         // +0x008 .. +0x17b
    void HideList(float time);                          // 0x005559D0
};

struct CRedefinerList : CScrollableList {
    bool  m_bActive;                                    // +0x17c
    bool  m_bSelecting;                                 // +0x17d
    float m_fadeTime;                                   // +0x180
    void OnDeactivate();
};

void CRedefinerList::OnDeactivate()
{
    m_bActive = false;
    HideList(0.2f);
    m_fadeTime = 0.2f;
    m_bSelecting = false;
    SetState(0x25);
    SetState(0x24);
}

}