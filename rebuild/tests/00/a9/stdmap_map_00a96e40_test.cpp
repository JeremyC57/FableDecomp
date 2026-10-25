// behaviour test for 0x00A96E40 map<unsigned long, long> copy constructor: the copy gets its own header (red, parent = the copied root), an empty source gives root 0 and leftmost = rightmost = header, a non-empty one is cloned node by node with leftmost / rightmost at the copy's minimum / maximum, and the node count is copied
#include <cstdio>
#include <cstdlib>
struct Node { bool color; Node* parent; Node* left; Node* right; int key; };
struct Tree { Node* header; unsigned int count; };
static int gClones = 0;
static Node* Clone(Node* x) { Node* t = (Node*)std::malloc(sizeof(Node)); *t = *x; t->left = 0; t->right = 0; ++gClones; return t; }
static Node* Copy(Node* x, Node* p)                      // STLport _M_copy
{
    Node* top = Clone(x);
    top->parent = p;
    if (x->right) top->right = Copy(x->right, top);
    p = top; x = x->left;
    while (x != 0) {
        Node* y = Clone(x);
        p->left = y; y->parent = p;
        if (x->right) y->right = Copy(x->right, y);
        p = y; x = x->left;
    }
    return top;
}
static void CopyConstruct(Tree* self, const Tree& x)
{
    self->header = 0;
    self->header = (Node*)std::malloc(sizeof(Node));
    self->count = 0;
    Node* h = self->header;
    h->color = false;
    if (x.header->parent == 0) { h->parent = 0; h->left = h; h->right = h; }
    else {
        h->parent = Copy(x.header->parent, h);
        Node* m = h->parent; while (m->left) m = m->left; h->left = m;
        m = h->parent; while (m->right) m = m->right; h->right = m;
    }
    self->count = x.count;
}
static Node* Mk(int k, Node* l, Node* r) { Node* n = (Node*)std::malloc(sizeof(Node)); n->color = true; n->key = k; n->left = l; n->right = r; n->parent = 0; if (l) l->parent = n; if (r) r->parent = n; return n; }
static int Same(Node* a, Node* b) { if (!a || !b) return a == b; return a != b && a->key == b->key && Same(a->left, b->left) && Same(a->right, b->right); }
int main()
{
    int fails = 0;
    Tree src, cp, e, ce;
    Node* root = Mk(50, Mk(20, Mk(10, 0, 0), Mk(30, 0, 0)), Mk(80, 0, Mk(90, 0, 0)));
    src.header = (Node*)std::malloc(sizeof(Node)); src.header->parent = root; root->parent = src.header; src.count = 6;
    CopyConstruct(&cp, src);
    if (cp.header == src.header || cp.count != 6 || gClones != 6) fails |= 1;
    if (!Same(cp.header->parent, root) || cp.header->parent->parent != cp.header) fails |= 2;
    if (cp.header->left->key != 10 || cp.header->right->key != 90 || cp.header->color != false) fails |= 4;
    e.header = (Node*)std::malloc(sizeof(Node)); e.header->parent = 0; e.count = 0;
    gClones = 0;
    CopyConstruct(&ce, e);
    if (ce.header->parent != 0 || ce.header->left != ce.header || ce.header->right != ce.header || ce.count != 0 || gClones != 0) fails |= 8;
    if (fails) { std::printf("00a96e40_TEST FAIL %d\n", fails); return 1; }
    std::printf("00a96e40_TEST PASS\n");
    return 0;
}