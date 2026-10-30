// std::map::operator[](const long& __k) -- (map<long, map<long, CCountedPointer<CEngineSceneMap> > >, CEngineScene::MapList) STLport subscript: lower_bound the key, insert {key, empty inner map} at that hint when absent, return the inner map
// Leaf: ??A?$map@JV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@U?$less@J@2@V?$allocator@U?$pair@$$CBJV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@std@@@std@@@2@@std@@QAEAAV?$map@JV?$CCountedPointer@VCEngineSceneMap@@@@U?$less@J@std@@V?$allocator@U?$pair@$$CBJV?$CCountedPointer@VCEngineSceneMap@@@@@std@@@3@@1@ABJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B988E0, 302 bytes, thiscall, `ret 4`.
// Identity (two sources): (1) its only caller is CEngineScene::GetMapAt 0x00B94BF0 on CEngineScene +4 (Ego_r.pdb
//   CEngineScene::MapList, map<long, map<long, CCountedPointer<CEngineSceneMap> > >), and the PDB name set holds exactly
//   this `??A?$map@JV?$map@JV?$CCountedPointer@VCEngineSceneMap@@...@QAEAAV...@ABJ@Z`; (2) its callees are the landed
//   inner map copy ctor 0x00B97DE0, the outer tree's insert_unique(iterator, const value_type&) 0x00B985D0 and the inner
//   tree's _M_erase 0x00B97580; the default value is an empty inner map (a 0x1C-byte header node: 0x10 + the 12-byte
//   pair<const long, CCountedPointer<CEngineSceneMap> >); signed key compares (`jl` / `jge`) = long.
// Label: the manifest's "std_map_find_or_insert_variant" is descriptive.
// Callees: malloc 0x00BFEA0E (the T() temporary's header node), map<long, CCountedPointer<CEngineSceneMap> >::map(const
//   map&) 0x00B97DE0 (the value_type pair's second), _Rb_tree::insert_unique(iterator, const value_type&) 0x00B985D0
//   (iterator built in its stack slot, result through the dead `__k` slot), the inner _Rb_tree::_M_erase 0x00B97580 and
//   free 0x00BFEA14 (the two inline inner-map destructors: clear() + the base's header deallocate).
// Shape: STLport 4.6 map::operator[] (`__i = insert(__i, value_type(__k, _Tp()))`) with lower_bound inlined, as the
//   landed map<long, CEngineSceneRenderStats>::operator[] 0x00B97B60; the inner map's default ctor and dtor are inline.
extern "C" void* __cdecl malloc(unsigned int size);    // 0x00BFEA0E
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

template <class T> class CCountedPointer {             // 8 bytes: the object + its CCPPointerInfo count block
public:
    T*    Data;
    void* Info;
};
class CEngineSceneMap;

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    template <class _Tp1> allocator(const allocator<_Tp1>&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};

template <class _Arg1, class _Arg2, class _Result> struct binary_function { };
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;                               // +0x00
    _Base_ptr   _M_parent;                              // +0x04
    _Base_ptr   _M_left;                                // +0x08
    _Base_ptr   _M_right;                               // +0x0C
};

template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;                              // +0x10
};

struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp value_type; typedef _Tp& reference; typedef _Tp* pointer; };

template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Self& __it) { _M_node = __it._M_node; }
    typename _Traits::reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    bool operator==(const _Self& __y) const { return _M_node == __y._M_node; }
};

template <class _Tp, class _Alloc> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<_Tp> _Node;
    typedef allocator<_Node> _M_node_allocator_type;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(_M_node_allocator_type(__a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
    }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, _M_node_allocator_type> _M_header;   // +0x00
};

template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc = allocator<_Value> >
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef unsigned int size_type;
    typedef _Node* _Link_type;
    typedef typename _Base::allocator_type allocator_type;
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
protected:
    size_type _M_node_count;                            // +0x04
    _Compare  _M_key_compare;                           // +0x08

    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Rb_tree_node_base::_Color_type& _S_color(_Base_ptr __x) { return __x->_M_color; }
    static const _Key& _S_key(_Base_ptr __x) { return _KeyOfValue()(((_Link_type)__x)->_M_value_field); }

private:
    void _M_erase(_Link_type __x);                      // 0x00B97580 (inner tree)
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;   // used to distinguish header from root in iterator.operator++
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    _Rb_tree() : _Base(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Compare key_comp() const { return _M_key_compare; }
    iterator end() { return iterator(this->_M_header._M_data); }
    iterator insert_unique(iterator __position, const value_type& __x);   // 0x00B985D0 (outer tree)
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase(_M_root());
            _M_leftmost() = this->_M_header._M_data;
            _M_root() = 0;
            _M_rightmost() = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    iterator lower_bound(const key_type& __k)
    {
        _Link_type __y = this->_M_header._M_data;  // Last node which is not less than __k.
        _Link_type __x = _M_root();                // Current node.
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = (_Link_type)__x->_M_left;
            else
                __x = (_Link_type)__x->_M_right;
        return iterator(__y);
    }
};

template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, _Compare, _Alloc> _Rep_type;
    typedef typename _Rep_type::iterator iterator;
private:
    _Rep_type _M_t;
public:
    map() : _M_t() { }
    map(const map<_Key, _Tp, _Compare, _Alloc>& __x);   // 0x00B97DE0 (inner map)
    iterator end() { return _M_t.end(); }
    iterator lower_bound(const key_type& __x) { return _M_t.lower_bound(__x); }
    _Compare key_comp() const { return _M_t.key_comp(); }
    iterator insert(iterator __position, const value_type& __x) { return _M_t.insert_unique(__position, __x); }
    _Tp& operator[](const key_type& __k)
    {
        iterator __i = lower_bound(__k);
        // __i->first is greater than or equivalent to __k.
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, _Tp()));
        return (*__i).second;
    }
};

}   // namespace std

template std::map<long, CCountedPointer<CEngineSceneMap> >& std::map<long, std::map<long, CCountedPointer<CEngineSceneMap> > >::operator[](const long&);