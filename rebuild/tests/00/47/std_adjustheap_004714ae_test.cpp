// Behaviour test for 0x004714ae __adjust_heap<pair<unsigned long, CDefString>*>: sift a hole to the bottom, push the value
// back up; driving it as pop_heap repeatedly yields keys in descending order (a max-heap on the unsigned key)
#include <cstdio>
class CDefString { public: int TablePos; };
struct Entry { unsigned int first; CDefString second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static void PushHeap(Entry* first, int holeIndex, int topIndex, Entry value, Less comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(first[parent], value)) { first[holeIndex] = first[parent]; holeIndex = parent; parent = (holeIndex - 1) / 2; }
    first[holeIndex] = value;
}
static void AdjustHeap(Entry* first, int holeIndex, int len, Entry value, Less comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(first[secondChild], first[secondChild - 1])) secondChild--;
        first[holeIndex] = first[secondChild];
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) { first[holeIndex] = first[secondChild - 1]; holeIndex = secondChild - 1; }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
int main()
{
    unsigned int keys[11] = { 5, 17, 3, 0xfffffff0u, 9, 9, 1, 42, 8, 30, 2 };
    Entry v[11];
    for (int i = 0; i < 11; ++i) { v[i].first = keys[i]; v[i].second.TablePos = (int)keys[i]; }
    for (int parent = (11 - 2) / 2; parent >= 0; --parent) AdjustHeap(v, parent, 11, v[parent], Less());   // make_heap
    bool ok = v[0].first == 0xfffffff0u;
    unsigned int prev = 0xffffffffu;
    for (int len = 11; len > 0; --len) {                                                             // sort_heap
        Entry top = v[0];
        if (top.first > prev || top.second.TablePos != (int)top.first) ok = false;
        prev = top.first;
        Entry last = v[len - 1]; v[len - 1] = top;
        AdjustHeap(v, 0, len - 1, last, Less());
    }
    for (int i = 1; i < 11; ++i) if (v[i - 1].first > v[i].first) ok = false;
    if (!ok) { std::printf("004714ae_TEST FAIL\n"); return 1; }
    std::printf("004714ae_TEST PASS\n");
    return 0;
}