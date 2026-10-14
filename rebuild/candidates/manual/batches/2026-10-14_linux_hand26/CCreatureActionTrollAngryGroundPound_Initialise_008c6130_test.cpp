#include <cstdio>
static int g_base = 0, g_pound = 0;
struct Creature { unsigned char pad[0x1c8]; unsigned char flags; };
struct Ptr { virtual ~Ptr() {} Creature* p; Creature* GetPItem() const { return p; } };
struct Base { virtual ~Base() {} virtual void Initialise() { ++g_base; } Ptr PCreature; };
struct Act : Base { void Initialise() { SetUpPound(); Base::Initialise(); PCreature.GetPItem()->flags &= ~0x40; } virtual void SetUpPound() { ++g_pound; } };
int main()
{
    Creature c; c.flags = 0xff; Act a; a.PCreature.p = &c; a.Initialise();
    if (g_base != 1 || g_pound != 1 || c.flags != 0xbf) { std::printf("008c6130_TEST FAIL\n"); return 1; }
    std::printf("008c6130_TEST PASS\n");
    return 0;
}
