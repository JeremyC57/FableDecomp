// behaviour test for 0x005BB102 _Rb_tree<...>::erase(first, last) (set<EClothingSuitPart>): the whole range [leftmost, header) clears the tree in one step; any other range erases first..last-1 in order (post-increment before each erase) and never touches last
#include <cstdio>
struct Node { bool color; Node* parent; Node* left; Node* right; long key; int erased; };
static Node* Increment(Node* n)
{
    if (n->right != 0) {
        n = n->right;
        while (n->left != 0) n = n->left;
    } else {
        Node* y = n->parent;
        while (n == y->right) { n = y; y = y->parent; }
        if (n->right != y) n = y;
    }
    return n;
}
struct Tree {
    Node* header; unsigned int count; int clears; int order[16]; int erasedCount;
    void clear() { if (count != 0) { header->parent = 0; header->left = header; header->right = header; count = 0; } ++clears; }
    void eraseOne(Node* n) { n->erased = 1; order[erasedCount++] = (int)n->key; --count; }
    void erase(Node* first, Node* last)
    {
        if (first == header->left && last == header) clear();
        else while (first != last) { Node* t = first; first = Increment(first); eraseOne(t); }
    }
};
static Node gNodes[16];
static int gUsed = 0;
static Node* Insert(Node* root, Node* parent, long v)
{
    if (root == 0) {
        Node* n = &gNodes[gUsed++];
        n->color = false; n->parent = parent; n->left = 0; n->right = 0; n->key = v; n->erased = 0;
        return n;
    }
    if (v < root->key) root->left = Insert(root->left, root, v);
    else root->right = Insert(root->right, root, v);
    return root;
}
static void Build(Node& header, Tree& t)
{
    gUsed = 0;
    header.color = true; header.parent = 0; header.left = &header; header.right = &header; header.key = 0;
    t.header = &header; t.count = 0; t.clears = 0; t.erasedCount = 0;
    const long keys[] = { 50, 20, 70, 10, 30, 60, 80 };
    for (int i = 0; i < 7; ++i) { header.parent = Insert(header.parent, &header, keys[i]); ++t.count; }
    header.parent->parent = &header;
    Node* m = header.parent; while (m->left) m = m->left; header.left = m;
    m = header.parent; while (m->right) m = m->right; header.right = m;
}
int main()
{
    int fails = 0;
    Node header; Tree t;
    Build(header, t);
    t.erase(header.left, &header);                                   // whole tree
    if (t.clears != 1 || t.count != 0 || t.erasedCount != 0 || header.parent != 0 || header.left != &header) fails |= 1;
    Build(header, t);
    Node* first = Increment(header.left);                             // 20
    Node* last = Increment(Increment(Increment(first)));              // 60
    t.erase(first, last);                                             // erases 20, 30, 50
    if (t.clears != 0 || t.count != 4 || t.erasedCount != 3) fails |= 2;
    if (t.order[0] != 20 || t.order[1] != 30 || t.order[2] != 50) fails |= 4;
    if (last->erased || header.left->erased) fails |= 8;
    Build(header, t);
    t.erase(header.left, header.left);                                // empty range
    if (t.erasedCount != 0 || t.clears != 0 || t.count != 7) fails |= 16;
    Build(header, t);
    t.erase(Increment(header.left), &header);                         // all but the leftmost: node by node
    if (t.clears != 0 || t.erasedCount != 6 || t.count != 1 || t.order[5] != 80) fails |= 32;
    if (fails) { std::printf("005bb102_TEST FAIL %d\n", fails); return 1; }
    std::printf("005bb102_TEST PASS\n");
    return 0;
}