// Behaviour test for 0x00440C00 std::__destroy_aux over deque iterators: one destructor per element across block boundaries
#include <cstdio>
static int g_dtors = 0;
struct Elem { int key; long value; void Destroy() { ++g_dtors; key = -1; } };
struct DequeIter {                                  // {cur, first, last, node} with 4-element blocks for the test
    Elem* cur; Elem* first; Elem* last; Elem** node;
    void SetNode(Elem** n) { node = n; first = *n; last = first + 4; }
    DequeIter& operator++() { if (++cur == last) { SetNode(node + 1); cur = first; } return *this; }
    bool operator!=(const DequeIter& o) const { return cur != o.cur; }
};
static void DestroyAux(DequeIter first, DequeIter last)
{
    for (; first != last; ++first)
        first.cur->Destroy();
}
int main()
{
    Elem b0[4], b1[4], b2[4];
    for (int i = 0; i < 4; ++i) { b0[i].key = b1[i].key = b2[i].key = i; }
    Elem* map[3] = { b0, b1, b2 };
    DequeIter first; first.SetNode(&map[0]); first.cur = b0 + 2;   // start mid-block
    DequeIter last;  last.SetNode(&map[2]);  last.cur = b2 + 1;    // end mid-block
    DestroyAux(first, last);
    if (g_dtors != 2 + 4 + 1 || b0[1].key != 1 || b0[2].key != -1 || b1[3].key != -1 || b2[0].key != -1 || b2[1].key != 1) { std::printf("00440c00_TEST FAIL\n"); return 1; }
    DestroyAux(last, last);
    if (g_dtors != 7) { std::printf("00440c00_TEST FAIL\n"); return 1; }
    std::printf("00440c00_TEST PASS\n");
    return 0;
}