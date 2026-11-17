// Behaviour test for 0x0053CB9B: _Rb_tree copy constructor (header alloc, empty reset or _M_copy + min/max, count copy)
#include <cstdio>
#include <cstdlib>

struct Node {
    bool  color;
    Node* parent;
    Node* left;
    Node* right;
    unsigned long key;
};

static Node* S_minimum(Node* x) { while (x->left != 0) x = x->left; return x; }
static Node* S_maximum(Node* x) { while (x->right != 0) x = x->right; return x; }

static Node* clone(Node* x)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->color = x->color; n->key = x->key; n->left = 0; n->right = 0; n->parent = 0;
    return n;
}

struct Tree {
    Node* header;
    unsigned int count;
    Node* M_copy(Node* x, Node* p)
    {
        Node* top = clone(x);
        top->parent = p;
        if (x->right) top->right = M_copy(x->right, top);
        p = top; x = x->left;
        while (x) {
            Node* y = clone(x);
            p->left = y; y->parent = p;
            if (x->right) y->right = M_copy(x->right, y);
            p = y; x = x->left;
        }
        return top;
    }
    Tree() : header(0), count(0) { header = (Node*)std::malloc(sizeof(Node)); header->color = false; header->parent = 0; header->left = header; header->right = header; }
    Tree(const Tree& x) : header(0), count(0)
    {
        header = (Node*)std::malloc(sizeof(Node));
        if (x.header->parent == 0) {
            header->color = false;
            header->parent = 0;
            header->left = header;
            header->right = header;
        } else {
            header->color = false;
            header->parent = M_copy(x.header->parent, header);
            header->left = S_minimum(header->parent);
            header->right = S_maximum(header->parent);
        }
        count = x.count;
    }
};

static Node* mk(unsigned long k, Node* l, Node* r)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->color = true; n->key = k; n->left = l; n->right = r; n->parent = 0;
    if (l) l->parent = n;
    if (r) r->parent = n;
    return n;
}

int main()
{
    Tree empty;
    Tree e2(empty);
    if (e2.header == empty.header || e2.header->parent != 0 || e2.header->left != e2.header ||
        e2.header->right != e2.header || e2.count != 0 || e2.header->color != false) {
        std::printf("0053cb9b_TEST FAIL\n"); return 1;
    }
    Tree t;
    Node* root = mk(5, mk(2, mk(1, 0, 0), mk(3, 0, 0)), mk(8, 0, mk(9, 0, 0)));
    t.header->parent = root; root->parent = t.header; t.count = 6;
    t.header->left = S_minimum(root); t.header->right = S_maximum(root);
    Tree c(t);
    if (c.count != 6 || c.header->parent == root || c.header->parent->key != 5 ||
        c.header->parent->parent != c.header || c.header->left->key != 1 || c.header->right->key != 9 ||
        c.header->parent->left->right->key != 3) {
        std::printf("0053cb9b_TEST FAIL\n"); return 1;
    }
    std::printf("0053cb9b_TEST PASS\n");
    return 0;
}