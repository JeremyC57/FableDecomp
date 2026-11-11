// Behaviour test for NUISystem::CComponent::IsZoomIndependent (0x0052F1B0): it answers with the (virtual) IsIndependent.
#include <stdio.h>
namespace NUISystem {
class CComponent { public:
    bool independent;
    CComponent(bool i) : independent(i) { }
    virtual ~CComponent() { }
    virtual bool IsIndependent() const { return independent; }
    virtual bool IsZoomIndependent() const;
};
bool CComponent::IsZoomIndependent() const
{
    return IsIndependent();
}
struct Always : CComponent {
    Always() : CComponent(false) { }
    virtual bool IsIndependent() const { return true; }
};
}
int main(void)
{
    NUISystem::CComponent a(false), b(true); NUISystem::Always c;
    if (a.IsZoomIndependent() || !b.IsZoomIndependent() || !c.IsZoomIndependent()) { printf("0052f1b0_TEST FAIL\n"); return 1; }
    printf("0052f1b0_TEST PASS\n");
    return 0;
}