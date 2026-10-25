// std::map<CComponent*, pair<long, long> >::insert(iterator position, const value_type& x) -- hinted insert: forward to the tree's insert_unique(position, x) and return its iterator
// Leaf: ?insert@?$map@PAVCComponent@NUISystem@@U?$pair@JJ@std@@U?$less@PAVCComponent@NUISystem@@@4@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@std@@@2@U32@ABU?$pair@QAVCComponent@NUISystem@@U?$pair@JJ@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005B31CF, 29 bytes, thiscall `ret 0Ch` (hidden result, position, x): `push x ;
//   mov edx,position ; push ecx ; mov eax,esp ; push result ; mov [eax],edx ; call 0x005B2EFE ; mov eax,result` -- the
//   iterator's user copy constructor copies position into the reserved slot; `this` (the tree _M_t at +0) stays in ecx.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 455 "the 'TransferIn'/'FindInsertPos' rows are insert(hint, v)"; hand455
//   README callee notes): the only caller is the landed std::map<NUISystem::CComponent*, pair<long, long> >::operator[] 0x005B322B (call at 0x005B3265),
//   whose `i = insert(i, value_type(k, T()))` binds this Leaf -> 005b31cf in reloc_bindings; the body is STLport 4.6 _map.h
//   `insert(iterator, const value_type&) { return _M_t.insert_unique(position, x); }` with insert_unique(iterator, const
//   value_type&) at 0x005B2EFE (not landed). The landed CForwarder row was a shape placeholder (manifest label "TransferIn").
// Model: the landed operator[] rows' map (three template parameters) and one-parameter _Rb_tree_iterator, so the Leaf is
//   the bound one; the tree member is spelled as STLport's _Rb_tree<K, value_type, _Select1st, C, allocator>.
#pragma optimize("s", on)
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct less { };
template <class T> class allocator { };
template <class P> struct _Select1st { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {   // one parameter, as the landed operator[] rows spell it
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
};
template <class K, class V, class KoV, class C, class A> class _Rb_tree {
public:
    typedef _Rb_tree_iterator<V> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    C                   _M_key_compare;                  // +0x08
    iterator insert_unique(iterator position, const V& v);   // 0x005B2EFE
};
template <class K, class T, class C = less<K> > class map {      // three parameters, as the landed operator[] rows spell it
public:
    typedef pair<const K, T> value_type;
    typedef _Rb_tree<K, value_type, _Select1st<value_type>, C, allocator<value_type> > _Rep_type;
    typedef typename _Rep_type::iterator iterator;
    _Rep_type _M_t;                                      // +0x00
    iterator insert(iterator position, const value_type& x);
};
template <class K, class T, class C>
typename map<K, T, C>::iterator map<K, T, C>::insert(iterator position, const value_type& x)   // _map.h
{
    return _M_t.insert_unique(position, x);
}
}
namespace std {
template map<NUISystem::CComponent*, pair<long, long> >::iterator map<NUISystem::CComponent*, pair<long, long> >::insert(iterator, const value_type&);
}
