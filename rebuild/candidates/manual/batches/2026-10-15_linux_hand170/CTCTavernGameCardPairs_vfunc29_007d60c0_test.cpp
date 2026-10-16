#include <cstdio>
// Behaviour model of CTCTavernGameCardPairs::vfunc_29 (0x007D60C0): after the base EnterGame the pointer piece is created from the
// card def and placed at the game position, offset straight up by the def's pointer height.
struct V { float x, y, z; };
struct Def { long pointerDef; float height; };
static long g_created = -1;
static V g_placed;
static void Enter(const Def& d, V pos)
{
    g_created = d.pointerDef;
    V off = { 0.0f, 0.0f, d.height };
    V p = { pos.x + off.x, pos.y + off.y, pos.z + off.z };
    g_placed = p;
}
int main()
{
    Def d = { 17, 0.5f }; V pos = { 1.0f, 2.0f, 3.0f };
    Enter(d, pos);
    if (g_created != 17 || g_placed.x != 1.0f || g_placed.y != 2.0f || g_placed.z != 3.5f) { std::printf("007d60c0_TEST FAIL\n"); return 1; }
    std::printf("007d60c0_TEST PASS\n");
    return 0;
}
