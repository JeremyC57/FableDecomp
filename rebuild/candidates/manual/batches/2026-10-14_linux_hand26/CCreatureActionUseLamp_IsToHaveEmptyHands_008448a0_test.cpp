#include <cstdio>
struct Thing { unsigned char pad[0x34]; unsigned long bits5; unsigned char pad2[0x91 - 0x38]; unsigned char flags; Thing() : bits5(0), flags(0) {} };
struct Ptr { virtual ~Ptr() {} Thing* p; Thing* GetPItem() const { return p; } };
inline bool ALIVE(const Ptr& p) { if (p.GetPItem() != 0) { if (!(p.GetPItem()->flags & 1)) return true; } return false; }
struct Act { virtual ~Act() {} Ptr PCreature; unsigned char IsToHaveEmptyHands() { if (ALIVE(PCreature)) { if (PCreature.GetPItem()->bits5 & 0x100000) return 0; } return 1; } };
int main()
{
    Thing t; Act a; a.PCreature.p = 0;
    if (a.IsToHaveEmptyHands() != 1) { std::printf("008448a0_TEST FAIL\n"); return 1; }
    a.PCreature.p = &t;
    if (a.IsToHaveEmptyHands() != 1) { std::printf("008448a0_TEST FAIL\n"); return 1; }
    t.bits5 = 0x100000;
    if (a.IsToHaveEmptyHands() != 0) { std::printf("008448a0_TEST FAIL\n"); return 1; }
    t.flags = 1;
    if (a.IsToHaveEmptyHands() != 1) { std::printf("008448a0_TEST FAIL\n"); return 1; }
    std::printf("008448a0_TEST PASS\n");
    return 0;
}
