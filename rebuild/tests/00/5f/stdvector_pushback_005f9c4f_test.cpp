// behaviour test for 0x005F9C4F: push_back appends pairs in order, growing when full
#include <cstdio>
#include <vector>
#include <utility>
int main()
{
    std::vector<std::pair<long, long> > v;
    for (long i = 0; i < 5; ++i) v.push_back(std::make_pair(i, i * 10));
    if (v.size() != 5 || v[4].second != 40 || v[2].first != 2) { std::printf("005f9c4f_TEST FAIL\n"); return 1; }
    std::printf("005f9c4f_TEST PASS\n");
    return 0;
}