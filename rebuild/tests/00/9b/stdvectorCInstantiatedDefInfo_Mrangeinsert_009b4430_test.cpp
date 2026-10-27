#include <cstdio>
#include <vector>
// Behaviour model: inserting a range in the middle keeps order, with and without reallocation.
int main()
{
    std::vector<long> v; v.reserve(10);
    for (long i = 0; i < 4; ++i) v.push_back(i);
    long r[3] = { 10, 11, 12 };
    v.insert(v.begin() + 1, r, r + 3);
    if (v.size() != 7 || v[1] != 10 || v[3] != 12 || v[4] != 1 || v[6] != 3) { std::printf("009b4430_TEST FAIL\n"); return 1; }
    std::vector<long> w(2, 5);
    w.insert(w.end(), r, r + 3);
    if (w.size() != 5 || w[2] != 10 || w[4] != 12) { std::printf("009b4430_TEST FAIL\n"); return 1; }
    std::printf("009b4430_TEST PASS\n");
    return 0;
}