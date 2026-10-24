// Behaviour test for 0x006C8350 _Rb_tree<long,...>::equal_range: on a hand-built search tree (keys 10, 20, 20, 30)
// the pair is (first node with key >= k, first node with key > k), the header standing for end().
#include <cstdio>

struct node { unsigned char colour; node* parent; node* left; node* right; long key; };
struct iterator { node* _M_node; iterator(node* n) : _M_node(n) {} iterator(const iterator& o) : _M_node(o._M_node) {} };
struct iterator_pair { iterator first, second; iterator_pair(const iterator& a, const iterator& b) : first(a), second(b) {} };

struct Tree {
    node* head;
    node* lower(const long& k) const { node* y = head; node* x = head->parent; while (x) if (!(x->key < k)) y = x, x = x->left; else x = x->right; return y; }
    node* upper(const long& k) const { node* y = head; node* x = head->parent; while (x) if (k < x->key) y = x, x = x->left; else x = x->right; return y; }
    iterator_pair equal_range(const long& k) { return iterator_pair(iterator(lower(k)), iterator(upper(k))); }
};

static node mk(long k) { node n; n.colour = 0; n.parent = n.left = n.right = 0; n.key = k; return n; }

int main()
{
    node head = mk(0), n10 = mk(10), n20a = mk(20), n20b = mk(20), n30 = mk(30);
    // root n20a: left n10, right n30; n30.left = n20b
    head.parent = &n20a; n20a.left = &n10; n20a.right = &n30; n30.left = &n20b;
    Tree t; t.head = &head;
    long k = 20;
    iterator_pair p = t.equal_range(k);
    if (p.first._M_node != &n20a || p.second._M_node != &n30) { std::printf("006c8350_TEST FAIL\n"); return 1; }
    k = 5;  p = t.equal_range(k);
    if (p.first._M_node != &n10 || p.second._M_node != &n10) { std::printf("006c8350_TEST FAIL\n"); return 1; }
    k = 40; p = t.equal_range(k);
    if (p.first._M_node != &head || p.second._M_node != &head) { std::printf("006c8350_TEST FAIL\n"); return 1; }
    k = 30; p = t.equal_range(k);
    if (p.first._M_node != &n30 || p.second._M_node != &head) { std::printf("006c8350_TEST FAIL\n"); return 1; }
    std::printf("006c8350_TEST PASS\n");
    return 0;
}