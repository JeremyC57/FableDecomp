#include <cstdio>
// Behaviour model of the pair-vector map Add (0x006bf500): the pair is appended in order and the map flagged unsorted.
struct Pair { long key; long value; };
struct Map { Pair items[4]; int n; bool needsSort;
    void Add(const long& k, const long& v) { Pair p = { k, v }; items[n++] = p; needsSort = true; } };
int main()
{
    Map m; m.n = 0; m.needsSort = false;
    m.Add(5, 50); m.Add(2, 20);
    if (m.n != 2 || m.items[0].key != 5 || m.items[1].value != 20 || !m.needsSort) { std::printf("006bf500_TEST FAIL\n"); return 1; }
    std::printf("006bf500_TEST PASS\n");
    return 0;
}