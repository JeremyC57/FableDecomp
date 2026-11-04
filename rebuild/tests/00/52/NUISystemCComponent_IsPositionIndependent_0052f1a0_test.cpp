// Behaviour test for NUISystem::CComponent::IsPositionIndependent 0x0052F1A0: a component is position-independent exactly when its
// (virtual) IsIndependent says so, so an override of IsIndependent changes the answer.
#include <cstdio>
struct CComponent {
    bool Independent;
    virtual ~CComponent() {}
    virtual bool IsIndependent() const { return Independent; }
    virtual bool IsPositionIndependent() const { return IsIndependent(); }
};
struct CAlwaysIndependent : CComponent { bool IsIndependent() const { return true; } };
int main()
{
    CComponent a1; a1.Independent = false;
    CAlwaysIndependent a2; a2.Independent = false;
    if (a1.IsPositionIndependent() || !a2.IsPositionIndependent()) { std::printf("0052f1a0_TEST FAIL\n"); return 1; }
    a1.Independent = true;
    if (!a1.IsPositionIndependent()) { std::printf("0052f1a0_TEST FAIL\n"); return 1; }
    std::printf("0052f1a0_TEST PASS\n");
    return 0;
}