// Portable test for 0x005BB0A7 std::list<CDefPointer<const CAppearanceModifierDef> >::insert(iterator, const T&):
// _M_create_node copies the counted pointer (AddRef), the node is linked before position, the new node's iterator is returned.
#include <stdio.h>
#include <stdlib.h>
#include <new>

struct Pointee { void* vtbl; unsigned long RefCount; };            // CDefPointeeBase: count at +4
struct DefPtr {
    Pointee* Object;
    DefPtr(const DefPtr& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    explicit DefPtr(Pointee* p) : Object(p) {}
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { DefPtr data; };
struct Iter { NodeBase* node; };

static Node* create_node(const DefPtr& x) {                         // 0x005BAC52
    Node* p = (Node*)malloc(sizeof(Node));
    new ((void*)&p->data) DefPtr(x);
    return p;
}
static Iter insert(Iter position, const DefPtr& x) {               // 0x005BB0A7
    Node* tmp = create_node(x);
    NodeBase* n = position.node;
    NodeBase* p = n->prev;
    tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
    Iter r; r.node = tmp; return r;
}

int main() {
    NodeBase header; header.next = &header; header.prev = &header;
    Pointee def; def.vtbl = 0; def.RefCount = 1;
    DefPtr a(&def);
    Iter end; end.node = &header;
    Iter r1 = insert(end, a);                                      // push_back
    if (def.RefCount != 2 || header.next != r1.node || header.prev != r1.node || r1.node->next != &header || r1.node->prev != &header) { printf("FAIL first\n"); return 1; }
    DefPtr nul((Pointee*)0);
    Iter r2 = insert(r1, nul);                                     // before the first node
    if (header.next != r2.node || r2.node->next != r1.node || r1.node->prev != r2.node || ((Node*)r2.node)->data.Object != 0) { printf("FAIL second\n"); return 1; }
    if (((Node*)r1.node)->data.Object != &def || def.RefCount != 2) { printf("FAIL data\n"); return 1; }
    free(r1.node); free(r2.node);
    printf("005bb0a7_TEST PASS\n");
    return 0;
}
