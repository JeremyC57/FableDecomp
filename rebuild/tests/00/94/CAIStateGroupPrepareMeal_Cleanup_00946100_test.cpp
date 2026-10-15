#include <cstdio>
// PrepareMeal::Cleanup: free the claimed action point of the living cooking pot
struct Thing { unsigned char flags91; long freed; void FreeActionPoint(long i) { freed = i; } };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } };
struct Group {
    Ptr pot; long actionPointIndex;
    void Cleanup() { if (actionPointIndex != -1) { if (pot.GetPItem() != 0 && !(pot.GetPItem()->flags91 & 1)) pot.GetPItem()->FreeActionPoint(actionPointIndex); } }
};
int main()
{
    Thing t; t.flags91 = 0; t.freed = -5; Group g; g.pot.item = &t; g.actionPointIndex = 3;
    g.Cleanup(); if (t.freed != 3) { std::printf("00946100_TEST FAIL\n"); return 1; }
    t.freed = -5; g.actionPointIndex = -1; g.Cleanup(); if (t.freed != -5) { std::printf("00946100_TEST FAIL\n"); return 1; }
    g.actionPointIndex = 2; t.flags91 = 1; g.Cleanup(); if (t.freed != -5) { std::printf("00946100_TEST FAIL\n"); return 1; }
    t.flags91 = 0; g.pot.item = 0; g.Cleanup(); if (t.freed != -5) { std::printf("00946100_TEST FAIL\n"); return 1; }
    std::printf("00946100_TEST PASS\n"); return 0;
}