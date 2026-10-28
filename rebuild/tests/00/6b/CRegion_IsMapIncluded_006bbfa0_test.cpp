// Behaviour test for 0x006BBFA0 CRegion::IsMapIncluded: linear search of the region map list (standalone copy of the logic)
#include <cstdio>
#include <vector>
#include <algorithm>
struct Region { std::vector<long> Maps; bool IsMapIncluded(long m) const { return std::find(Maps.begin(), Maps.end(), m) != Maps.end(); } };

int main()
{
    Region r; if (r.IsMapIncluded(1)) { std::printf("006bbfa0_TEST FAIL\n"); return 1; }
    for (long i = 0; i < 7; ++i) r.Maps.push_back(i * 3);
    if (!r.IsMapIncluded(0) || !r.IsMapIncluded(18) || r.IsMapIncluded(4)) { std::printf("006bbfa0_TEST FAIL\n"); return 1; }
    std::printf("006bbfa0_TEST PASS\n");
    return 0;
}