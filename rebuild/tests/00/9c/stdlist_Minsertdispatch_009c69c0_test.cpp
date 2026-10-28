// Test: list<CPendingWrite>::_M_insert_dispatch -- inserts a copy of every element of [first, last) before pos, in order
#include <cstdio>
#include <cstdlib>
struct CPendingWrite { void* PBufferChunk; unsigned long FilePos; unsigned long WriteLength; };
struct Node { Node* next; Node* prev; CPendingWrite data; };
static Node* MakeList() { Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s->prev = s; return s; }
static void InsertDispatch(Node* pos, Node* first, Node* last)
{
    for ( ; first != last; first = first->next) {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = first->data;
        Node* p = pos->prev;
        tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
    }
}
int main()
{
    Node* a = MakeList(); Node* b = MakeList();
    CPendingWrite w[3] = { { 0, 10, 1 }, { 0, 20, 2 }, { 0, 30, 3 } };
    for (int i = 0; i < 3; ++i) { Node one; one.data = w[i]; Node* s = MakeList(); InsertDispatch(s, &one, &one); }
    Node src[3];
    for (int i = 0; i < 3; ++i) { src[i].data = w[i]; src[i].next = (i < 2) ? &src[i + 1] : 0; }
    Node tail; src[2].next = &tail;
    InsertDispatch(a, &src[0], &tail);                  // into an empty list
    InsertDispatch(a->next->next, &src[0], &src[1]);    // before the second element
    unsigned long expect[4] = { 10, 10, 20, 30 }; int n = 0;
    for (Node* it = a->next; it != a; it = it->next, ++n) if (n >= 4 || it->data.FilePos != expect[n]) { std::printf("009c69c0_TEST FAIL\n"); return 1; }
    InsertDispatch(b, &src[0], &src[0]);
    if (n != 4 || b->next != b) { std::printf("009c69c0_TEST FAIL\n"); return 1; }
    std::printf("009c69c0_TEST PASS\n");
    return 0;
}