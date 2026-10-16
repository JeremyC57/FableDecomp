#include <cstdio>
// Behaviour model of the __pop_heap instantiation at 0x00CBBA60: *result = *first, then __adjust_heap(first, 0, last - first, value).
struct Block { int refs; };
struct Pair {
    unsigned long long key; Block* b;
    Pair() : key(0), b(0) {}
    Pair(unsigned long long k, Block* bl) : key(k), b(bl) { if (b) ++b->refs; }
    Pair(const Pair& o) : key(o.key), b(o.b) { if (b) ++b->refs; }
    ~Pair() { if (b) --b->refs; }
    Pair& operator=(const Pair& o) { key = o.key; if (b != o.b) { if (b) --b->refs; b = o.b; if (b) ++b->refs; } return *this; }
};
static void adjust_heap(Pair* first, long hole, long len, Pair value)
{
    long top = hole, child = 2 * hole + 2;
    while (child < len) { if (first[child].key < first[child - 1].key) --child; first[hole] = first[child]; hole = child; child = 2 * child + 2; }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    while (hole > top) { long parent = (hole - 1) / 2; if (!(first[parent].key < value.key)) break; first[hole] = first[parent]; hole = parent; }
    first[hole] = value;
}
static void pop_heap(Pair* first, Pair* last, Pair* result, Pair value) { *result = *first; adjust_heap(first, 0, long(last - first), value); }
int main()
{
    Block b1 = { 0 }, b2 = { 0 }, b3 = { 0 };
    {
        Pair h[3]; h[0] = Pair(9, &b1); h[1] = Pair(5, &b2); h[2] = Pair(7, &b3);
        Pair v = h[2];
        pop_heap(h, h + 2, h + 2, v);
        if (h[2].key != 9 || h[0].key != 7 || h[1].key != 5) { std::printf("00cbba60_TEST FAIL\n"); return 1; }
    }
    if (b1.refs != 0 || b2.refs != 0 || b3.refs != 0) { std::printf("00cbba60_TEST FAIL\n"); return 1; }
    std::printf("00cbba60_TEST PASS\n");
    return 0;
}
