#include <cstdio>
#include <cstring>
#include <new>
static long g_owner = 0; static int g_dataCalls = 0;
namespace NUISystem {
struct IComponentA { virtual void va00() { } unsigned char _state[0x10]; };
struct IComponentB { virtual void vb00() { } };
struct CComponentRoot { virtual void v00() { } };
struct CChangingStateComponent : CComponentRoot, IComponentA, IComponentB {
    unsigned char _body[0x15c - 0x1c];
    CChangingStateComponent(long owner) { g_owner = owner; }
};
struct CComponent;
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) { } };
struct C2DVector { float x, y; };
struct CSpellContainerList : CChangingStateComponent {
    virtual void v00() { }
    virtual void va00() { }
    virtual void vb00() { }
    float LastInputTime; unsigned long SpellIndex; unsigned long SelectedChild;
    CComponent* SpellSelectionChild; CComponent* SpellIndicationChild;
    CRetailVector<CComponent*> SpellContainers;
    long SpellToAssign; C2DVector PositionOffset; float AlphaOffset; float NextSpellContainerAlpha;
    C2DVector NextSpellContainerPosition; C2DVector PositionOffsetToUse; float AlphaOffsetToUse; float InputDelay;
    void GetDataFromDef() { ++g_dataCalls; SpellIndex = 3; }
    CSpellContainerList(long owner) : CChangingStateComponent(owner), SpellSelectionChild(0), InputDelay(0) { GetDataFromDef(); }
};
}
int main()
{
    static unsigned char buf[sizeof(NUISystem::CSpellContainerList)];
    std::memset(buf, 0xCD, sizeof(buf));
    NUISystem::CSpellContainerList* l = new (buf) NUISystem::CSpellContainerList(0x1234);
    bool ok = g_owner == 0x1234 && g_dataCalls == 1 && l->SpellIndex == 3 && l->SpellSelectionChild == 0
        && l->SpellContainers.first == 0 && l->SpellContainers.last == 0 && l->SpellContainers.end == 0 && l->InputDelay == 0.0f;
    if (!ok) { std::printf("00544b70_TEST FAIL\n"); return 1; }
    std::printf("00544b70_TEST PASS\n");
    return 0;
}