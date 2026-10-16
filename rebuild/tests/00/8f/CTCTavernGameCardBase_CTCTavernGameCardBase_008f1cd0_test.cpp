#include <cstdio>
// Behaviour model of CTCTavernGameCardBase::CTCTavernGameCardBase (0x008F1CD0): card, pack and particle vectors empty, no selected or
// highlighted card, pointer scale 0 and no pointer object.
struct Vec { void* a; void* b; void* c; Vec() : a(0), b(0), c(0) { } };
struct CardBase { Vec cards, pack; void* pointer; float scale; Vec particles; void* selected; void* highlighted; };
int main()
{
    CardBase c; c.selected = 0; c.highlighted = 0; c.scale = 0; c.pointer = 0;
    if (c.cards.a || c.pack.c || c.particles.b || c.pointer || c.scale != 0 || c.selected || c.highlighted) { std::printf("008f1cd0_TEST FAIL\n"); return 1; }
    std::printf("008f1cd0_TEST PASS\n");
    return 0;
}