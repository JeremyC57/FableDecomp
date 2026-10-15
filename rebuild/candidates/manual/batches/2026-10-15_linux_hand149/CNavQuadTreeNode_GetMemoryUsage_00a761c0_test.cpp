#include <cstdio>
struct Node {
    Node* TL; Node* TR; Node* BL; Node* BR; unsigned int leafSize;
    unsigned int GetMemoryUsage(unsigned int& q, unsigned int& l, unsigned int& b, unsigned int& t) const
    {
        if (!TL) { ++l; ++t; return leafSize; }
        ++q; ++t;
        unsigned int usage = 0x20 + TR->GetMemoryUsage(q, l, b, t);
        usage += TL->GetMemoryUsage(q, l, b, t);
        usage += BL->GetMemoryUsage(q, l, b, t);
        usage += BR->GetMemoryUsage(q, l, b, t);
        return usage;
    }
};
int main()
{
    Node leaf[4]; for (int i = 0; i < 4; ++i) { leaf[i].TL = leaf[i].TR = leaf[i].BL = leaf[i].BR = 0; leaf[i].leafSize = 10 + i; }
    Node root; root.TL = &leaf[0]; root.TR = &leaf[1]; root.BL = &leaf[2]; root.BR = &leaf[3];
    unsigned int q = 0, l = 0, b = 0, t = 0;
    unsigned int u = root.GetMemoryUsage(q, l, b, t);
    if (u != 0x20 + 46 || q != 1 || l != 4 || t != 5 || b != 0) { std::printf("00a761c0_TEST FAIL\n"); return 1; }
    std::printf("00a761c0_TEST PASS\n");
    return 0;
}
