// Behaviour test for 0x00692A50 CCreatureActionBase::IsUsingActionMovement: movement allowed by the creature or forced animation, then animation or forced movement (standalone copy of the logic)
#include <cstdio>
struct Action { bool use, always, anim, forced;
    bool IsUsingActionMovement() const { if (!use && !always) return false; return anim || forced; } };

int main()
{
    Action a; a.use = false; a.always = false; a.anim = true; a.forced = true; if (a.IsUsingActionMovement()) { std::printf("00692a50_TEST FAIL\n"); return 1; }
    a.use = true; a.anim = false; a.forced = false; if (a.IsUsingActionMovement()) { std::printf("00692a50_TEST FAIL\n"); return 1; }
    a.forced = true; if (!a.IsUsingActionMovement()) { std::printf("00692a50_TEST FAIL\n"); return 1; }
    a.use = false; a.always = true; a.forced = false; a.anim = true; if (!a.IsUsingActionMovement()) { std::printf("00692a50_TEST FAIL\n"); return 1; }
    std::printf("00692a50_TEST PASS\n");
    return 0;
}