// NUISystem::CList::AddChildDynamically(CComponent_005383a4* child) -- the base CChangingStateComponent adds the child (0x0052CAB0), then the last scrolling child's component (the 8-byte pairs at +0xb0, index size()-1) is appended to the pointer vector at +0x170 (push_back @ 0x00486024)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005383A4, length 57 bytes.
// Leaf: AddChildDynamically
// Shape: the size-optimised frame-pointer module, the appended pointer homed in the dead argument slot, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CComponent_005383a4;
struct SChildEntry_005383a4 {                         // 8 bytes
    CComponent_005383a4* m_pComponent;
    long              m_index;
};
struct CRetailVector_005383a4 {                       // VC7.1 Dinkumware vector: first / last / end
    SChildEntry_005383a4* m_first;
    SChildEntry_005383a4* m_last;
    SChildEntry_005383a4* m_end;
    unsigned long size() const { return m_last - m_first; }
    CComponent_005383a4* LastComponent() const { return m_first[size() - 1].m_pComponent; }
};
struct CRetailPointerVector_005383a4 {                // a vector of component pointers
    CComponent_005383a4** m_first;
    CComponent_005383a4** m_last;
    CComponent_005383a4** m_end;
    void push_back(CComponent_005383a4* const& value);   // 0x00486024
};

struct CChangingStateComponent_005383a4 {
    unsigned char _pad00[0xb0];
    CRetailVector_005383a4 m_scrollingChildren;       // +0xb0
    unsigned char _padbc[0xb4];
    CRetailPointerVector_005383a4 m_dynamicChildren;  // +0x170
    void AddChildDynamically(CComponent_005383a4* child);   // 0x0052CAB0
};

namespace NUISystem {
struct CList : CChangingStateComponent_005383a4 {
    void AddChildDynamically(CComponent_005383a4* child);
};
}

void NUISystem::CList::AddChildDynamically(CComponent_005383a4* child)
{
    CChangingStateComponent_005383a4::AddChildDynamically(child);
    CComponent_005383a4* last = m_scrollingChildren.m_first[m_scrollingChildren.size() - 1].m_pComponent;
    m_dynamicChildren.push_back(last);
}
