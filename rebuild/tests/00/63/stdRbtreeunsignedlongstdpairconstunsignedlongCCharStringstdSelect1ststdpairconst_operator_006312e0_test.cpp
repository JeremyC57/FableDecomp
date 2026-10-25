// Behaviour test for the STLport _Rb_tree::operator= at 0x006312E0: the source template with malloc stand-ins for
// _M_erase / _M_copy; checks the copied shape (root, leftmost, rightmost, count), self-assignment and an empty source.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
namespace std {

template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;                                       // +0x00
    T2 second;                                      // +0x04
};
template <class Pair> struct _Select1st;
template <class T> struct less { };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                             // +0x00
    _Base_ptr _M_parent;                            // +0x04
    _Base_ptr _M_left;                              // +0x08
    _Base_ptr _M_right;                             // +0x0C
    static _Base_ptr _S_minimum(_Base_ptr x) { while (x->_M_left != 0) x = x->_M_left; return x; }
    static _Base_ptr _S_maximum(_Base_ptr x) { while (x->_M_right != 0) x = x->_M_right; return x; }
};

template <class Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    Value _M_value_field;                           // +0x10
};

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Node* _Link_type;
    typedef _Rb_tree_node_base* _Base_ptr;

    _STLP_alloc_proxy<_Node*, allocator<_Node> > _M_header;     // +0x00
    unsigned int _M_node_count;                                 // +0x04
    Compare _M_key_compare;                                     // +0x08 (empty)

    _Base_ptr& _M_root() const { return this->_M_header._M_data->_M_parent; }
    _Base_ptr& _M_leftmost() const { return this->_M_header._M_data->_M_left; }
    _Base_ptr& _M_rightmost() const { return this->_M_header._M_data->_M_right; }

    void _M_erase(_Link_type x);                                // 0x00630D90
    _Link_type _M_copy(_Link_type x, _Link_type p);             // 0x006310E0
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Link_type)_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    _Rb_tree& operator=(const _Rb_tree& __x);
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& __x)
{
    if (this != &__x) {
        // Note that _Key may be a constant type.
        clear();
        _M_node_count = 0;
        _M_key_compare = __x._M_key_compare;
        if (__x._M_root() == 0) {
            _M_root() = 0;
            _M_leftmost() = this->_M_header._M_data;
            _M_rightmost() = this->_M_header._M_data;
        }
        else {
            _M_root() = _M_copy((_Link_type)__x._M_root(), this->_M_header._M_data);
            _M_leftmost() = _Rb_tree_node_base::_S_minimum(_M_root());
            _M_rightmost() = _Rb_tree_node_base::_S_maximum(_M_root());
            _M_node_count = __x._M_node_count;
        }
    }
    return *this;
}

}

class CCharString {                                 // 4 bytes: the string-data pointer
public:
    void* PStringData;
};

typedef std::pair<const unsigned long, CCharString> TActionValue;

static int g_erased = 0;
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void std::_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_erase(_Link_type x)
{
    while (x != 0) { _M_erase((_Link_type)x->_M_right); _Link_type y = (_Link_type)x->_M_left; free(x); ++g_erased; x = y; }
}
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename std::_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_Link_type
std::_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::_M_copy(_Link_type x, _Link_type p)
{
    _Link_type top = (_Link_type)malloc(sizeof(_Node));
    memcpy(top, x, sizeof(_Node));
    top->_M_parent = p; top->_M_left = 0; top->_M_right = 0;
    if (x->_M_left) top->_M_left = _M_copy((_Link_type)x->_M_left, top);
    if (x->_M_right) top->_M_right = _M_copy((_Link_type)x->_M_right, top);
    return top;
}
typedef std::_Rb_tree<unsigned long, TActionValue, std::_Select1st<TActionValue>, std::less<unsigned long>, std::allocator<TActionValue> > Tree;
static Tree::_Link_type node(long key)
{
    Tree::_Link_type n = (Tree::_Link_type)malloc(sizeof(Tree::_Node)); memset(n, 0, sizeof(Tree::_Node));
    *(long*)&n->_M_value_field = key; return n;
}
static void init(Tree& t) { t._M_header._M_data = node(-1); t._M_header._M_data->_M_left = t._M_header._M_data; t._M_header._M_data->_M_right = t._M_header._M_data; t._M_node_count = 0; }
int main()
{
    Tree src, dst, empty; init(src); init(dst); init(empty);
    Tree::_Link_type r = node(2), l = node(1), g = node(3);
    r->_M_left = l; r->_M_right = g; l->_M_parent = r; g->_M_parent = r; r->_M_parent = src._M_header._M_data;
    src._M_header._M_data->_M_parent = r; src._M_header._M_data->_M_left = l; src._M_header._M_data->_M_right = g; src._M_node_count = 3;
    Tree::_Link_type d = node(9); d->_M_parent = dst._M_header._M_data;
    dst._M_header._M_data->_M_parent = d; dst._M_header._M_data->_M_left = d; dst._M_header._M_data->_M_right = d; dst._M_node_count = 1;
    dst = src;
    std::_Rb_tree_node_base* h = dst._M_header._M_data;
    int ok = g_erased == 1 && dst._M_node_count == 3 && h->_M_parent != r && *(long*)&((Tree::_Link_type)h->_M_parent)->_M_value_field == 2
        && *(long*)&((Tree::_Link_type)h->_M_left)->_M_value_field == 1 && *(long*)&((Tree::_Link_type)h->_M_right)->_M_value_field == 3
        && h->_M_parent->_M_parent == h;
    dst = dst;
    ok = ok && dst._M_node_count == 3 && g_erased == 1;
    dst = empty;
    ok = ok && g_erased == 4 && dst._M_node_count == 0 && h->_M_parent == 0 && h->_M_left == h && h->_M_right == h;
    printf(ok ? "006312e0_TEST PASS\n" : "006312e0_TEST FAIL\n");
    return ok ? 0 : 1;
}
