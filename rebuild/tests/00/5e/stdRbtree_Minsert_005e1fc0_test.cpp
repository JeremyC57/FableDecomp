// Behaviour test for _Rb_tree<CCharString, pair<const CCharString, CIVCountedPointer<..> > >::_M_insert (0x005E1FC0):
// the left/right link choice, root/leftmost/rightmost maintenance, the node copy (key + counted pointer reference)
// and the count -- through a copy of the model (rebalance stubbed as a no-op: only the link logic is tested).
#include <cstdio>
#include <cstring>
#include <cstdlib>

struct Pointee { int RefCount; };
struct Node {
    bool color; Node* parent; Node* left; Node* right;
    const char* key; Pointee* value;
};
struct Tree {
    Node* header; unsigned int count;
    static bool Less(const char* a, const char* b)
    {
        if (b == a) return false;
        if (a == 0) return true;
        if (b == 0) return false;
        return std::strcmp(a, b) < 0;
    }
    Node* CreateNode(const char* key, Pointee* value)
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        z->key = key; z->value = value;
        if (value) ++value->RefCount;
        return z;
    }
    Node* Insert(Node* x, Node* y, const char* key, Pointee* value, Node* w)
    {
        Node* z;
        if (y == header || (w == 0 && (x != 0 || Less(key, y->key)))) {
            z = CreateNode(key, value);
            y->left = z;
            if (y == header) { header->parent = z; header->right = z; }
            else if (y == header->left) header->left = z;
        } else {
            z = CreateNode(key, value);
            y->right = z;
            if (y == header->right) header->right = z;
        }
        z->parent = y; z->left = 0; z->right = 0;
        ++count;
        return z;
    }
};

int main()
{
    Node hdr; hdr.color = false; hdr.parent = 0; hdr.left = &hdr; hdr.right = &hdr;
    Tree t; t.header = &hdr; t.count = 0;
    Pointee p; p.RefCount = 1;
    Node* root = t.Insert(0, &hdr, "m", &p, 0);              // empty tree: root, leftmost and rightmost
    if (hdr.parent != root || hdr.left != root || hdr.right != root || t.count != 1 || p.RefCount != 2) { std::printf("005e1fc0_TEST FAIL\n"); return 1; }
    Node* a = t.Insert(0, root, "a", &p, 0);                 // "a" < "m": left child, new leftmost
    if (root->left != a || hdr.left != a || hdr.right != root || a->parent != root) { std::printf("005e1fc0_TEST FAIL\n"); return 1; }
    Node* z = t.Insert(0, root, "z", 0, 0);                  // "z" > "m": right child, new rightmost, null pointer
    if (root->right != z || hdr.right != z || z->value != 0 || t.count != 3 || p.RefCount != 3) { std::printf("005e1fc0_TEST FAIL\n"); return 1; }
    Node* b = t.Insert(a, a, "b", &p, 0);                    // x != 0 forces the left link
    if (a->left != b || hdr.left != b) { std::printf("005e1fc0_TEST FAIL\n"); return 1; }
    Node* c = t.Insert(0, b, "0", &p, b);                    // w != 0 forces the right link
    if (b->right != c || hdr.right != z || c->left != 0 || c->right != 0) { std::printf("005e1fc0_TEST FAIL\n"); return 1; }
    std::printf("005e1fc0_TEST PASS\n");
    return 0;
}