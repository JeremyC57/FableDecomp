// _STL::map<unsigned long, float>::operator[](const unsigned long& k) -- find a group's weight by index, inserting 0.0f when absent (COptimisedVertex::Groups)
// Grade: DIFFER(115v115) -- one scheduling slot: retail stores the float 0.0f of value_type(k, float()) after the result-pointer push (`push edx ; mov [esp+1Ch],0`), the lane stores it before the iterator copy (`mov eax,esp ; mov [esp+18h],0 ; mov [eax],edx`)
// Leaf: ??A?$map@KM
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD6420, 0x73 bytes, thiscall, `ret 4`, returns &(*i).second.
// Identity: the callee 0x00AD5030 is the landed _Rb_tree::insert_unique(iterator, const value_type&) of
// pair<unsigned, float> (node: key +0x10, float +0x14); Ego_r.pdb QuadricOptimiserInternals::COptimisedVertex::Groups
// is map<unsigned long, float>. Catalog "BTreeMap_FindOrInsert" is descriptive.
// Callee: _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00AD5030 (iterator by value built in the argument
// slot through its user copy constructor, result through the hidden pointer -- homed in the dead `k` slot).
// Shape: STLport operator[] (same as the landed map<unsigned long, CSectionDesc> 0x009F1470): inline lower_bound
// (unsigned `jb` descent), `if (i == end() || key_comp()(k, (*i).first)) i = insert(i, value_type(k,
// __default_constructed((float*)0)))`; the float zero lands as an immediate store after the pushes.
namespace _STL {

template <class _T1, class _T2> struct pair
{
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less
{
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base
{
    char                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base
{
    _Value _M_value_field;                          // +0x10
};

template <class _Value> struct _Rb_tree_iterator
{
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() {}
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map
{
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    typedef _Rb_tree_node<value_type> _Node;

    _Rb_tree_node_base* _M_header;                  // +0x00 (alloc proxy)
    unsigned int        _M_node_count;              // +0x04
    _Compare            _M_key_compare;             // +0x08

    static const _Key& _S_key(_Rb_tree_node_base* __x) { return ((_Node*)__x)->_M_value_field.first; }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator lower_bound(const _Key& __k)
    {
        _Rb_tree_node_base* __y = _M_header;        // last node which is not less than __k
        _Rb_tree_node_base* __x = _M_header->_M_parent;
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = __x->_M_left;
            else
                __x = __x->_M_right;
        return iterator(__y);
    }
    iterator insert_unique(iterator __position, const value_type& __x);    // 0x00AD5030 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template class map<unsigned long, float>;

} // namespace _STL
