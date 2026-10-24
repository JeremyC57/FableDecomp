// Behaviour test for 0x00485BF0 _Rb_tree<CWideString, pair<const CWideString, bool> >::_M_insert (model, rebalance stand-in).
#include <cstdio>
#include <cstdlib>

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Value { long first; int second; };
struct Node : NodeBase { Value value; };
struct Iterator { NodeBase* node; Iterator(NodeBase* n) : node(n) { } };

static int g_rebalances = 0;
static void Rebalance(NodeBase*, NodeBase*& root) { ++g_rebalances; root->color = true; }

struct Tree {
    Node* header;
    unsigned int count;
    Node* create(const Value& v) { Node* n = (Node*)std::malloc(sizeof(Node)); n->value = v; return n; }
    Iterator insert(NodeBase* x_, NodeBase* y_, const Value& v, NodeBase* w_)
    {
        Node* w = (Node*)w_; Node* x = (Node*)x_; Node* y = (Node*)y_; Node* z;
        if (y == header || (w == 0 && (x != 0 || v.first < y->value.first))) {
            z = create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count;
        return Iterator(z);
    }
};

int main()
{
    Node header; header.color = false; header.parent = 0; header.left = &header; header.right = &header;
    Tree t; t.header = &header; t.count = 0;
    Value a = { 5, 50 }, b = { 2, 20 }, c = { 9, 90 };
    Iterator ia = t.insert(0, &header, a, 0);
    Iterator ib = t.insert(0, ia.node, b, 0);          // 2 < 5: left of root, new leftmost
    Iterator ic = t.insert(0, ia.node, c, 0);          // 9 > 5: right of root, new rightmost
    int ok = header.parent == ia.node && header.left == ib.node && header.right == ic.node
          && ia.node->left == ib.node && ia.node->right == ic.node && ib.node->parent == ia.node
          && ((Node*)ic.node)->value.second == 90 && t.count == 3 && g_rebalances == 3;
    std::free(ia.node); std::free(ib.node); std::free(ic.node);
    if (!ok) { std::printf("00485bf0_TEST FAIL\n"); return 1; }
    std::printf("00485bf0_TEST PASS\n");
    return 0;
}