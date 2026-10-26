// behaviour test for 0x0055CF50 std::list::list(const list&) [list<NUISystem::CObserver*>]: a fresh self-linked sentinel, then every element of x copied
// in order before begin() of the (empty) new list; an empty source gives an empty list; the source is left untouched
#include <cstdio>
#include <cstdlib>
struct Elem { int v[1]; };
struct Node { Node* next; Node* prev; Elem data; };
struct List {
    Node* node;
    List() { node = (Node*)std::malloc(sizeof(Node)); node->next = node; node->prev = node; }
    void InsertBefore(Node* pos, const Elem& e)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        tmp->data = e;
        tmp->next = pos;
        tmp->prev = pos->prev;
        pos->prev->next = tmp;
        pos->prev = tmp;
    }
    List(const List& x)                                  // the function under test: sentinel, then insert(begin(), x.begin(), x.end())
    {
        node = (Node*)std::malloc(sizeof(Node));
        node->next = node;
        node->prev = node;
        Node* pos = node->next;
        for (Node* f = x.node->next; f != x.node; f = f->next)
            InsertBefore(pos, f->data);
    }
    int Size() const { int n = 0; for (Node* p = node->next; p != node; p = p->next) ++n; return n; }
};
static Elem Make(int k) { Elem e; for (int i = 0; i < 1; ++i) e.v[i] = k * 10 + i; return e; }
int main()
{
    int fail = 0;
    List a;
    for (int k = 1; k <= 4; ++k) a.InsertBefore(a.node, Make(k));
    List b(a);
    if (b.node == a.node || b.Size() != 4 || a.Size() != 4) fail |= 1;
    int k = 1;
    for (Node* p = b.node->next; p != b.node; p = p->next, ++k)
        for (int i = 0; i < 1; ++i)
            if (p->data.v[i] != k * 10 + i) fail |= 2;
    Node* back = b.node->prev;
    if (back->data.v[0] != 40 || back->next != b.node) fail |= 4;
    List e;
    List f(e);
    if (f.node->next != f.node || f.node->prev != f.node) fail |= 8;
    if (fail) { std::printf("0055cf50_TEST FAIL %d\n", fail); return 1; }
    std::printf("0055cf50_TEST PASS\n");
    return 0;
}