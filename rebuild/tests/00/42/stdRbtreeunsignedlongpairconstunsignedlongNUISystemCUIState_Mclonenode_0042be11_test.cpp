#include <stdlib.h>
#include <new>
#include <stdio.h>
#include <string.h>
static int g_copies = 0;
namespace NUISystem {
class CUIState {                                         // 0x24 bytes (Ego_r.pdb 36)
public:
    unsigned long StateChangeFlag;
    long          StateChangeType;
    float         Position[2];
    float         Zoom[2];
    unsigned long Colour;
    float         UpdateTime;
    bool          LinearChange;
    CUIState() {}
    CUIState& operator=(const CUIState& s) { memcpy(this, &s, sizeof(*this)); return *this; }
    CUIState(const CUIState& s);
};
}
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
template <class T> struct less {};
template <class T> class allocator {};
template <class V> struct _Select1st {};
struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00 (_Rb_tree_Color_type)
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0c
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                    // +0x10
};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_node<V>* _Link_type;
    _Rb_tree_node_base* _M_header;
    size_t              _M_node_count;
    _Link_type _M_create_node(const V& x)
    {
        _Link_type tmp = (_Link_type)malloc(sizeof(_Rb_tree_node<V>));
        _Construct(&tmp->_M_value_field, x);
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
};
}

template std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_M_clone_node(std::_Rb_tree<unsigned long, std::pair<const unsigned long, NUISystem::CUIState>, std::_Select1st<std::pair<const unsigned long, NUISystem::CUIState> >, std::less<unsigned long>, std::allocator<std::pair<const unsigned long, NUISystem::CUIState> > >::_Link_type);

NUISystem::CUIState::CUIState(const CUIState& s) { *this = s; ++g_copies; }
typedef std::pair<const unsigned long, NUISystem::CUIState> V;
typedef std::_Rb_tree<unsigned long, V, std::_Select1st<V>, std::less<unsigned long>, std::allocator<V> > T;
int main()
{
    T t; T::_Link_type x = (T::_Link_type)malloc(sizeof(*x)); memset(x, 0x55, sizeof(*x));
    x->_M_color = true; *(unsigned long*)&x->_M_value_field.first = 9; x->_M_value_field.second.UpdateTime = 1.5f;
    T::_Link_type n = t._M_clone_node(x);
    int ok = n != x && n->_M_color == true && n->_M_left == 0 && n->_M_right == 0 && n->_M_value_field.first == 9
          && n->_M_value_field.second.UpdateTime == 1.5f && g_copies == 1;
    x->_M_color = false; T::_Link_type m = t._M_clone_node(x); ok &= m->_M_color == false;
    free(n); free(m); free(x);
    printf(ok ? "0042be11_TEST PASS\n" : "0042be11_TEST FAIL\n");
    return ok ? 0 : 1;
}