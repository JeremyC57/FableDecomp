#include <cstdio>
#include <vector>
// Behaviour model: a growth insert keeps the old elements in order, inserts fill_len copies, and doubles the capacity.
int main()
{
    std::vector<std::vector<int> > v(2, std::vector<int>(1, 7));
    v.reserve(2);
    std::vector<int> x(3, 5);
    v.insert(v.begin() + 1, 2, x);
    if (v.size() != 4 || v[0][0] != 7 || v[1].size() != 3 || v[2][2] != 5 || v[3][0] != 7) { std::printf("00afe6f0_TEST FAIL\n"); return 1; }
    std::printf("00afe6f0_TEST PASS\n");
    return 0;
}