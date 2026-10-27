#include <cstdio>
#include <vector>
// Behaviour model: the tail moves down over the erased range and the size shrinks by its length.
int main()
{
    std::vector<int> v; for (int i = 0; i < 6; ++i) v.push_back(i);
    std::vector<int>::iterator r = v.erase(v.begin() + 1, v.begin() + 3);
    if (v.size() != 4 || *r != 3 || v[0] != 0 || v[3] != 5) { std::printf("00f34cc0_TEST FAIL\n"); return 1; }
    std::printf("00f34cc0_TEST PASS\n");
    return 0;
}