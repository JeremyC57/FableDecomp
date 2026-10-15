#include <cstdio>
struct L {
    struct N { N* prev; N* next; void* data; };
    N* head; N* tail; long count; long maxFree; long freeCount; N* freeList;
    void* Remove(N* node)
    {
        if (!node) return 0;
        if (node->prev == 0) head = node->next; else node->prev->next = node->next;
        if (node->next == 0) tail = node->prev; else node->next->prev = node->prev;
        void* data = node->data;
        if (freeCount < maxFree) { node->next = freeList; freeList = node; ++freeCount; }
        else delete node;
        --count;
        return data;
    }
};
int main()
{
    int a, b, c;
    L l; L::N* n1 = new L::N; L::N* n2 = new L::N; L::N* n3 = new L::N;
    n1->prev = 0; n1->next = n2; n1->data = &a; n2->prev = n1; n2->next = n3; n2->data = &b; n3->prev = n2; n3->next = 0; n3->data = &c;
    l.head = n1; l.tail = n3; l.count = 3; l.maxFree = 1; l.freeCount = 0; l.freeList = 0;
    int ok = l.Remove(0) == 0;
    ok = ok && l.Remove(n2) == &b && n1->next == n3 && n3->prev == n1 && l.freeList == n2 && l.freeCount == 1 && l.count == 2;
    ok = ok && l.Remove(n1) == &a && l.head == n3 && l.freeCount == 1 && l.count == 1;
    ok = ok && l.Remove(n3) == &c && l.head == 0 && l.tail == 0 && l.count == 0;
    delete n2;
    if (!ok) { std::printf("00cb13f0_TEST FAIL\n"); return 1; }
    std::printf("00cb13f0_TEST PASS\n");
    return 0;
}
