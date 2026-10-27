// Behaviour test for 0x008FC890 std::list<CPhysicsMeshObject>::insert(pos, x): a new node holding a copy of x (mesh
// pointer shared with its count bumped, matrix and scales copied) is linked before pos and returned.
#include <cstdio>
#include <cstdlib>
#include <new>

struct Pointee { unsigned long IVRefCount; };
class MeshObject {
public:
    Pointee* Mesh; float M[12]; float Scale, OO_Scale;
    virtual ~MeshObject() { }
    MeshObject() : Mesh(0), Scale(1.0f), OO_Scale(1.0f) { for (int i = 0; i < 12; ++i) M[i] = 0.0f; }
    MeshObject(const MeshObject& o) : Mesh(o.Mesh), Scale(o.Scale), OO_Scale(o.OO_Scale)
    {
        if (Mesh) ++Mesh->IVRefCount;
        for (int i = 0; i < 12; ++i) M[i] = o.M[i];
    }
};
struct NodeBase { NodeBase* next; NodeBase* prev; };
struct Node : NodeBase { MeshObject data; };

static NodeBase* Insert(NodeBase* pos, const MeshObject& x)
{
    Node* tmp = (Node*)std::malloc(sizeof(Node));
    new (static_cast<void*>(&tmp->data)) MeshObject(x);
    NodeBase* p = pos->prev;
    tmp->next = pos; tmp->prev = p; p->next = tmp; pos->prev = tmp;
    return tmp;
}

int main()
{
    NodeBase head; head.next = head.prev = &head;
    Pointee mesh = { 1 };
    MeshObject a; a.Mesh = &mesh; a.Scale = 2.0f; a.OO_Scale = 0.5f; a.M[11] = 9.0f;
    NodeBase* n1 = Insert(&head, a);
    MeshObject b;
    NodeBase* n0 = Insert(n1, b);
    if (head.next != n0 || n0->next != n1 || n1->next != &head || head.prev != n1 || n1->prev != n0) { std::printf("008fc890_TEST FAIL\n"); return 1; }
    Node* c = static_cast<Node*>(n1);
    if (c->data.Mesh != &mesh || mesh.IVRefCount != 2 || c->data.Scale != 2.0f || c->data.OO_Scale != 0.5f || c->data.M[11] != 9.0f) { std::printf("008fc890_TEST FAIL\n"); return 1; }
    if (static_cast<Node*>(n0)->data.Mesh != 0) { std::printf("008fc890_TEST FAIL\n"); return 1; }
    static_cast<Node*>(n0)->data.~MeshObject(); std::free(n0);
    c->data.~MeshObject(); std::free(c);
    std::printf("008fc890_TEST PASS\n");
    return 0;
}