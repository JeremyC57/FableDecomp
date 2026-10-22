// Behaviour test for _Rb_global<bool>::_Rebalance_for_erase (0x0042A34E): build red-black trees with the STLport insert
// fix-up, erase every key in several orders, and check order, leftmost/rightmost and the red-black invariants after each erase.
#include <cstdio>

struct _Rb_tree_node_base {
    bool _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
struct Node : _Rb_tree_node_base { int key; };
static const bool _S_rb_tree_red = false;
static const bool _S_rb_tree_black = true;
template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }
static _Rb_tree_node_base* _S_minimum(_Rb_tree_node_base* x) { while (x->_M_left) x = x->_M_left; return x; }
static _Rb_tree_node_base* _S_maximum(_Rb_tree_node_base* x) { while (x->_M_right) x = x->_M_right; return x; }
static void _Rotate_left(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_right;
    x->_M_right = y->_M_left;
    if (y->_M_left) y->_M_left->_M_parent = x;
    y->_M_parent = x->_M_parent;
    if (x == root) root = y; else if (x == x->_M_parent->_M_left) x->_M_parent->_M_left = y; else x->_M_parent->_M_right = y;
    y->_M_left = x; x->_M_parent = y;
}
static void _Rotate_right(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    _Rb_tree_node_base* y = x->_M_left;
    x->_M_left = y->_M_right;
    if (y->_M_right) y->_M_right->_M_parent = x;
    y->_M_parent = x->_M_parent;
    if (x == root) root = y; else if (x == x->_M_parent->_M_right) x->_M_parent->_M_right = y; else x->_M_parent->_M_left = y;
    y->_M_right = x; x->_M_parent = y;
}
static void _Rebalance(_Rb_tree_node_base* x, _Rb_tree_node_base*& root)
{
    x->_M_color = _S_rb_tree_red;
    while (x != root && x->_M_parent->_M_color == _S_rb_tree_red) {
        _Rb_tree_node_base* g = x->_M_parent->_M_parent;
        if (x->_M_parent == g->_M_left) {
            _Rb_tree_node_base* y = g->_M_right;
            if (y && y->_M_color == _S_rb_tree_red) { x->_M_parent->_M_color = _S_rb_tree_black; y->_M_color = _S_rb_tree_black; g->_M_color = _S_rb_tree_red; x = g; }
            else { if (x == x->_M_parent->_M_right) { x = x->_M_parent; _Rotate_left(x, root); }
                   x->_M_parent->_M_color = _S_rb_tree_black; x->_M_parent->_M_parent->_M_color = _S_rb_tree_red; _Rotate_right(x->_M_parent->_M_parent, root); }
        } else {
            _Rb_tree_node_base* y = g->_M_left;
            if (y && y->_M_color == _S_rb_tree_red) { x->_M_parent->_M_color = _S_rb_tree_black; y->_M_color = _S_rb_tree_black; g->_M_color = _S_rb_tree_red; x = g; }
            else { if (x == x->_M_parent->_M_left) { x = x->_M_parent; _Rotate_right(x, root); }
                   x->_M_parent->_M_color = _S_rb_tree_black; x->_M_parent->_M_parent->_M_color = _S_rb_tree_red; _Rotate_left(x->_M_parent->_M_parent, root); }
        }
    }
    root->_M_color = _S_rb_tree_black;
}

static _Rb_tree_node_base* _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                         _Rb_tree_node_base*& __leftmost, _Rb_tree_node_base*& __rightmost)
{
    _Rb_tree_node_base* __y = __z;
    _Rb_tree_node_base* __x = 0;
    _Rb_tree_node_base* __x_parent = 0;
    if (__y->_M_left == 0)
        __x = __y->_M_right;
    else
        if (__y->_M_right == 0)
            __x = __y->_M_left;
        else {
            __y = __y->_M_right;
            while (__y->_M_left != 0)
                __y = __y->_M_left;
            __x = __y->_M_right;
        }
    if (__y != __z) {
        __z->_M_left->_M_parent = __y;
        __y->_M_left = __z->_M_left;
        if (__y != __z->_M_right) {
            __x_parent = __y->_M_parent;
            if (__x) __x->_M_parent = __y->_M_parent;
            __y->_M_parent->_M_left = __x;
            __y->_M_right = __z->_M_right;
            __z->_M_right->_M_parent = __y;
        }
        else
            __x_parent = __y;
        if (__root == __z)
            __root = __y;
        else if (__z->_M_parent->_M_left == __z)
            __z->_M_parent->_M_left = __y;
        else
            __z->_M_parent->_M_right = __y;
        __y->_M_parent = __z->_M_parent;
        swap(__y->_M_color, __z->_M_color);
        __y = __z;
    }
    else {
        __x_parent = __y->_M_parent;
        if (__x) __x->_M_parent = __y->_M_parent;
        if (__root == __z)
            __root = __x;
        else
            if (__z->_M_parent->_M_left == __z)
                __z->_M_parent->_M_left = __x;
            else
                __z->_M_parent->_M_right = __x;
        if (__leftmost == __z)
            if (__z->_M_right == 0)
                __leftmost = __z->_M_parent;
            else
                __leftmost = _S_minimum(__x);
        if (__rightmost == __z)
            if (__z->_M_left == 0)
                __rightmost = __z->_M_parent;
            else
                __rightmost = _S_maximum(__x);
    }
    if (__y->_M_color != _S_rb_tree_red) {
        while (__x != __root && (__x == 0 || __x->_M_color == _S_rb_tree_black))
            if (__x == __x_parent->_M_left) {
                _Rb_tree_node_base* __w = __x_parent->_M_right;
                if (__w->_M_color == _S_rb_tree_red) {
                    __w->_M_color = _S_rb_tree_black;
                    __x_parent->_M_color = _S_rb_tree_red;
                    _Rotate_left(__x_parent, __root);
                    __w = __x_parent->_M_right;
                }
                if ((__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) &&
                    (__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black)) {
                    __w->_M_color = _S_rb_tree_red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black) {
                        if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
                        __w->_M_color = _S_rb_tree_red;
                        _Rotate_right(__w, __root);
                        __w = __x_parent->_M_right;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = _S_rb_tree_black;
                    if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
                    _Rotate_left(__x_parent, __root);
                    break;
                }
            } else {
                _Rb_tree_node_base* __w = __x_parent->_M_left;
                if (__w->_M_color == _S_rb_tree_red) {
                    __w->_M_color = _S_rb_tree_black;
                    __x_parent->_M_color = _S_rb_tree_red;
                    _Rotate_right(__x_parent, __root);
                    __w = __x_parent->_M_left;
                }
                if ((__w->_M_right == 0 || __w->_M_right->_M_color == _S_rb_tree_black) &&
                    (__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black)) {
                    __w->_M_color = _S_rb_tree_red;
                    __x = __x_parent;
                    __x_parent = __x_parent->_M_parent;
                } else {
                    if (__w->_M_left == 0 || __w->_M_left->_M_color == _S_rb_tree_black) {
                        if (__w->_M_right) __w->_M_right->_M_color = _S_rb_tree_black;
                        __w->_M_color = _S_rb_tree_red;
                        _Rotate_left(__w, __root);
                        __w = __x_parent->_M_left;
                    }
                    __w->_M_color = __x_parent->_M_color;
                    __x_parent->_M_color = _S_rb_tree_black;
                    if (__w->_M_left) __w->_M_left->_M_color = _S_rb_tree_black;
                    _Rotate_right(__x_parent, __root);
                    break;
                }
            }
        if (__x) __x->_M_color = _S_rb_tree_black;
    }
    return __y;
}

