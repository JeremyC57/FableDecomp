// Behaviour test for NUISystem::CSpellContainerList::AddSpellIndicationChild (0x00544330): the child's raw pointer is remembered in
// SpellIndicationChild and the child is added through the CComponent base AddChild, not through the list's own AddChild override.
#include <cstdio>
class CComponent;
template <class T> class CCountedPointer { public: T* Data; void* Info; T* Get() const { return Data; } };
static int g_baseAdds = 0, g_overrideAdds = 0;
static CComponent* g_lastAdded = 0;
class CComponent {
public:
    virtual ~CComponent() {}
    virtual void AddChild(CCountedPointer<CComponent>& child) { ++g_baseAdds; g_lastAdded = child.Data; }
};
class CSpellContainerList : public CComponent {
public:
    CSpellContainerList() : SpellSelectionChild(0), SpellIndicationChild(0) {}
    virtual void AddChild(CCountedPointer<CComponent>& child) { ++g_overrideAdds; (void)child; }
    virtual void AddSpellIndicationChild(CCountedPointer<CComponent>& child)
    {
        SpellIndicationChild = child.Get();
        CComponent::AddChild(child);
    }
    CComponent* SpellSelectionChild;
    CComponent* SpellIndicationChild;
};
int main()
{
    CComponent child;
    CCountedPointer<CComponent> p; p.Data = &child; p.Info = 0;
    CSpellContainerList list;
    list.AddSpellIndicationChild(p);
    if (list.SpellIndicationChild != &child || g_baseAdds != 1 || g_overrideAdds != 0 || g_lastAdded != &child) {
        std::printf("00544330_TEST FAIL\n"); return 1;
    }
    std::printf("00544330_TEST PASS\n");
    return 0;
}