// NUISystem::CScrollableList::CScrollableList(long) -- a changing-state UI component whose scroll state (three dwords, two flags) starts cleared
// Leaf: ??0CScrollableList
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00555180, 70 bytes (+10 int3 padding). Manifest label
// "CTCHeroPodium::CTCHeroPodium" is propagated from an unrelated body: the three vtables stored are
// NUISystem::CScrollableList's (0x0124A72C primary, 0x0124A708 for the base at +4, 0x0124A700 for the base at
// +0x18 -- rtti-vtables.tsv); callers 0x0041D3D2 and 0x00556358.
// Callee: NUISystem::CChangingStateComponent::CChangingStateComponent(long) @ 0x0052CC50.
//
// Shape: a speed TU (argument loaded into eax and pushed, register zero `xor eax,eax`), unlike the size-optimised
// CYesNo/COk siblings; the zero stores (+0x16C, +0x170, +0x174 dwords, +0x178/+0x179 bytes) precede the three
// vtable stores (init-list members of the most-derived class). The 16 bytes at +0x15C..+0x16C are left alone by
// the constructor (plain data the list fills later).

namespace NUISystem {

struct CComponentRoot_Retail {                 // primary base: vptr only
    virtual void v00();
};

struct CComponentBase2_Retail {                // base at +0x04, 0x14 bytes
    virtual void v00();
    unsigned char _body[0x10];
};

struct CComponentBase3_Retail {                // base at +0x18
    virtual void v00();
};

struct CChangingStateComponent : CComponentRoot_Retail, CComponentBase2_Retail, CComponentBase3_Retail {
    unsigned char _body[0x15c - 0x1c];         // +0x1c..+0x15c
    CChangingStateComponent(long state);       // 0x0052CC50
};

struct CScrollableList : CChangingStateComponent {   // vtables 0x0124A72C / 0x0124A708 / 0x0124A700
    unsigned char m_layout[0x10];              // +0x15C (not initialised here)
    long          m_firstVisible;              // +0x16C
    long          m_selected;                  // +0x170
    long          m_count;                     // +0x174
    bool          m_scrollUp;                  // +0x178
    bool          m_scrollDown;                // +0x179
    virtual void v00();
    CScrollableList(long state);
};

CScrollableList::CScrollableList(long state)
    : CChangingStateComponent(state), m_firstVisible(0), m_selected(0), m_count(0),
      m_scrollUp(false), m_scrollDown(false)
{
}

}
