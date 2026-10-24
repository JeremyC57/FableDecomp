#include <cstdio>
#include <cstdlib>
// Behaviour test: STLport _Rb_tree::insert_unique(const value_type&) for map<ETableSprites, long (signed key)>: descend comparing the key,
// step back with _Rb_global<bool>::_M_decrement when the last move went left (unless that is begin()), insert only if the
// predecessor's key is smaller, else return the existing node with false.
typedef int Key;
struct Node { bool Color; Node* _M_parent; Node* _M_left; Node* _M_right; Key First; long Second; };
struct Pair { Node* first; bool second; };
struct Tree {
    Node* _M_header;   // parent = root, left = leftmost, right = rightmost
    int Count;
    Tree() : Count(0) { _M_header = (Node*)std::malloc(sizeof(Node)); _M_header->_M_parent = 0; _M_header->_M_left = _M_header; _M_header->_M_right = _M_header; _M_header->Color = true; }
    static Node* _M_decrement(Node* x)   // 0x004294E2
    {
        if (x->_M_left != 0) { Node* y = x->_M_left; while (y->_M_right != 0) y = y->_M_right; return y; }
        Node* y = x->_M_parent;
        while (x == y->_M_left) { x = y; y = y->_M_parent; }
        return y;
    }
    // _M_insert stand-in (0x00632A30 / 0x00632D30): link as a child of y, no rebalancing needed for the test
    Node* _M_insert(Node* x, Node* y, const Key& k, long v)
    {
        Node* z = (Node*)std::malloc(sizeof(Node));
        z->First = k; z->Second = v; z->_M_left = z->_M_right = 0; z->Color = false;
        if (y == _M_header || x != 0 || k < y->First) {
            y->_M_left = z;
            if (y == _M_header) { _M_header->_M_parent = z; _M_header->_M_right = z; }
            else if (y == _M_header->_M_left) _M_header->_M_left = z;
        } else {
            y->_M_right = z;
            if (y == _M_header->_M_right) _M_header->_M_right = z;
        }
        z->_M_parent = y;
        ++Count;
        return z;
    }
    Pair insert_unique(const Key& k, long v)
    {
        Node* y = _M_header;
        Node* x = _M_header->_M_parent;
        bool comp = true;
        while (x != 0) {
            y = x;
            comp = k < x->First;
            x = comp ? x->_M_left : x->_M_right;
        }
        Node* j = y;
        Pair r;
        if (comp) {
            if (j == _M_header->_M_left) { r.first = _M_insert(x, y, k, v); r.second = true; return r; }
            j = _M_decrement(j);
        }
        if (j->First < k) { r.first = _M_insert(x, y, k, v); r.second = true; return r; }
        r.first = j; r.second = false;
        return r;
    }
};
int main()
{
    Tree t;
    Key keys[6] = { 5, -3, 9, 0, 7, -8 };
    for (int i = 0; i < 6; ++i) {
        Pair p = t.insert_unique(keys[i], i);
        if (!p.second || p.first->First != keys[i] || p.first->Second != i) { std::printf("00632990_TEST FAIL\n"); return 1; }
    }
    Key dups[3] = { 9, -3, 0 };
    for (int d = 0; d < 3; ++d) {
        Pair p = t.insert_unique(dups[d], 99);
        if (p.second || p.first->First != dups[d] || p.first->Second == 99) { std::printf("00632990_TEST FAIL\n"); return 1; }
    }
    if (t.Count != 6) { std::printf("00632990_TEST FAIL\n"); return 1; }
    // in-order walk is sorted
    Node* n = t._M_header->_M_left;
    Key prev = n->First;
    for (int c = 1; c < 6; ++c) {
        if (n->_M_right) { n = n->_M_right; while (n->_M_left) n = n->_M_left; }
        else { Node* y = n->_M_parent; while (n == y->_M_right) { n = y; y = y->_M_parent; } n = y; }
        if (!(prev < n->First)) { std::printf("00632990_TEST FAIL\n"); return 1; }
        prev = n->First;
    }
    std::printf("00632990_TEST PASS\n");
    return 0;
}