// behaviour test for 0x004D16A0 std::sort_heap over (time, camera tag) pairs (STLport pop_heap loop with __adjust_heap)
#include <cstdio>
struct Tag { int Person, Position, FocusPerson, Focus, Zoom, View; };
struct Entry { float first; Tag second; };
struct Less { bool operator()(const Entry& a, const Entry& b) const { return a.first < b.first; } };
static void push_heap(Entry* first, int hole, int top, Entry val, Less comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void adjust_heap(Entry* first, int hole, int len, Entry val, Less comp)
{
    int top = hole, child = 2 * hole + 2;
    while (child < len) {
        if (comp(first[child], first[child - 1])) child--;
        first[hole] = first[child]; hole = child; child = 2 * (child + 1);
    }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    push_heap(first, hole, top, val, comp);
}
static void sort_heap(Entry* first, Entry* last, Less comp)
{
    while (last - first > 1) {
        Entry val = *(last - 1);
        *(last - 1) = *first;
        adjust_heap(first, 0, (int)(last - 1 - first), val, comp);
        --last;
    }
}
int main()
{
    Entry e[9];
    const float t[9] = { 4.5f, 1.0f, 8.0f, 2.5f, 9.0f, 0.5f, 7.0f, 3.0f, 6.0f };
    for (int i = 0; i < 9; ++i) { e[i].first = t[i]; e[i].second.View = i; e[i].second.Person = (int)(t[i] * 2); }
    for (int len = 2; len <= 9; ++len) push_heap(e, len - 1, 0, e[len - 1], Less());   // make_heap
    sort_heap(e, e + 9, Less());
    bool ok = true;
    for (int i = 1; i < 9; ++i) ok = ok && e[i - 1].first <= e[i].first;
    for (int i = 0; i < 9; ++i) ok = ok && e[i].second.Person == (int)(e[i].first * 2);   // tags travel with their keys
    std::printf(ok ? "004d16a0_TEST PASS\n" : "004d16a0_TEST FAIL\n");
    return ok ? 0 : 1;
}