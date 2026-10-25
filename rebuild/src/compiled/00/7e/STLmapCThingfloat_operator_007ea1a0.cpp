// _STL::map<CThing*, float>::operator[](CThing* const& key) -- the distance slot of a thing in CTCAvoidRegionExit::DistMap: lower_bound walk, and a (key, 0.0f) pair inserted at that hint when the key is absent
// Grade: RELOCATION_MATCH -- chunk-422 recipe: insert_unique(hint, v) 0x007E9DE0 receives the pair temporary's address; a same-TU
//   noinline stand-in that READS v but never keeps its address lets VC7.1 store the pair's 0.0f after the hidden-result push, as
//   retail (a stand-in that ignores v lets VC drop the pair stores altogether: 101 bytes).
// Leaf: ??A?$map@PAVCThing@@M
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA1A0, 115 bytes (`ret 4`). STLport map<CThing*, float>::operator[] instantiated
//   for CTCAvoidRegionExit::DistMap (0x013BAF00); its one caller is CTCAvoidRegionExit::UpdateMarkers (`DistMap[parent] =
//   dist` @ 0x007E994A). Catalog label `RBTree_LowerBound` names only the first half.
// Callees: _Rb_tree::insert_unique(iterator hint, const value_type&) @ 0x007E9DE0 (manifest "RBTree_InsertOrFind": hidden
//   iterator result, the hint by value built in its argument slot).
// Layout: STLport _Rb_tree header-node pointer at +0, node parent +4, left +8, right +0xc, key +0x10, value +0x14.
// Shape: STLport 4.6 operator[] verbatim -- `iterator i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first))
//   i = insert(i, value_type(k, T())); return (*i).second;` with the STLport iterator (body-assignment constructors over
//   _Rb_tree_base_iterator); retail homes the returned iterator in the dead key-argument slot.
#include <stddef.h>

class CThing;

namespace _STL {

template <class T1, class T2> struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};

template <class T> class allocator { };

template <class Pair> struct _Select1st {
    const typename Pair::first_type& operator()(const Pair& x) const { return x.first; }
};

struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;              // +0x04
    _Rb_tree_node_base* _M_left;                // +0x08
    _Rb_tree_node_base* _M_right;               // +0x0c
};

template <class Value> struct _Rb_tree_node : _Rb_tree_node_base {
    Value _M_value_field;                       // +0x10
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};

template <class Value> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {   // STLport: body assignments
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    Value& operator*() const { return static_cast<_Rb_tree_node<Value>*>(_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc> class _Rb_tree {
public:
    typedef _Rb_tree_node<Value> _Node;
    typedef _Rb_tree_iterator<Value> iterator;
    _Rb_tree_node_base* _M_header;              // +0x00
    size_t _M_node_count;                       // +0x04
    Compare _M_key_compare;                     // +0x08

    static const Key& _S_key(_Rb_tree_node_base* x) { return KeyOfValue()(((_Node*)x)->_M_value_field); }
    _Rb_tree_node_base*& _M_root() const { return _M_header->_M_parent; }
    iterator end() { return iterator(_M_header); }
    Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const Key& k)
    {
        _Rb_tree_node_base* y = _M_header;      // last node which is not less than k
        _Rb_tree_node_base* x = _M_root();      // current node
        while (x != 0)
            if (!_M_key_compare(_S_key(x), k))
                y = x, x = x->_M_left;
            else
                x = x->_M_right;
        return iterator(y);
    }
    iterator insert_unique(iterator position, const Value& v);   // 0x00991030
};

#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x007E9DE0 (reads v, never keeps its address).
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
__declspec(noinline) typename _Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
_Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const Value& v)
{
    _M_node_count += *(const unsigned char*)&v.first + *(const unsigned char*)&v.second;
    return position;
}
#pragma auto_inline(on)

template <class Key, class T, class Compare = less<Key>, class Alloc = allocator<pair<const Key, T> > > class map {
public:
    typedef Key key_type;
    typedef pair<const Key, T> value_type;
    typedef _Rb_tree<Key, value_type, _Select1st<value_type>, Compare, Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;

    iterator lower_bound(const key_type& k) { return _M_t.lower_bound(k); }
    iterator end() { return _M_t.end(); }
    Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator position, const value_type& v) { return _M_t.insert_unique(position, v); }
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        // i->first is greater than or equivalent to k.
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, T()));
        return (*i).second;
    }
};

}   // namespace _STL

template class _STL::map<CThing*, float>;