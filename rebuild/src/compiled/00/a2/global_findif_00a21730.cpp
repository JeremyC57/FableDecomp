// __find_if(first, last, value_equals<float, const CANavQuadTreeNode*> pred, input_iterator_tag) -- walk a multimap<float, const CANavQuadTreeNode*> in key order to the first entry mapping to the predicate's node
// Leaf: ??$__find_if@U?$_Rb_tree_iterator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@std@@V?$value_equals@MPBVCANavQuadTreeNode@@@@@std@@YI?AU?$_Rb_tree_iterator@U?$pair@$$CBMPBVCANavQuadTreeNode@@@std@@@0@U10@0V?$value_equals@MPBVCANavQuadTreeNode@@@@ABUinput_iterator_tag@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21730, 100 bytes, `ret 0Ch`. Callers: CNavigatorAStar::
//   CalculateNavigableWidthOfLine (0x00A1DAD9, the value is a node from 0x00A15140) and the out-of-line
//   `find_if` wrapper 0x00A21C80. The catalog label "BinarySearchTree_FindNextWithValue_int" is descriptive.
// Identity: Ego_r.pdb `find_if<multimap<float, const CANavQuadTreeNode*>::iterator, value_equals<float, const
//   CANavQuadTreeNode*>>` with `value_equals::operator()(pair<float, const CANavQuadTreeNode*>)` -- the pair BY VALUE
//   (QAE, non-const) -- and `value_equals(const CANavQuadTreeNode*)`. Under STLport the out-of-line body is
//   `__find_if(..., const input_iterator_tag&)`; the member name `Value` is descriptive.
// Calling convention: an STLport free template under /Gr = __fastcall: the hidden iterator result in ecx, the
//   `const input_iterator_tag&` (never read) in edx, first / last / predicate (all class objects) on the stack.
// Shape: `while (first != last && !pred(*first)) ++first;` with the rb-tree increment inlined. The by-value pair
//   parameter converts the multimap's pair<const float, ...> into the 8-byte frame temporary and only its `second`
//   store survives (the dead `mov [esp+0Ch],esi`); a `const pair&` parameter folds the temporary away (95 bytes).
class CANavQuadTreeNode;

namespace std {
struct input_iterator_tag { input_iterator_tag() {} };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
};
struct _Rb_tree_node_base {
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment()
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
    }
};
template <class _Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) { _M_node = __it._M_node; }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};
template <class _Value>
inline bool operator!=(const _Rb_tree_iterator<_Value>& __x, const _Rb_tree_iterator<_Value>& __y) { return __x._M_node != __y._M_node; }

template <class _InputIter, class _Predicate>
_InputIter __fastcall __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}

template <class _Key, class _Value> class value_equals {   // Ego_r.pdb value_equals<float, const CANavQuadTreeNode*>
public:
    _Value Value;                                       // descriptive member name
    value_equals(_Value value) : Value(value) {}
    bool operator()(std::pair<_Key, _Value> entry) { return entry.second == Value; }
};

typedef std::_Rb_tree_iterator<std::pair<const float, const CANavQuadTreeNode*> > CNodeByCostIterator;
template CNodeByCostIterator __fastcall std::__find_if(CNodeByCostIterator, CNodeByCostIterator, value_equals<float, const CANavQuadTreeNode*>, const std::input_iterator_tag&);