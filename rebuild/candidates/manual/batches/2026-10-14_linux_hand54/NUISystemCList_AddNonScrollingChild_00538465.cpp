// NUISystem::CList::AddNonScrollingChild(const SComponentRef_00538465&) -- the base CChangingStateComponent adds the child (0x0052CAB0), then its component is appended to the pointer vector at +0x170 (push_back @ 0x00486024)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00538465, length 42 bytes.
// Leaf: AddNonScrollingChild
// Shape: the size-optimised frame-pointer module, the appended pointer homed in the dead argument slot, `ret 4`.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CComponent_00538465;
struct SChildEntry_00538465 {                         // 8 bytes
    CComponent_00538465* m_pComponent;
    long              m_index;
};
struct CRetailVector_00538465 {                       // VC7.1 Dinkumware vector: first / last / end
    SChildEntry_00538465* m_first;
    SChildEntry_00538465* m_last;
    SChildEntry_00538465* m_end;
    unsigned long size() const { return m_last - m_first; }
    CComponent_00538465* LastComponent() const { return m_first[size() - 1].m_pComponent; }
};
struct CRetailPointerVector_00538465 {                // a vector of component pointers
    CComponent_00538465** m_first;
    CComponent_00538465** m_last;
    CComponent_00538465** m_end;
    void push_back(CComponent_00538465* const& value);   // 0x00486024
};
struct SComponentRef_00538465 {                       // the argument: a reference to the component pointer
    CComponent_00538465* m_pComponent;
    CComponent_00538465* GetPComponent() const { return m_pComponent; }
};

struct CChangingStateComponent_00538465 {
    unsigned char _pad00[0xb0];
    CRetailVector_00538465 m_scrollingChildren;       // +0xb0
    unsigned char _padbc[0xb4];
    CRetailPointerVector_00538465 m_dynamicChildren;  // +0x170
    void AddChildDynamically(const SComponentRef_00538465&);           // 0x0052CAB0
};

namespace NUISystem {
struct CList : CChangingStateComponent_00538465 {
    void AddNonScrollingChild(const SComponentRef_00538465&);
};
}

void NUISystem::CList::AddNonScrollingChild(const SComponentRef_00538465& child)
{
    CChangingStateComponent_00538465::AddChildDynamically(child);
    m_dynamicChildren.push_back(child.GetPComponent());
}
