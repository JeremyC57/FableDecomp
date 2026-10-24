// Behaviour test for 0x00810D40 list<pair<CIntelligentPointer<CThing>,long> >::push_back (model copy: insert(end(), x)).
#include <cstdio>
#include <cstdlib>
#include <new>
struct CThing { int id; };
struct IP { int vptr_slot; CThing* PData; };
struct P { IP first; long second; };
struct Node { Node* next; Node* prev; P data; };
struct It { Node* node; It(Node* n) : node(n) { } It(const It& o) : node(o.node) { } };
struct List {
    Node* node;
    It end() { return It(node); }
    It insert(It pos, const P& x)
    {
        Node* n = (Node*)std::malloc(sizeof(Node));
        new (&n->data) P(x);
        n->next = pos.node; n->prev = pos.node->prev;
        pos.node->prev->next = n; pos.node->prev = n;
        return It(n);
    }
    void push_back(const P& x) { insert(end(), x); }
};
int main()
{
    Node sentinel; sentinel.next = &sentinel; sentinel.prev = &sentinel;
    List l; l.node = &sentinel;
    CThing t[3];
    int i, ok = 1;
    for (i = 0; i < 3; ++i) { P p; p.first.vptr_slot = 0; p.first.PData = &t[i]; p.second = 100 + i; l.push_back(p); }
    Node* n = sentinel.next;
    for (i = 0; i < 3; ++i) { if (n == &sentinel || n->data.first.PData != &t[i] || n->data.second != 100 + i) ok = 0; n = n->next; }
    if (n != &sentinel || sentinel.prev->data.second != 102 || sentinel.prev->next != &sentinel) ok = 0;
    n = sentinel.next;
    while (n != &sentinel) { Node* nx = n->next; std::free(n); n = nx; }
    if (!ok) { std::printf("00810d40_TEST FAIL\n"); return 1; }
    std::printf("00810d40_TEST PASS\n");
    return 0;
}