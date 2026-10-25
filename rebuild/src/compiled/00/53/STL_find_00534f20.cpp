// _STL::__find(_Rb_tree_iterator first, _Rb_tree_iterator last, CComponent* const& val, const input_iterator_tag&) -- linear search of a set<CComponent*> range: walk the in-order successors until the node value equals val
// Leaf: ??$__find@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00534F20, length 95 bytes, `ret 0Ch`.
// Callers: NUISystem::CComponent::PositionChildDying 0x00531B40 (landed; it carries a same-TU stand-in of this body)
//   and the front end 0x00535320 (landed). No callees: the iterator increment is inline.
// Label: catalog "std::__find_in_tree_by_key" is descriptive; the body is STLport's `__find` for input iterators.
// Shape: STLport 4.x `_algobase.c` verbatim (`while (__first != __last && !(*__first == __val)) ++__first; return
//   __first;`) in a /Gr algorithm TU (`__fastcall` on the template): the iterator result goes through the hidden pointer
//   in ecx (the iterator class has user constructors), &val is the first register-eligible argument (edx), first, last
//   and the unused tag reference sit on the stack (`ret 0Ch`). operator++ is STLport 4.6's `_M_node =
//   _Rb_global<bool>::_M_increment(_M_node)` (the static successor walk, inlined); an in-place member
//   `_Rb_tree_base_iterator::_M_increment()` gives the same instructions with the node in esi and &val left in edx.
namespace NUISystem { class CComponent; }

namespace _STL {
struct input_iterator_tag { input_iterator_tag() { } };

struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;                                  // +0x00
    _Base_ptr _M_parent;                                 // +0x04
    _Base_ptr _M_left;                                   // +0x08
    _Base_ptr _M_right;                                  // +0x0c
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};

template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static _Base_ptr _M_increment(_Base_ptr _M_node);
};
template <class _Dummy>
_Rb_tree_node_base* _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    } else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_right) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        if (_M_node->_M_right != __y)
            _M_node = __y;
    }
    return _M_node;
}
typedef _Rb_global<bool> _Rb_global_inst;

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base::_Base_ptr _Base_ptr;
    _Base_ptr _M_node;
    _Rb_tree_base_iterator() : _M_node(0) { }
    _Rb_tree_base_iterator(_Base_ptr __x) : _M_node(__x) { }
};

template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef typename _Traits::reference reference;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Base_ptr __x) : _Rb_tree_base_iterator(__x) { }
    _Rb_tree_iterator(const _Self& __it) : _Rb_tree_base_iterator(__it._M_node) { }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};

template <class _InputIter, class _Tp>
_InputIter __fastcall __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
}

typedef _STL::_Rb_tree_iterator<NUISystem::CComponent*, _STL::_Const_traits<NUISystem::CComponent*> > TComponentSetIter;
template TComponentSetIter __fastcall _STL::__find<TComponentSetIter, NUISystem::CComponent*>(TComponentSetIter, TComponentSetIter, NUISystem::CComponent* const&, const _STL::input_iterator_tag&);