#include <cstdio>

static int g_upd = 0;
struct CThingCalls { void SetAsFrameUpdated() { ++g_upd; } };
struct Flags { unsigned char Moving : 1; unsigned char OldMoving : 1; };
struct Ph { void* vt; CThingCalls* sub; unsigned char pad[0x34]; unsigned char bits;
    void SetAsMoving(bool m) { sub->SetAsFrameUpdated(); ((Flags*)&bits)->Moving = m; } };
int main()
{
    CThingCalls t; Ph p; p.sub = &t; p.bits = 0xFE; p.SetAsMoving(true);
    if (g_upd != 1 || p.bits != 0xFF) { std::printf("006b0130_TEST FAIL\n"); return 1; }
    p.SetAsMoving(false); if (p.bits != 0xFE) { std::printf("006b0130_TEST FAIL\n"); return 1; }
    std::printf("006b0130_TEST PASS\n");
    return 0;
}