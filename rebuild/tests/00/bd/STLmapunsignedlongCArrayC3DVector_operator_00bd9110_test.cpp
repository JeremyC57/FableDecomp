// behaviour test for 0x00BD9110 (STLport red-black tree model)
#include <cstdio>
#include <cstdlib>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Array { void* first; void* last; void* end; };
struct Node : NodeBase { unsigned long key; Array value; };
static int g_inserts = 0;

struct Map {
    NodeBase* header; unsigned count;
    NodeBase* InsertUnique(NodeBase* hint, unsigned long key)   // stand-in for insert_unique: links a new node under hint
    {
        ++g_inserts;
        Node* n = (Node*)std::malloc(sizeof(Node));
        n->key = key; n->value.first = n->value.last = n->value.end = 0; n->left = n->right = 0;
        if (header->parent == 0) { header->parent = n; header->left = header->right = n; n->parent = header; }
        else if (hint == header) { NodeBase* p = header->right; p->right = n; n->parent = p; header->right = n; }
        else { hint->left = n; n->parent = hint; if (header->left == hint) header->left = n; }
        ++count;
        return n;
    }
    Array& operator[](const unsigned long& k)
    {
        NodeBase* y = header; NodeBase* x = header->parent;
        while (x != 0)
            if (!(((Node*)x)->key < k)) { y = x; x = x->left; } else x = x->right;
        if (y == header || k < ((Node*)y)->key)
            y = InsertUnique(y, k);
        return ((Node*)y)->value;
    }
};

int main()
{
    NodeBase head; head.parent = 0; head.left = head.right = &head;
    Map m; m.header = &head; m.count = 0;
    Array& a = m[10];
    a.first = &head;
    Array& b = m[10];
    if (&a != &b || g_inserts != 1 || b.first != &head) { std::printf("00bd9110_TEST FAIL\n"); return 1; }
    Array& c = m[20];
    Array& d = m[5];
    if (g_inserts != 3 || m.count != 3 || &c == &a || &d == &a || c.first != 0) { std::printf("00bd9110_TEST FAIL\n"); return 1; }
    if (&m[20] != &c || &m[5] != &d || g_inserts != 3) { std::printf("00bd9110_TEST FAIL\n"); return 1; }
    std::printf("00bd9110_TEST PASS\n");
    return 0;
}