struct Tree {
    _Rb_tree_node_base header;   // parent = root, left = leftmost, right = rightmost
    Tree() { header._M_color = _S_rb_tree_red; header._M_parent = 0; header._M_left = &header; header._M_right = &header; }
    void Insert(Node* n)
    {
        n->_M_left = n->_M_right = 0;
        _Rb_tree_node_base* p = &header; _Rb_tree_node_base* c = header._M_parent; bool left = true;
        while (c) { p = c; left = n->key < ((Node*)c)->key; c = left ? c->_M_left : c->_M_right; }
        n->_M_parent = p;
        if (p == &header) { header._M_parent = n; header._M_left = n; header._M_right = n; }
        else if (left) { p->_M_left = n; if (p == header._M_left) header._M_left = n; }
        else { p->_M_right = n; if (p == header._M_right) header._M_right = n; }
        _Rebalance(n, header._M_parent);
    }
    void Erase(Node* n)
    {
        _Rb_tree_node_base* d = _Rebalance_for_erase(n, header._M_parent, header._M_left, header._M_right);
        if (d != n) bad = true;
    }
    static bool bad;
};
bool Tree::bad = false;

static int BlackHeight(_Rb_tree_node_base* n, int lo, int hi, bool& ok)
{
    if (!n) return 1;
    int k = ((Node*)n)->key;
    if (k < lo || k > hi) ok = false;
    if (n->_M_color == _S_rb_tree_red) {
        if ((n->_M_left && n->_M_left->_M_color == _S_rb_tree_red) || (n->_M_right && n->_M_right->_M_color == _S_rb_tree_red)) ok = false;
    }
    if (n->_M_left && n->_M_left->_M_parent != n) ok = false;
    if (n->_M_right && n->_M_right->_M_parent != n) ok = false;
    int l = BlackHeight(n->_M_left, lo, k, ok), r = BlackHeight(n->_M_right, k, hi, ok);
    if (l != r) ok = false;
    return l + (n->_M_color == _S_rb_tree_black ? 1 : 0);
}
static int Count(_Rb_tree_node_base* n) { return n ? 1 + Count(n->_M_left) + Count(n->_M_right) : 0; }

int main()
{
    const int N = 40;
    for (int order = 0; order < 3; ++order) {
        Node nodes[N];
        Tree t;
        for (int i = 0; i < N; ++i) { nodes[i].key = (i * 17) % N; t.Insert(&nodes[i]); }
        int remaining = N;
        for (int j = 0; j < N; ++j) {
            int idx = order == 0 ? j : order == 1 ? N - 1 - j : (j * 7) % N;
            t.Erase(&nodes[idx]);
            --remaining;
            bool ok = true;
            _Rb_tree_node_base* root = t.header._M_parent;
            if (root) {
                if (root->_M_color != _S_rb_tree_black || root->_M_parent != &t.header) ok = false;
                BlackHeight(root, -1, N, ok);
                if (t.header._M_left != _S_minimum(root) || t.header._M_right != _S_maximum(root)) ok = false;
            } else if (t.header._M_left != &t.header || t.header._M_right != &t.header) ok = false;
            if (!ok || Count(root) != remaining || Tree::bad) { std::printf("0042a34e_TEST FAIL\n"); return 1; }
        }
    }
    std::printf("0042a34e_TEST PASS\n");
    return 0;
}
