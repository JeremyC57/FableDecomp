// Portable test for 0x005BB3EF std::list<CDefPointer<const CAppearanceModifierDef> >::push_back(const T&): insert(end(), x).
#include <stdio.h>
#include <stdlib.h>
#include <new>

struct Pointee { void* vtbl; unsigned long RefCount; };
struct DefPtr {
    Pointee* Object;
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    explicit DefPtr(Pointee* p) : Object(p) {}
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { DefPtr data; };
struct Iter { NodeBase* node; };

struct List {
    NodeBase* header;                                               // _M_node._M_data
    Iter end() { Iter i; i.node = header; return i; }
    Iter insert(Iter position, const DefPtr& x) {                   // 0x005BB0A7
        Node* tmp = (Node*)malloc(sizeof(Node));
        new ((void*)&tmp->data) DefPtr(x);
        NodeBase* n = position.node; NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        Iter r; r.node = tmp; return r;
    }
    void push_back(const DefPtr& x) { insert(end(), x); }           // 0x005BB3EF
};

int main() {
    NodeBase h; h.next = &h; h.prev = &h;
    List l; l.header = &h;
    Pointee a; a.vtbl = 0; a.RefCount = 1;
    Pointee b; b.vtbl = 0; b.RefCount = 5;
    DefPtr pa(&a), pb(&b);
    l.push_back(pa);
    l.push_back(pb);
    Node* first = (Node*)h.next; Node* second = (Node*)h.prev;
    if (first->data.Object != &a || second->data.Object != &b || first->next != second || second->prev != first || second->next != &h) { printf("FAIL order\n"); return 1; }
    if (a.RefCount != 2 || b.RefCount != 6) { printf("FAIL refcount\n"); return 1; }
    free(first); free(second);
    printf("005bb3ef_TEST PASS\n");
    return 0;
}
