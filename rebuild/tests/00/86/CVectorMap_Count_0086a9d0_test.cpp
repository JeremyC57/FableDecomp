// Behaviour test for 0x0086A9D0: CVectorMap::Count = UpperBound(key) - LowerBound(key) over a sorted 16-byte pair array.
#include <cstdio>

struct SSkeletalMorph { int GroupID; unsigned char _rest[8]; };
struct SPair { int first; SSkeletalMorph second; };

class CSkeletalMorphMap
{
public:
    SPair* _M_start;
    SPair* _M_finish;
    SPair* _M_end_of_storage;
    const SPair* LowerBound(const int& key) const
    {
        const SPair* it = _M_start;
        while (it != _M_finish && it->first < key) ++it;
        return it;
    }
    const SPair* UpperBound(const int& key) const
    {
        const SPair* it = _M_start;
        while (it != _M_finish && !(key < it->first)) ++it;
        return it;
    }
    int Count(const int& key) const { return UpperBound(key) - LowerBound(key); }
};

int main()
{
    SPair items[6];
    int keys[6] = { 1, 3, 3, 3, 7, 9 };
    for (int i = 0; i < 6; ++i) { items[i].first = keys[i]; items[i].second.GroupID = i; }
    CSkeletalMorphMap map; map._M_start = items; map._M_finish = items + 6; map._M_end_of_storage = items + 6;
    int fails = 0;
    if (sizeof(SPair) != 16u) ++fails;
    int k;
    k = 3; if (map.Count(k) != 3) ++fails;
    k = 1; if (map.Count(k) != 1) ++fails;
    k = 5; if (map.Count(k) != 0) ++fails;
    k = 10; if (map.Count(k) != 0) ++fails;
    k = 0; if (map.Count(k) != 0) ++fails;
    if (fails) { std::printf("0086a9d0_TEST FAIL\n"); return 1; }
    std::printf("0086a9d0_TEST PASS\n");
    return 0;
}