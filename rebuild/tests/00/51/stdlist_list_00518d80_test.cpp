// behaviour test for 0x00518D80 list<C2DLineF >::list(const list& x): the copy gets its own self-linked sentinel, then every element of x is inserted before the copy's end in order (an empty x leaves the sentinel linked to itself), and x is left untouched
#include <cstdio>
#include <cstdlib>
struct Node { Node* next; Node* prev; int value; };
struct List { Node* node; };
static int gCopies = 0;
static void InsertBefore(Node* pos, int v)          // list::insert(pos, *first): a fresh node linked in before pos
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->value = v; ++gCopies;
    n->next = pos;
    n->prev = pos->prev;
    pos->prev->next = n;
    pos->prev = n;
}
static void CopyConstruct(List* self, const List& x)
{
    self->node = 0;                                   // _M_node(a, (_Node*)0)
    Node* s = (Node*)std::malloc(sizeof(Node));       // allocate(1)
    s->next = s;
    s->prev = s;
    self->node = s;
    Node* pos = self->node->next;                     // begin()
    for (Node* f = x.node->next; f != x.node; f = f->next)   // _M_insert_dispatch(pos, x.begin(), x.end(), __false_type())
        InsertBefore(pos, f->value);
}
static void Fill(List& l, int n)
{
    Node* s = (Node*)std::malloc(sizeof(Node)); s->next = s; s->prev = s; l.node = s;
    for (int k = 0; k < n; ++k) InsertBefore(s, 100 + k);
}
static int Count(const List& l, int base)
{
    int k = 0;
    for (Node* p = l.node->next; p != l.node; p = p->next, ++k) {
        if (p->value != base + k || p->next->prev != p) return -1;
    }
    return k;
}
static void Free(List& l)
{
    Node* p = l.node->next;
    while (p != l.node) { Node* q = p->next; std::free(p); p = q; }
    std::free(l.node);
}
int main()
{
    int fails = 0;
    List a, b, e, c;
    Fill(a, 5); gCopies = 0;
    CopyConstruct(&b, a);
    if (b.node == a.node) fails |= 1;
    if (Count(b, 100) != 5 || gCopies != 5) fails |= 2;
    if (Count(a, 100) != 5) fails |= 4;
    if (b.node->prev->value != 104 || b.node->prev->next != b.node) fails |= 8;
    Fill(e, 0); gCopies = 0;
    CopyConstruct(&c, e);
    if (c.node->next != c.node || c.node->prev != c.node || gCopies != 0) fails |= 16;
    Free(a); Free(b); Free(e); Free(c);
    if (fails) { std::printf("00518d80_TEST FAIL %d\n", fails); return 1; }
    std::printf("00518d80_TEST PASS\n");
    return 0;
}