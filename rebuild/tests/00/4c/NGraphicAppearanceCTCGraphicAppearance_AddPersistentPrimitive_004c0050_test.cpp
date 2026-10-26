// Behaviour test for 0x004C0050 CTCGraphicAppearance::AddPersistentPrimitive: a copy of the primitive (type, handle
// with its counted object add-ref'ed, bank index) is linked at the back of the Primitives list; the stored copy is returned.
#include <cstdio>
#include <cstdlib>
#include <new>

struct CPrimitiveObject { long RefCount; };
class CEnginePrimitiveHandle { public:
    CPrimitiveObject* Data;
    CEnginePrimitiveHandle() : Data(0) {}
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& o) : Data(o.Data) { if (Data) ++Data->RefCount; }
};
struct CPrimitive { long Type; CEnginePrimitiveHandle PrimitiveHandle; long BankIndex; };

struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { CPrimitive data; };

struct CTCGraphicAppearance {
    NodeBase* Primitives;                          // the list sentinel
    CPrimitive* AddPersistentPrimitive(const CPrimitive& primitive)
    {
        NodeBase* n = Primitives;
        Node* tmp = static_cast<Node*>(std::malloc(sizeof(Node)));
        if (&tmp->data) new (&tmp->data) CPrimitive(primitive);
        NodeBase* p = n->prev;
        tmp->next = n; tmp->prev = p; p->next = tmp; n->prev = tmp;
        return &static_cast<Node*>(Primitives->prev)->data;
    }
};

int main()
{
    NodeBase sentinel; sentinel.next = sentinel.prev = &sentinel;
    CTCGraphicAppearance app; app.Primitives = &sentinel;
    CPrimitiveObject obj = { 1 };
    CPrimitive a; a.Type = 3; a.PrimitiveHandle.Data = &obj; a.BankIndex = 7;
    CPrimitive* r1 = app.AddPersistentPrimitive(a);
    CPrimitive b; b.Type = 5; b.BankIndex = 9;
    CPrimitive* r2 = app.AddPersistentPrimitive(b);
    if (r1 == &a || r1->Type != 3 || r1->BankIndex != 7 || r1->PrimitiveHandle.Data != &obj || obj.RefCount != 2
        || r2->Type != 5 || r2->PrimitiveHandle.Data != 0 || sentinel.next->next->prev != sentinel.next
        || &static_cast<Node*>(sentinel.next)->data != r1 || &static_cast<Node*>(sentinel.prev)->data != r2) {
        std::printf("004c0050_TEST FAIL\n"); return 1;
    }
    std::free(sentinel.next->next); std::free(sentinel.next);
    std::printf("004c0050_TEST PASS\n");
    return 0;
}