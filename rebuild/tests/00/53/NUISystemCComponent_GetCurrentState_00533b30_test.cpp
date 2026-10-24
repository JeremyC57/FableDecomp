// Behaviour test for 0x00533B30 NUISystem::CComponent::GetCurrentState
#include <cstdio>
#include <map>
struct CUIState { unsigned long StateChangeFlag; long StateChangeType; float X, Y; };
struct CComponent {
    std::map<unsigned long, CUIState> States;
    CUIState* GetCurrentState() { return &States[0]; }
};
int main()
{
    CComponent c;
    CUIState* s = c.GetCurrentState();
    int ok = c.States.size() == 1 && s == &c.States[0];
    s->StateChangeType = 4;
    CUIState two = { 1, 2, 0.0f, 0.0f };
    c.States[2] = two;
    ok = ok && c.GetCurrentState()->StateChangeType == 4 && c.GetCurrentState() == s && c.States.size() == 2;
    if (!ok) { std::printf("00533b30_TEST FAIL\n"); return 1; }
    std::printf("00533b30_TEST PASS\n");
    return 0;
}