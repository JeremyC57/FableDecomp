// Behaviour test for 0x00BEF700 std::__partial_sort over (pair<long,long>, decal) entries: the smallest keys end up sorted in [first, middle)
#include <cstdio>
#include <vector>
#include <algorithm>

struct Entry { long k1, k2; long tex, id; std::vector<int> blends; };
static bool less_(const Entry& a, const Entry& b) { return a.k1 < b.k1 || (!(b.k1 < a.k1) && a.k2 < b.k2); }

static void partial_sort_(Entry* first, Entry* middle, Entry* last)
{
    std::make_heap(first, middle, less_);
    for (Entry* i = middle; i < last; ++i)
        if (less_(*i, *first)) {
            Entry v = *i;
            *i = *first;
            std::pop_heap(first, middle, less_);    // root moves to middle-1
            *(middle - 1) = v;
            std::push_heap(first, middle, less_);
        }
    std::sort_heap(first, middle, less_);
}

int main()
{
    long keys[8][2] = { {5,1}, {2,9}, {7,0}, {2,3}, {1,4}, {9,9}, {0,8}, {2,3} };
    Entry e[8];
    for (int i = 0; i < 8; ++i) { e[i].k1 = keys[i][0]; e[i].k2 = keys[i][1]; e[i].id = i; e[i].blends.assign(i, i); }
    partial_sort_(e, e + 4, e + 8);
    long want[4][2] = { {0,8}, {1,4}, {2,3}, {2,3} };
    for (int i = 0; i < 4; ++i)
        if (e[i].k1 != want[i][0] || e[i].k2 != want[i][1] || (int)e[i].blends.size() != e[i].id) { std::printf("00bef700_TEST FAIL\n"); return 1; }
    std::printf("00bef700_TEST PASS\n");
    return 0;
}