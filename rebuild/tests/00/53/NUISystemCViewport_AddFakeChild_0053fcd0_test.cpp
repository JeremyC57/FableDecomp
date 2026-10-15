#include <cstdio>
#ifndef _MSC_VER
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif
namespace NUISystem {
struct CComponent;
static CComponent* g_pushed = 0; static CComponent* g_parentSet = 0; static CComponent* g_parentTarget = 0;
struct CComponentVector { CComponent* m_pFirst; CComponent* m_pLast; CComponent* m_pEnd; void push_back(CComponent* const& child) { g_pushed = child; } };
struct CComponent {
    virtual void v00() { }
    virtual void SetParent(CComponent* parent) { g_parentTarget = this; g_parentSet = parent; }
};
struct CViewport : CComponent {
    unsigned char _pad04[0x160]; CComponentVector m_fakeChildren;
    void AddFakeChild(CComponent* child) { m_fakeChildren.push_back(child); child->SetParent(this); }
};
}
int main()
{
    NUISystem::CViewport viewport; NUISystem::CComponent child;
    viewport.AddFakeChild(&child);
    if (NUISystem::g_pushed != &child || NUISystem::g_parentTarget != &child || NUISystem::g_parentSet != &viewport) { std::printf("0053fcd0_TEST FAIL\n"); return 1; }
    std::printf("0053fcd0_TEST PASS\n"); return 0;
}