// Behaviour test for the multimap<float, const CANavQuadTreeNode*> __find_if (0x00A21730): in-order walk of a
// hand-built tree finds the first entry whose mapped node is the target, returns last when none matches, and handles an empty range.
#include <cstdio>

class CANavQuadTreeNode { public: int Id; };

namespace _STL {
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
    _Rb_tree_node(const _Value& v) : _M_value_field(v) {}
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
_InputIter __find_if(_InputIter __first, _InputIter __last, _Predicate __pred, const input_iterator_tag&)
{
    while (__first != __last && !__pred(*__first))
        ++__first;
    return __first;
}
}

template <class _Key, class _Value> class value_equals {
public:
    _Value Value;
    value_equals(_Value value) : Value(value) {}
    bool operator()(_STL::pair<_Key, _Value> entry) { return entry.second == Value; }
};
typedef value_equals<float, const CANavQuadTreeNode*> CIsMappedNode;

typedef _STL::pair<const float, const CANavQuadTreeNode*> SValue;
typedef _STL::_Rb_tree_node<SValue> SNode;
typedef _STL::_Rb_tree_iterator<SValue> SIter;

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int ok = 1;
    CANavQuadTreeNode nodes[6];
    for (int i = 0; i < 6; ++i) nodes[i].Id = i;
    // header + 5 entries keyed 10..50 (key k maps to nodes[k/10]); tree: 30 root, 20 (left, with left 10), 50 (right, with left 40)
    _STL::_Rb_tree_node_base header;
    SNode n10(SValue(10.0f, &nodes[1])), n20(SValue(20.0f, &nodes[2])), n30(SValue(30.0f, &nodes[3])), n40(SValue(40.0f, &nodes[4])), n50(SValue(50.0f, &nodes[5]));
    header._M_parent = &n30; header._M_left = &n10; header._M_right = &n50;
    n30._M_parent = &header; n30._M_left = &n20; n30._M_right = &n50;
    n20._M_parent = &n30; n20._M_left = &n10; n20._M_right = 0;
    n10._M_parent = &n20; n10._M_left = 0; n10._M_right = 0;
    n50._M_parent = &n30; n50._M_left = &n40; n50._M_right = 0;
    n40._M_parent = &n50; n40._M_left = 0; n40._M_right = 0;

    SIter first; first._M_node = &n10;
    SIter last; last._M_node = &header;
    _STL::input_iterator_tag tag;
    float expectKey[6] = { 0.0f, 10.0f, 20.0f, 30.0f, 40.0f, 50.0f };
    for (int k = 1; k <= 5; ++k) {
        CIsMappedNode pred(&nodes[k]);
        SIter r = _STL::__find_if(first, last, pred, tag);
        if (r._M_node == &header || !nearf((*r).first, expectKey[k]) || (*r).second != &nodes[k]) ok = 0;
    }
    CIsMappedNode none(&nodes[0]);
    if (_STL::__find_if(first, last, none, tag)._M_node != &header) ok = 0;    // no match -> last
    SIter mid; mid._M_node = &n40;
    CIsMappedNode early(&nodes[2]);
    if (_STL::__find_if(mid, last, early, tag)._M_node != &header) ok = 0;     // match outside [40, end) -> last
    if (_STL::__find_if(last, last, early, tag)._M_node != &header) ok = 0;    // empty range

    if (!ok) { std::printf("00a21730_TEST FAIL\n"); return 1; }
    std::printf("00a21730_TEST PASS\n");
    return 0;
}