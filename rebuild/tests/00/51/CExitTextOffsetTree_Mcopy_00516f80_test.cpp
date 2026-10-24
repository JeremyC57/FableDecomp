// Behaviour test for 0x00516F80 _Rb_tree::_M_copy (map<CCharString, C2DVector>): a 6-node source tree is cloned node by
// node -- keys copy-constructed, vectors and colours copied, every parent/left/right link mirrored onto the new nodes,
// null children kept null -- and the clone's root hangs under the given parent.
#include <cstdio>
#include <cstdlib>
#include <new>
using std::malloc;
static int g_copies;

class CCharString {
public:
    const char* PString;
    CCharString(const char* s) : PString(s) {}
    CCharString(const CCharString& other) : PString(other.PString) { ++g_copies; }
};
class C2DVector {                              // Ego_r.pdb: float X, Y
public:
    float X;
    float Y;
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
}

struct _Rb_tree_node_base {
    bool                _M_color;              // +0x0
    _Rb_tree_node_base* _M_parent;             // +0x4
    _Rb_tree_node_base* _M_left;               // +0x8
    _Rb_tree_node_base* _M_right;              // +0xC
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                          // +0x10
};

class CExitTextOffsetTree {                    // STLport _Rb_tree<CCharString, pair<const CCharString, C2DVector>, ...>
public:
    typedef std::pair<const CCharString, C2DVector> value_type;
    typedef _Rb_tree_node<value_type> _Node;
    typedef _Node* _Link_type;
    _Node*        _M_header;                   // +0x0
    unsigned long _M_node_count;               // +0x4
    unsigned long _M_key_compare;              // +0x8 (empty less<CCharString>, padded)

    static _Link_type _S_left(_Link_type x) { return (_Link_type)x->_M_left; }
    static _Link_type _S_right(_Link_type x) { return (_Link_type)x->_M_right; }
    static _Node* allocate(unsigned int n) { return (_Node*)malloc(n * sizeof(_Node)); }
    _Link_type _M_create_node(const value_type& x)
    {
        _Link_type tmp = allocate(1);
        std::_Construct(&tmp->_M_value_field, x);
        return tmp;
    }
    _Link_type _M_clone_node(_Link_type x)
    {
        _Link_type tmp = _M_create_node(x->_M_value_field);
        tmp->_M_color = x->_M_color;
        tmp->_M_left = 0;
        tmp->_M_right = 0;
        return tmp;
    }
    _Link_type _M_copy(_Link_type x, _Link_type p);
};
CExitTextOffsetTree::_Link_type CExitTextOffsetTree::_M_copy(_Link_type x, _Link_type p)
{
    _Link_type top = _M_clone_node(x);
    top->_M_parent = p;
    if (x->_M_right)
        top->_M_right = _M_copy(_S_right(x), top);
    p = top;
    x = _S_left(x);
    while (x != 0) {
        _Link_type y = _M_clone_node(x);
        p->_M_left = y;
        y->_M_parent = p;
        if (x->_M_right)
            y->_M_right = _M_copy(_S_right(x), y);
        p = y;
        x = _S_left(x);
    }
    return top;
}
typedef CExitTextOffsetTree::_Node N;
static bool same_shape(const _Rb_tree_node_base* a, const _Rb_tree_node_base* b, const _Rb_tree_node_base* bp)
{
    if (a == 0 || b == 0) return a == b;
    if (a == b || b->_M_parent != bp || a->_M_color != b->_M_color) return false;
    const N* na = (const N*)a; const N* nb = (const N*)b;
    if (na->_M_value_field.first.PString != nb->_M_value_field.first.PString) return false;
    if (na->_M_value_field.second.X != nb->_M_value_field.second.X || na->_M_value_field.second.Y != nb->_M_value_field.second.Y) return false;
    return same_shape(a->_M_left, b->_M_left, b) && same_shape(a->_M_right, b->_M_right, b);
}
int main()
{
    //          d
    //       b     f
    //      a c   e
    static const char* keys[6] = { "a", "b", "c", "d", "e", "f" };
    N* n[6];
    for (int i = 0; i < 6; ++i) {
        n[i] = (N*)std::malloc(sizeof(N));
        n[i]->_M_value_field.second.X = (float)i; n[i]->_M_value_field.second.Y = (float)(10 * i);
        *(const char**)&n[i]->_M_value_field.first.PString = keys[i];
        n[i]->_M_color = (i % 2) != 0; n[i]->_M_left = n[i]->_M_right = 0;
    }
    n[3]->_M_left = n[1]; n[1]->_M_parent = n[3]; n[3]->_M_right = n[5]; n[5]->_M_parent = n[3];
    n[1]->_M_left = n[0]; n[0]->_M_parent = n[1]; n[1]->_M_right = n[2]; n[2]->_M_parent = n[1];
    n[5]->_M_left = n[4]; n[4]->_M_parent = n[5];
    _Rb_tree_node_base header;
    CExitTextOffsetTree t; t._M_header = 0; t._M_node_count = 0; t._M_key_compare = 0;
    N* c = t._M_copy(n[3], (N*)&header);
    int ok = c != n[3] && c->_M_parent == &header && same_shape(n[3], c, &header) && g_copies == 6;
    N* leaf = t._M_copy(n[2], n[0]);
    ok &= leaf->_M_parent == n[0] && leaf->_M_left == 0 && leaf->_M_right == 0 && g_copies == 7;
    ok &= leaf->_M_value_field.second.Y == 20.0f;
    if (!ok) { std::printf("00516f80_TEST FAIL\n"); return 1; }
    std::printf("00516f80_TEST PASS\n");
    return 0;
}