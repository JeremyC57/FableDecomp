// behaviour test for 0x00B96D70 (STLport red-black tree model)
#include <cstdio>
#include <cstdlib>
#include <new>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Stats { unsigned long v[7]; };
struct Value { long key; Stats stats; };
struct Node : NodeBase { Value value; };
static int g_rebalanced = 0;
static void Rebalance(NodeBase*, NodeBase*&) { ++g_rebalanced; }

struct Tree {
    NodeBase* header; unsigned count;
    NodeBase* Insert(NodeBase* x, NodeBase* y, const Value& v, NodeBase* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || v.key < ((Node*)y)->value.key))) {
            z = (Node*)std::malloc(sizeof(Node)); new (&z->value) Value(v);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = (Node*)std::malloc(sizeof(Node)); new (&z->value) Value(v);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return z;
    }
};

int main()
{
    NodeBase head; head.parent = 0; head.left = &head; head.right = &head;
    Tree t; t.header = &head; t.count = 0;
    Value v; v.key = 5; for (int i = 0; i < 7; ++i) v.stats.v[i] = (unsigned long)(i * 10);
    Node* root = (Node*)t.Insert(0, &head, v, 0);
    if (head.parent != root || head.left != root || head.right != root || root->value.stats.v[6] != 60) { std::printf("00b96d70_TEST FAIL\n"); return 1; }
    Value lo = v; lo.key = 2;
    Node* l = (Node*)t.Insert(0, root, lo, 0);       // key 2 < 5: left of root, new leftmost
    if (root->left != l || head.left != l || head.right != root) { std::printf("00b96d70_TEST FAIL\n"); return 1; }
    Value hi = v; hi.key = 9;
    Node* r = (Node*)t.Insert(0, root, hi, 0);       // key 9 > 5: right of root, new rightmost
    if (root->right != r || head.right != r || t.count != 3 || g_rebalanced != 3 || r->parent != root) { std::printf("00b96d70_TEST FAIL\n"); return 1; }
    std::printf("00b96d70_TEST PASS\n");
    return 0;
}