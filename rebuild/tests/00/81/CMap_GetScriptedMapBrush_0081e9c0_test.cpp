// Behaviour test for 0x0081E9C0 CMap::GetScriptedMapBrush(long): the index-th element of the brush list
// (the past-the-end `throw;` is not exercised: with no exception in flight it terminates).
#include <cstdio>

struct Node { Node* next; Node* prev; int brush; };

struct BrushList {
    Node head;
    Node items[4];
    void Build(int n)
    {
        head.next = head.prev = &head;
        for (int i = 0; i < n; ++i) {
            Node* p = &items[i];
            p->brush = 100 + i;
            p->next = &head; p->prev = head.prev;
            head.prev->next = p; head.prev = p;
        }
    }
    int* Get(long index)
    {
        long i = 0;
        for (Node* it = head.next; it != &head; it = it->next, ++i)
            if (i == index)
                return &it->brush;
        return 0;
    }
};

int main()
{
    BrushList l;
    l.Build(4);
    for (long i = 0; i < 4; ++i) {
        int* b = l.Get(i);
        if (!b || *b != 100 + i) { std::printf("0081e9c0_TEST FAIL\n"); return 1; }
    }
    if (l.Get(0) != &l.items[0].brush) { std::printf("0081e9c0_TEST FAIL\n"); return 1; }
    std::printf("0081e9c0_TEST PASS\n");
    return 0;
}