// Behaviour test for 0x0086B4D0 std::__pop_heap over pair<long, CSkeletalMorph>: *result = *first, then the value is
// sifted down from the root of [first, last) (STLport __adjust_heap semantics, max-heap by key).
#include <cstdio>

struct Morph { long GroupID; long Name; bool AllowVariation; };
struct Entry { long first; Morph second; };

static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }

static void PushHeap(Entry* first, int hole, int top, Entry val)
{
    int parent = (hole - 1) / 2;
    while (hole > top && Less(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void AdjustHeap(Entry* first, int hole, int len, Entry val)
{
    int top = hole, second = 2 * hole + 2;
    while (second < len) {
        if (Less(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    PushHeap(first, hole, top, val);
}
static void PopHeap(Entry* first, Entry* last, Entry* result, Entry val)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), val);
}

static Entry Make(long k) { Entry e; e.first = k; e.second.GroupID = k * 10; e.second.Name = k; e.second.AllowVariation = (k & 1) != 0; return e; }

int main()
{
    Entry h[5] = { Make(9), Make(7), Make(8), Make(3), Make(5) };   // a max-heap
    Entry out = Make(0);
    Entry val = h[4];
    PopHeap(h, h + 4, &out, val);                                   // as __pop_heap_aux would call it
    if (out.first != 9 || out.second.GroupID != 90) { std::printf("0086b4d0_TEST FAIL\n"); return 1; }
    if (h[0].first != 8) { std::printf("0086b4d0_TEST FAIL\n"); return 1; }
    for (int i = 1; i < 4; ++i)
        if (h[(i - 1) / 2].first < h[i].first) { std::printf("0086b4d0_TEST FAIL\n"); return 1; }
    std::printf("0086b4d0_TEST PASS\n");
    return 0;
}