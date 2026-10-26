// behaviour test for 0x008170d0 _Push_heap (THitLocationIntersectionElement, `<` on key)
#include <cstdio>
#include <cstring>
struct CHitLocation;
class C3DVector { public: float X; float Y; float Z; };
struct THitLocationIntersectionElement {
    int key;
    CHitLocation* hit_location;
    C3DVector intersection_pos;
    C3DVector outward_normal;
};
struct THitLocationIntersectionElementPred {
    char m_tag;
    bool operator()(const THitLocationIntersectionElement& a, const THitLocationIntersectionElement& b) const { return a.key < b.key; }
};
static void _Push_heap(THitLocationIntersectionElement* first, int hole, int top, THitLocationIntersectionElement value, THitLocationIntersectionElementPred comp)
{
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static int check(THitLocationIntersectionElement* h, int n)
{
    THitLocationIntersectionElementPred c;
    for (int i = 1; i < n; ++i)
        if (c(h[(i - 1) / 2], h[i])) return 0;   // heap property: no parent orders before its child
    return 1;
}
int main()
{
    static const int keys[8] = { 5, 3, 9, 1, 7, 2, 8, 4 };
    THitLocationIntersectionElement heap[8];
    std::memset(heap, 0, sizeof(heap));
    THitLocationIntersectionElementPred c;
    c.m_tag = 0;
    for (int i = 0; i < 8; ++i) {
        THitLocationIntersectionElement v;
        std::memset(&v, 0, sizeof(v));
        v.key = (long)keys[i];
        v.intersection_pos.Y = i + 1;   // payload marker travels with the key
        _Push_heap(heap, i, 0, v, c);
        if (!check(heap, i + 1)) { std::printf("008170d0_TEST FAIL\n"); return 1; }
    }
    // the top is the greatest key under `<`, the smallest under `>`
    if (heap[0].key != (long)(9)) { std::printf("008170d0_TEST FAIL\n"); return 1; }
    if (heap[0].intersection_pos.Y != 3) { std::printf("008170d0_TEST FAIL\n"); return 1; }
    // a push with top == hole stores in place
    THitLocationIntersectionElement w;
    std::memset(&w, 0, sizeof(w));
    w.key = (long)100;
    _Push_heap(heap, 7, 7, w, c);
    if (heap[7].key != (long)100) { std::printf("008170d0_TEST FAIL\n"); return 1; }
    std::printf("008170d0_TEST PASS\n");
    return 0;
}