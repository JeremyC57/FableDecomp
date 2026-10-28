// Behaviour test for 0x0064DD4D NPlayerGui::CDrawSpiritTargetingCursor ctor: four null targeted-thing slots (standalone copy of the logic)
#include <cstdio>
#include <vector>
struct Ptr { void* P; Ptr(void* p = 0) : P(p) { } };
struct Cursor { std::vector<Ptr> Targets; Cursor() { Ptr none; Targets.resize(4, none); } };

int main()
{
    Cursor c; if (c.Targets.size() != 4) { std::printf("0064dd4d_TEST FAIL\n"); return 1; }
    for (unsigned i = 0; i < c.Targets.size(); ++i) if (c.Targets[i].P) { std::printf("0064dd4d_TEST FAIL\n"); return 1; }
    std::printf("0064dd4d_TEST PASS\n");
    return 0;
}