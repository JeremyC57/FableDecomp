// Behaviour test for 0x00534F20: STLport __find over red-black tree iterators (in-order successor walk)
#include <cstdio>

struct Node {
    bool  color;
    Node* parent;
    Node* left;
    Node* right;
    int   value;
};

static Node* Increment(Node* n)
{
    if (n->right != 0) {
        n = n->right;
        while (n->left != 0)
            n = n->left;
    } else {
        Node* y = n->parent;
        while (n == y->right) {
            n = y;
            y = y->parent;
        }
        if (n->right != y)
            n = y;
    }
    return n;
}

static Node* Find(Node* first, Node* last, const int& val)
{
    while (first != last && !(first->value == val))
        first = Increment(first);
    return first;
}

static Node g_nodes[8];

static void Link(Node* p, Node* l, Node* r)
{
    p->left = l; p->right = r;
    if (l) l->parent = p;
    if (r) r->parent = p;
}

int main()
{
    // header g_nodes[0]; tree: 4 (root) -> 2 (1,3), 6 (5, -)
    Node* h = &g_nodes[0];
    Node* n1 = &g_nodes[1]; Node* n2 = &g_nodes[2]; Node* n3 = &g_nodes[3];
    Node* n4 = &g_nodes[4]; Node* n5 = &g_nodes[5]; Node* n6 = &g_nodes[6];
    n1->value = 1; n2->value = 2; n3->value = 3; n4->value = 4; n5->value = 5; n6->value = 6;
    Link(n4, n2, n6); Link(n2, n1, n3); Link(n6, n5, 0); Link(n1, 0, 0); Link(n3, 0, 0); Link(n5, 0, 0);
    h->parent = n4; n4->parent = h; h->left = n1; h->right = n6;
    // in-order walk from leftmost reaches every value then the header
    Node* it = h->left;
    for (int k = 1; k <= 6; ++k) {
        if (it->value != k) { std::printf("00534f20_TEST FAIL\n"); return 1; }
        it = Increment(it);
    }
    if (it != h) { std::printf("00534f20_TEST FAIL\n"); return 1; }
    int v5 = 5, v9 = 9, v3 = 3;
    if (Find(h->left, h, v5) != n5 || Find(h->left, h, v9) != h || Find(n4, h, v3) != h || Find(n2, n6, v3) != n3) {
        std::printf("00534f20_TEST FAIL\n"); return 1;
    }
    std::printf("00534f20_TEST PASS\n");
    return 0;
}