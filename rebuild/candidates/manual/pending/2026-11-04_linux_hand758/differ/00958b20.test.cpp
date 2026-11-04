// Behaviour test for CAIStateGroup_MinionReactToCrime::OnReplace (0x00958B20), g++ model of the release / withdraw / mask-reset steps.
#include <cstdio>
struct State { bool buildingAlive; long forceOpen; bool guardTold; bool hasMessage; const void* sender; const void* me; bool removed;
               bool deedMaskApplied; bool hasOpinion; long mask, defMask; };
static void OnReplace(State& s)
{
    if (s.buildingAlive) --s.forceOpen;
    if (!s.guardTold && s.hasMessage && s.sender == s.me) s.removed = true;
    s.guardTold = false;
    if (s.deedMaskApplied) { if (s.hasOpinion) s.mask = s.defMask; s.deedMaskApplied = false; }
}
int main()
{
    int me = 0, other = 0;
    State a = { true, 2, false, true, &me, &me, false, true, true, 0, 9 }; OnReplace(a);
    int ok = a.forceOpen == 1 && a.removed && !a.guardTold && a.mask == 9 && !a.deedMaskApplied;
    State b = { false, 2, true, true, &me, &me, false, true, false, 0, 9 }; OnReplace(b);
    ok = ok && b.forceOpen == 2 && !b.removed && !b.guardTold && b.mask == 0 && !b.deedMaskApplied;
    State c = { false, 0, false, true, &other, &me, false, false, true, 1, 9 }; OnReplace(c); ok = ok && !c.removed && c.mask == 1;
    std::printf(ok ? "00958b20_TEST PASS\n" : "00958b20_TEST FAIL\n");
    return ok ? 0 : 1;
}
