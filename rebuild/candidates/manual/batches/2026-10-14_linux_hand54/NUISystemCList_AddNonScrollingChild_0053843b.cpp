// NUISystem::CList::AddNonScrollingChild(const SComponentRef_0053843b&) -- the base CChangingStateComponent adds the child (0x00533B50), then its component is appended to the pointer vector at +0x170 (push_back @ 0x00486024)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0053843B, length 42 bytes.
// Leaf: AddNonScrollingChild
// Shape: the size-optimised frame-pointer module, the appended pointer homed in the dead argument slot, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CComponent_0053843b;
struct SChildEntry_0053843b {                         // 8 bytes
    CComponent_0053843b* m_pComponent;
    long              m_index;
};
struct CRetailVector_0053843b {                       // VC7.1 Dinkumware vector: first / last / end
    SChildEntry_0053843b* m_first;
    SChildEntry_0053843b* m_last;
    SChildEntry_0053843b* m_end;
    unsigned long size() const { return m_last - m_first; }
    CComponent_0053843b* LastComponent() const { return m_first[size() - 1].m_pComponent; }
};
struct CRetailPointerVector_0053843b {                // a vector of component pointers
    CComponent_0053843b** m_first;
    CComponent_0053843b** m_last;
    CComponent_0053843b** m_end;
    void push_back(CComponent_0053843b* const& value);   // 0x00486024
};
struct SComponentRef_0053843b {                       // the argument: a reference to the component pointer
    CComponent_0053843b* m_pComponent;
    CComponent_0053843b* GetPComponent() const { return m_pComponent; }
};

struct CChangingStateComponent_0053843b {
    unsigned char _pad00[0xb0];
    CRetailVector_0053843b m_scrollingChildren;       // +0xb0
    unsigned char _padbc[0xb4];
    CRetailPointerVector_0053843b m_dynamicChildren;  // +0x170
    void AddChild(const SComponentRef_0053843b&);           // 0x00533B50
};

namespace NUISystem {
struct CList : CChangingStateComponent_0053843b {
    void AddNonScrollingChild(const SComponentRef_0053843b&);
};
}

void NUISystem::CList::AddNonScrollingChild(const SComponentRef_0053843b& child)
{
    CChangingStateComponent_0053843b::AddChild(child);
    m_dynamicChildren.push_back(child.GetPComponent());
}
