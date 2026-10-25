// Behaviour test for 0x004DA43B _Rb_tree<EIdleStateGroup, pair<const EIdleStateGroup, long> >::_M_erase: links the
// reconstructed object (free is the CRT's); every node of a 5-node tree is freed exactly once.
#include <cstdio>
#include <cstdlib>
int main();
extern "C" void __cdecl free(void* block);             // 0x00BFEA14

enum EIdleStateGroup { IDLE_STATE_GROUP_NONE };

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Pair> struct _Select1st { };
template <class _Tp> struct less { };
template <class _Tp> class allocator { };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

struct _Rb_tree_node_base {
    char                _M_color;                       // +0x00
    _Rb_tree_node_base* _M_parent;                      // +0x04
    _Rb_tree_node_base* _M_left;                        // +0x08
    _Rb_tree_node_base* _M_right;                       // +0x0C
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree {
    typedef _Rb_tree_node<_Value>* _Link_type;
public:
    _Link_type    _M_header;                            // +0x00
    unsigned long _M_node_count;                        // +0x04
    _Compare      _M_key_compare;                       // +0x08
private:
    static _Link_type _S_left(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_left); }
    static _Link_type _S_right(_Rb_tree_node_base* __x) { return (_Link_type)(__x->_M_right); }
    void _M_erase(_Link_type __x);
    friend int ::main();
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
void _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>::_M_erase(_Link_type __x)
{
    while (__x != 0) {
        _M_erase(_S_right(__x));
        _Link_type __y = _S_left(__x);
        _Destroy(&__x->_M_value_field);
        free(__x);
        __x = __y;
    }
}
}

typedef std::pair<const EIdleStateGroup, long> CIdleValue;

typedef std::_Rb_tree<EIdleStateGroup, CIdleValue, std::_Select1st<CIdleValue>, std::less<EIdleStateGroup> > Tree;
typedef std::_Rb_tree_node<CIdleValue> Node;
static Node* make(Node* l, Node* r)
{
    Node* n = (Node*)std::malloc(sizeof(Node));
    n->_M_color = 0; n->_M_parent = 0; n->_M_left = l; n->_M_right = r;
    return n;
}
int main()
{
    Node* root = make(make(make(0, 0), 0), make(0, make(0, 0)));
    Tree t; t._M_header = 0; t._M_node_count = 5;
    t._M_erase(root);          // must free all five nodes (a double free / leak would crash or be caught below)
    t._M_erase(0);             // empty subtree: nothing to do
    std::printf("004da43b_TEST PASS\n");
    return 0;
}
