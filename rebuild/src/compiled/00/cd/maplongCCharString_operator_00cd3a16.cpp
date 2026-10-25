// map<long, CCharString>::operator[](const long& key) -- STLport map subscript: lower_bound the key, insert (key, CCharString()) at that hint when absent, return the mapped string
#pragma optimize("y", off)
#pragma optimize("s", on)
// Leaf: ??A?$map@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD3A16, 99 bytes, `__fastcall` (map in ecx, the key reference on the
//   stack: `ret 4`).
// Label: the catalog name "CPersistTraits_TransferInHelper" is descriptive; the body is STLport 4.6's map::operator[]
//   for the long -> CCharString map whose _M_erase is 0x00CD29F0 (node: key +0x10, string +0x14).
// Callees: _Rb_tree::_M_lower_bound(const long&) @ 0x00CD28F4 (node pointer in eax), CCharString() @ 0x0099E4B0,
//   pair<const long, CCharString>(const long&, const CCharString&) @ 0x00CD20CE, map::insert(iterator, const
//   value_type&) @ 0x00CD36C2 (hidden iterator result), ~CCharString @ 0x0099EAE0 (x2).
// Shape: frame-pointer size TU. `__i == end() || key_comp()(k, (*__i).first)` (less<long> inline: `cmp ; jge`); the
//   `__default_constructed((_Tp*)0)` CCharString temporary is homed in the dead key slot [ebp+8]; the hint iterator
//   is passed by value through its user copy constructor (built in the argument slot: `push ecx ; mov eax,esp ;
//   mov [eax],esi`); the pair temporary's string and the default string die in that order; the result is
//   `&(*__i).second` (`lea eax,[esi+14h]`).
class CCharString {
public:
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class T> struct less {
    bool operator()(const T& a, const T& b) const { return a < b; }
};
template <class T> class allocator { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b);                     // 0x00CD20CE (pair<const long, CCharString>, out of line)
};
template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }
struct _Rb_tree_node_base {
    typedef _Rb_tree_node_base* _Base_ptr;
    bool      _M_color;
    _Base_ptr _M_parent;
    _Base_ptr _M_left;
    _Base_ptr _M_right;
};
template <class V> struct _Rb_tree_node : _Rb_tree_node_base {
    V _M_value_field;                                   // +0x10
};
struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
};
template <class V> struct _Rb_tree_iterator : _Rb_tree_base_iterator {
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& other) const { return _M_node == other._M_node; }
};
template <class K, class T, class C = less<K>, class A = allocator<pair<const K, T> > >
class map {
public:
    typedef K key_type;
    typedef pair<const K, T> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                      // +0x00 (the tree's header proxy)
    unsigned int        _M_node_count;                  // +0x04
    C                   _M_key_compare;                 // +0x08
    _Rb_tree_node_base* _M_lower_bound(const K& k) const;   // 0x00CD28F4
    iterator lower_bound(const K& k) { return iterator(_M_lower_bound(k)); }
    iterator end() { return iterator(_M_header); }
    C key_comp() const { return _M_key_compare; }
    iterator insert(iterator position, const value_type& x);   // 0x00CD36C2
    T& operator[](const key_type& k)
    {
        iterator i = lower_bound(k);
        if (i == end() || key_comp()(k, (*i).first))
            i = insert(i, value_type(k, __default_constructed((T*)0)));
        return (*i).second;
    }
};
template class map<long, CCharString>;
}