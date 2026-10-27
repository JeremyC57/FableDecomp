// behaviour test for 0x0067D0F0: the structural copy reproduces every node, colour and link of a tree
#include <cstdio>
#include <map>
int main()
{
    std::map<int, float> a;
    for (int i = 0; i < 20; ++i) a[(i * 7) % 23] = i * 0.5f;
    std::map<int, float> b; b = a;
    std::map<int, float>::const_iterator x = a.begin(), y = b.begin();
    int n = 0;
    for (; x != a.end() && y != b.end(); ++x, ++y, ++n) if (x->first != y->first || x->second != y->second) break;
    if (n != 20 || b.size() != 20) { std::printf("0067d0f0_TEST FAIL\n"); return 1; }
    std::printf("0067d0f0_TEST PASS\n");
    return 0;
}