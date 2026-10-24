// Behaviour test for 0x004c43b0 _Rb_tree<...>::erase(iterator): the node is unlinked through _Rebalance_for_erase(node,
// root, leftmost, rightmost), the value's counted pointer is released (destroying the object on its last reference)
// and cleared, the node is freed and the count decremented.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_destroyed = 0;
struct CCounted { long RefCount; void Destroy() { ++g_destroyed; } void Release() { if (--RefCount == 0) Destroy(); } };
struct CCountedPtr { CCounted* Object; ~CCountedPtr() { if (Object) { Object->Release(); Object = 0; } } };
struct Value { void* first; CCountedPtr second; long priority; };
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Node : NodeBase { Value value; };

static NodeBase* g_unlinked = 0; static NodeBase** g_args[3];
static NodeBase* Rebalance_for_erase(NodeBase* z, NodeBase*& root, NodeBase*& lm, NodeBase*& rm)
{ g_unlinked = z; g_args[0] = &root; g_args[1] = &lm; g_args[2] = &rm; return z; }

struct Tree {
    NodeBase* header; unsigned int count;
    void erase(NodeBase* position)
    {
        Node* y = static_cast<Node*>(Rebalance_for_erase(position, header->parent, header->left, header->right));
        y->value.~Value();
        if (y) std::free(y);
        --count;
    }
};

int main()
{
    NodeBase header = { false, 0, 0, 0 };
    CCounted obj = { 2 };
    Node* a = new (std::malloc(sizeof(Node))) Node(); a->value.second.Object = &obj;
    Node* b = new (std::malloc(sizeof(Node))) Node(); b->value.second.Object = &obj;
    header.parent = a; header.left = a; header.right = b;
    Tree t = { &header, 2 };
    t.erase(a);
    if (g_unlinked != a || g_args[0] != &header.parent || g_args[1] != &header.left || g_args[2] != &header.right
        || t.count != 1 || obj.RefCount != 1 || g_destroyed != 0) { std::printf("004c43b0_TEST FAIL\n"); return 1; }
    t.erase(b);
    if (g_unlinked != b || t.count != 0 || obj.RefCount != 0 || g_destroyed != 1) { std::printf("004c43b0_TEST FAIL\n"); return 1; }
    std::printf("004c43b0_TEST PASS\n");
    return 0;
}