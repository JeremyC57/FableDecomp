// behaviour test for 0x007fa320 std::__adjust_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2>:
// STLport sift-down: the hole moves to the better child until a leaf, then the value is pushed back up; with the
// targeting comparator (less = worse score) a rebuilt heap keeps its best-scored thing on top
#include <cstdio>
struct Thing { float score; };
struct Compare { bool operator()(const Thing* a, const Thing* b) const { return a->score < b->score; } };
static void push_heap_(Thing** first, int hole, int top, Thing* val, Compare comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], val)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = val;
}
static void adjust_heap(Thing** first, int hole, int len, Thing* val, Compare comp)
{
    int top = hole; int second = 2 * hole + 2;
    while (second < len) {
        if (comp(first[second], first[second - 1])) second--;
        first[hole] = first[second]; hole = second; second = 2 * (second + 1);
    }
    if (second == len) { first[hole] = first[second - 1]; hole = second - 1; }
    push_heap_(first, hole, top, val, comp);
}
int main()
{
    int fails = 0;
    Thing t[7] = { {3}, {9}, {1}, {7}, {5}, {8}, {2} };
    Thing* h[7]; for (int i = 0; i < 7; ++i) h[i] = &t[i];
    for (int p = (7 - 2) / 2; p >= 0; --p) adjust_heap(h, p, 7, h[p], Compare());
    for (int i = 1; i < 7; ++i) if (h[(i - 1) / 2]->score < h[i]->score) fails |= 1;
    if (h[0]->score != 9) fails |= 2;
    float last = 1e9f;   // pop everything: scores come out best first
    for (int n = 7; n > 0; --n) {
        Thing* top = h[0]; Thing* v = h[n - 1]; h[n - 1] = top; adjust_heap(h, 0, n - 1, v, Compare());
        if (top->score > last) fails |= 4; last = top->score;
    }
    if (fails) { std::printf("007fa320_TEST FAIL %d\n", fails); return 1; }
    std::printf("007fa320_TEST PASS\n");
    return 0;
}