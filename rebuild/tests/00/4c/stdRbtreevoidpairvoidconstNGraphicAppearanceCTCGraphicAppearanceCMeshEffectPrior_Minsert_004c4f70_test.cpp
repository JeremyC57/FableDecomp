// Behaviour test for 0x004C4F70 _Rb_tree<void*, pair<void* const, CMeshEffectPriorityEntry>>::_M_insert(x, y, v, w): the node goes left of
// y when y is the header, or (w null) when x is set or v's key orders before y's; otherwise right. Header root/leftmost/
// rightmost links follow, the node's links are reset, the count grows and the result iterator names the new node.
#include <cstdio>
#include <cstdlib>

struct Entry { void* PEffect; long Priority; bool B0, B1, B2; };
struct Value { void* first; Entry second; };
struct Node { bool color; Node* parent; Node* left; Node* right; Value v; };
static int g_rebalances = 0;
static void Rebalance(Node*, Node*&) { ++g_rebalances; }

struct Tree {
    Node* header; unsigned count;
    Node* Create(const Value& v) { Node* n = static_cast<Node*>(std::malloc(sizeof(Node))); n->v = v; return n; }
    Node** Insert(Node** result, Node* x, Node* y, const Value& v, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || (unsigned long)v.first < (unsigned long)y->v.first))) {
            z = Create(v); y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = Create(v); y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        Rebalance(z, header->parent);
        ++count; *result = z; return result;
    }
};

int main()
{
    Node h = { false, 0, &h, &h }; Tree t = { &h, 0 };
    Node* it = 0;
    Value a = { (void*)0x100, { 0, 4, true, false, true } };
    t.Insert(&it, 0, &h, a, 0);                        // empty: left of header => root/leftmost/rightmost
    Node* na = it;
    if (h.parent != na || h.left != na || h.right != na || t.count != 1 || !na->v.second.B0 || na->v.second.Priority != 4) {
        std::printf("004c4f70_TEST FAIL\n"); return 1;
    }
    Value b = { (void*)0x200, { 0, 8, false, false, false } };
    t.Insert(&it, 0, na, b, 0);                        // 0x200 !< 0x100: right of na, new rightmost
    Node* nb = it;
    if (na->right != nb || h.right != nb || h.left != na || nb->parent != na || nb->left || nb->right || t.count != 2) {
        std::printf("004c4f70_TEST FAIL\n"); return 1;
    }
    Value c = { (void*)0x80, { 0, 9, false, true, false } };
    t.Insert(&it, 0, na, c, 0);                        // 0x80 < 0x100: left of na, new leftmost
    if (na->left != it || h.left != it || t.count != 3 || g_rebalances != 3 || it->v.second.Priority != 9 || !it->v.second.B1) {
        std::printf("004c4f70_TEST FAIL\n"); return 1;
    }
    Value d = { (void*)0x10, { 0, 0, false, false, false } };
    t.Insert(&it, 0, nb, d, nb);                       // w set: right of nb regardless of the key
    if (nb->right != it || h.right != it) { std::printf("004c4f70_TEST FAIL\n"); return 1; }
    std::printf("004c4f70_TEST PASS\n");
    return 0;
}