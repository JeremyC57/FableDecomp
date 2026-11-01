// CTCInventoryMap::PanMap(C2DVector delta) -- pan the zooming map by delta and, once it has moved, select the list entry of the region whose world-map offset lies nearest the map centre (within 128 units)
// Leaf: ?PanMap@CTCInventoryMap@@QAEXVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FB140, 877 bytes, thiscall `ret 8`, ebp frame aligned to 8 (Ego_r.pdb
//   ?PanMap@CTCInventoryMap@@QAEXVC2DVector@@@Z).
// Callees: CRegion::GetWorldMapOffset() const 0x006BBF40 (FableWin ?GetWorldMapOffset@CRegion@@QBE?BVC2DVector@@XZ: it returns
//   PDef's C2DVector at +0xAC, its neighbour 0x006BBF70 the one at +0xB4 = Ego_r CRegionDef WorldMapOffset +0xB8 / NameGraphicOffset
//   +0xC0 less 12; the manifest and the landed 0x005FB8F0 call it GetNameGraphicOffset, NAME_FIXES),
//   std::_Rb_tree_iterator<pair<const long, CRegion>, _Nonconst_traits<..> >::operator++() 0x00613630 (the STLport COMDAT, ICF-folded
//   with the other instantiations; landed 0x00613630 source), the zooming map's virtual Pan (+0x238) and C2DVector getters
//   (+0x30 / +0x40 / +0x248), the map list's state (+0x158), selected value (+0x238), item vector (+0x28C) and select (+0x2B4),
//   and each list item's state-flag set / clear (+0x0C / +0x10 on its second base). Virtual slot names are descriptive.
// Layout: Ego_r.pdb CTCInventoryMap pmap_real / MapList / MapRegions (map<long, CRegion>) at retail +0x188 / +0x214 / +0x218
//   (12-byte STLport containers; landed 0x005FB8F0 SetPanMode reads the same offsets).
// Shape: the real STLport 4.6 _Rb_tree / map text (as the landed 0x00613630 operator++ source carries it): the default-constructed
//   iterator (`_M_node = 0`) is the best-region iterator and the free operator!= on _Rb_tree_base_iterator; with the stand-in
//   iterator VC7.1 put the best iterator in edi and the loop took a different allocation and an alignment pad (873v877,
//   the prior's residue; 6 probes on declaration order / node locals made no difference).
#include <math.h>
class C2DVector {
public:
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
};
class CRegion {
public:
    const C2DVector GetWorldMapOffset() const;          // 0x006BBF40
};
extern "C" void* __cdecl malloc(unsigned int __size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* __p);                 // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* __p) { return __p; }
inline void __cdecl operator delete(void*, void*) { }
namespace std {
template <class _Tp> class allocator {
public:
    typedef _Tp value_type;
    allocator() { }
    allocator(const allocator<_Tp>&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _T1, class _T2> struct pair {
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) { }
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
};
template <class _Arg1, class _Arg2, class _Result> struct binary_function {
    typedef _Arg1 first_argument_type;
    typedef _Arg2 second_argument_type;
    typedef _Result result_type;
};
template <class _Tp> struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};
template <class _Tp> struct _Identity {
    const _Tp& operator()(const _Tp& __x) const { return __x; }
};
template <class _Pair> struct _Select1st {
    const typename _Pair::first_type& operator()(const _Pair& __x) const { return __x.first; }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last) { for (; __first != __last; ++__first) _Destroy(&*__first); }
struct input_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct _Rb_tree_node_base {
    typedef bool _Color_type;
    typedef _Rb_tree_node_base* _Base_ptr;
    _Color_type _M_color;
    _Base_ptr   _M_parent;
    _Base_ptr   _M_left;
    _Base_ptr   _M_right;
    static _Base_ptr __cdecl _S_minimum(_Base_ptr __x) { while (__x->_M_left != 0) __x = __x->_M_left; return __x; }
    static _Base_ptr __cdecl _S_maximum(_Base_ptr __x) { while (__x->_M_right != 0) __x = __x->_M_right; return __x; }
};
template <class _Value> struct _Rb_tree_node : public _Rb_tree_node_base {
    _Value _M_value_field;
};
template <class _Dummy> class _Rb_global {
public:
    typedef _Rb_tree_node_base* _Base_ptr;
    static void __cdecl _Rebalance(_Rb_tree_node_base* __x, _Rb_tree_node_base*& __root);                 // 0x0042971D
    static _Rb_tree_node_base* __cdecl _Rebalance_for_erase(_Rb_tree_node_base* __z, _Rb_tree_node_base*& __root,
                                                            _Rb_tree_node_base*& __leftmost,
                                                            _Rb_tree_node_base*& __rightmost);              // 0x0042A34E
    static _Rb_tree_node_base* __cdecl _M_increment(_Rb_tree_node_base* __x);
    static _Rb_tree_node_base* __cdecl _M_decrement(_Rb_tree_node_base* __x);
};
typedef _Rb_global<bool> _Rb_global_inst;
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_increment(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_right != 0) {
        _M_node = _M_node->_M_right;
        while (_M_node->_M_left != 0)
            _M_node = _M_node->_M_left;
    }
    else {
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
template <class _Dummy>
_Rb_tree_node_base* __cdecl _Rb_global<_Dummy>::_M_decrement(_Rb_tree_node_base* _M_node)
{
    if (_M_node->_M_color == false && _M_node->_M_parent->_M_parent == _M_node)
        _M_node = _M_node->_M_right;
    else if (_M_node->_M_left != 0) {
        _Base_ptr __y = _M_node->_M_left;
        while (__y->_M_right != 0)
            __y = __y->_M_right;
        _M_node = __y;
    }
    else {
        _Base_ptr __y = _M_node->_M_parent;
        while (_M_node == __y->_M_left) {
            _M_node = __y;
            __y = __y->_M_parent;
        }
        _M_node = __y;
    }
    return _M_node;
}
template <class _Iterator> struct iterator_traits {
    typedef typename _Iterator::iterator_category iterator_category;
    typedef typename _Iterator::value_type value_type;
    typedef typename _Iterator::difference_type difference_type;
};
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
__distance(const _InputIterator& __first, const _InputIterator& __last, const input_iterator_tag&)
{
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    _InputIterator __it(__first);
    while (__it != __last) {
        ++__it; ++__n;
    }
    return __n;
}
template <class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type __cdecl
distance(const _InputIterator& __first, const _InputIterator& __last)
{
    return __distance(__first, __last, typename iterator_traits<_InputIterator>::iterator_category());
}
struct _Rb_tree_base_iterator {
    typedef _Rb_tree_node_base* _Base_ptr;
    _Base_ptr _M_node;
};
inline bool operator==(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node == __y._M_node; }
inline bool operator!=(const _Rb_tree_base_iterator& __x, const _Rb_tree_base_iterator& __y) { return __x._M_node != __y._M_node; }
template <class _Tp> struct _Nonconst_traits {
    typedef _Tp value_type;
    typedef _Tp& reference;
    typedef _Tp* pointer;
};
template <class _Tp> struct _Const_traits {
    typedef _Tp value_type;
    typedef const _Tp& reference;
    typedef const _Tp* pointer;
};
template <class _Value, class _Traits> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    typedef bidirectional_iterator_tag iterator_category;
    typedef int difference_type;
    typedef _Value value_type;
    typedef typename _Traits::reference reference;
    typedef typename _Traits::pointer pointer;
    typedef _Rb_tree_iterator<_Value, _Traits> _Self;
    typedef _Rb_tree_node<_Value>* _Link_type;
    _Rb_tree_iterator() { _M_node = 0; }
    _Rb_tree_iterator(_Link_type __x) { _M_node = __x; }
    _Rb_tree_iterator(const _Rb_tree_iterator<_Value, _Nonconst_traits<_Value> >& __it) { _M_node = __it._M_node; }
    reference operator*() const { return _Link_type(_M_node)->_M_value_field; }
    pointer operator->() const { return &(operator*()); }
    _Self& operator++() { _M_node = _Rb_global_inst::_M_increment(_M_node); return *this; }
    _Self operator++(int) { _Self __tmp = *this; _M_node = _Rb_global_inst::_M_increment(_M_node); return __tmp; }
    _Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }
    _Self operator--(int) { _Self __tmp = *this; _M_node = _Rb_global_inst::_M_decrement(_M_node); return __tmp; }
};
template <class _Tp, class _Alloc> struct _Rb_tree_base {
    typedef _Rb_tree_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    _Rb_tree_base(const allocator_type& __a) : _M_header(allocator<_Node>(), (_Node*)0) { _M_header._M_data = _M_header.allocate(1); }
    ~_Rb_tree_base() { _M_header.deallocate(_M_header._M_data, 1); }
protected:
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
};
template <class _Key, class _Value, class _KeyOfValue, class _Compare, class _Alloc>
class _Rb_tree : public _Rb_tree_base<_Value, _Alloc> {
    typedef _Rb_tree_base<_Value, _Alloc> _Base;
protected:
    typedef _Rb_tree_node_base* _Base_ptr;
    typedef _Rb_tree_node<_Value> _Node;
    typedef bool _Color_type;
public:
    typedef _Key key_type;
    typedef _Value value_type;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef _Rb_tree_node<_Value>* _Link_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef typename _Base::allocator_type allocator_type;
    enum { _S_rb_tree_red = false, _S_rb_tree_black = true };
protected:
    _Link_type _M_create_node(const value_type& __x)
    {
        _Link_type __tmp = this->_M_header.allocate(1);
        _Construct(&__tmp->_M_value_field, __x);
        return __tmp;
    }
    _Link_type _M_clone_node(_Link_type __x)
    {
        _Link_type __tmp = _M_create_node(__x->_M_value_field);
        __tmp->_M_color = __x->_M_color;
        __tmp->_M_left = 0;
        __tmp->_M_right = 0;
        return __tmp;
    }
protected:
    size_type _M_node_count;
    _Compare _M_key_compare;
    _Link_type& _M_root() const { return (_Link_type&)this->_M_header._M_data->_M_parent; }
    _Link_type& _M_leftmost() const { return (_Link_type&)this->_M_header._M_data->_M_left; }
    _Link_type& _M_rightmost() const { return (_Link_type&)this->_M_header._M_data->_M_right; }
    static _Link_type& __cdecl _S_left(_Link_type __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Link_type __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& __cdecl _S_parent(_Link_type __x) { return (_Link_type&)(__x->_M_parent); }
    static reference __cdecl _S_value(_Link_type __x) { return __x->_M_value_field; }
    static const _Key& __cdecl _S_key(_Link_type __x) { return _KeyOfValue()(_S_value(__x)); }
    static _Color_type& __cdecl _S_color(_Link_type __x) { return (_Color_type&)(__x->_M_color); }
    static _Link_type& __cdecl _S_left(_Base_ptr __x) { return (_Link_type&)(__x->_M_left); }
    static _Link_type& __cdecl _S_right(_Base_ptr __x) { return (_Link_type&)(__x->_M_right); }
    static _Link_type& __cdecl _S_parent(_Base_ptr __x) { return (_Link_type&)(__x->_M_parent); }
    static reference __cdecl _S_value(_Base_ptr __x) { return ((_Link_type)__x)->_M_value_field; }
    static const _Key& __cdecl _S_key(_Base_ptr __x) { return _KeyOfValue()(_S_value(_Link_type(__x))); }
    static _Color_type& __cdecl _S_color(_Base_ptr __x) { return (_Color_type&)(_Link_type(__x)->_M_color); }
    static _Link_type __cdecl _S_minimum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_minimum(__x); }
    static _Link_type __cdecl _S_maximum(_Link_type __x) { return (_Link_type)_Rb_tree_node_base::_S_maximum(__x); }
public:
    typedef _Rb_tree_iterator<value_type, _Nonconst_traits<value_type> > iterator;
    typedef _Rb_tree_iterator<value_type, _Const_traits<value_type> > const_iterator;
private:
    iterator _M_insert(_Base_ptr __x, _Base_ptr __y, const value_type& __v, _Base_ptr __w = 0);
    _Link_type _M_copy(_Link_type __x, _Link_type __p);
    void _M_erase(_Link_type __x);
public:
    _Rb_tree() : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(_Compare()) { _M_empty_initialize(); }
    _Rb_tree(const _Compare& __comp) : _Rb_tree_base<_Value, _Alloc>(allocator_type()), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    _Rb_tree(const _Compare& __comp, const allocator_type& __a) : _Rb_tree_base<_Value, _Alloc>(__a), _M_node_count(0), _M_key_compare(__comp) { _M_empty_initialize(); }
    ~_Rb_tree() { clear(); }
    _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& operator=(const _Rb_tree<_Key, _Value, _KeyOfValue, _Compare, _Alloc>& __x);
private:
    void _M_empty_initialize()
    {
        _S_color(this->_M_header._M_data) = _S_rb_tree_red;
        _M_root() = 0;
        _M_leftmost() = this->_M_header._M_data;
        _M_rightmost() = this->_M_header._M_data;
    }
public:
    iterator begin() { return iterator(_M_leftmost()); }
    const_iterator begin() const { return const_iterator(_M_leftmost()); }
    iterator end() { return iterator(this->_M_header._M_data); }
    const_iterator end() const { return const_iterator(this->_M_header._M_data); }
    bool empty() const { return _M_node_count == 0; }
    size_type size() const { return _M_node_count; }
    pair<iterator, bool> insert_unique(const value_type& __x);
    iterator insert_equal(const value_type& __x);
    iterator insert_unique(iterator __position, const value_type& __x);
    iterator insert_equal(iterator __position, const value_type& __x);
    void erase(iterator __position)
    {
        _Link_type __y = (_Link_type)_Rb_global_inst::_Rebalance_for_erase(__position._M_node,
                                                                           this->_M_header._M_data->_M_parent,
                                                                           this->_M_header._M_data->_M_left,
                                                                           this->_M_header._M_data->_M_right);
        _Destroy(&__y->_M_value_field);
        this->_M_header.deallocate(__y, 1);
        --_M_node_count;
    }
    size_type erase(const key_type& __x);
    void erase(iterator __first, iterator __last)
    {
        if (__first == begin() && __last == end())
            clear();
        else
            while (__first != __last) erase(__first++);
    }
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
    iterator find(const key_type& __x) { return iterator(_M_find(__x)); }
    const_iterator find(const key_type& __x) const { return const_iterator(_M_find(__x)); }
private:
    _Rb_tree_node<_Value>* _M_find(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        if (__y == this->_M_header._M_data || _M_key_compare(__k, _S_key(__y)))
            __y = this->_M_header._M_data;
        return __y;
    }
    _Link_type _M_lower_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (!_M_key_compare(_S_key(__x), __k))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
    _Link_type _M_upper_bound(const key_type& __k) const
    {
        _Link_type __y = this->_M_header._M_data;
        _Link_type __x = _M_root();
        while (__x != 0)
            if (_M_key_compare(__k, _S_key(__x)))
                __y = __x, __x = _S_left(__x);
            else
                __x = _S_right(__x);
        return __y;
    }
public:
    size_type count(const key_type& __x) const;
    iterator lower_bound(const key_type& __x) { return iterator(_M_lower_bound(__x)); }
    const_iterator lower_bound(const key_type& __x) const { return const_iterator(_M_lower_bound(__x)); }
    iterator upper_bound(const key_type& __x) { return iterator(_M_upper_bound(__x)); }
    const_iterator upper_bound(const key_type& __x) const { return const_iterator(_M_upper_bound(__x)); }
    pair<iterator, iterator> equal_range(const key_type& __x) { return pair<iterator, iterator>(lower_bound(__x), upper_bound(__x)); }
};
template <class _Key, class _Tp, class _Compare = less<_Key>, class _Alloc = allocator<pair<const _Key, _Tp> > >
class map {
public:
    typedef _Key key_type;
    typedef _Tp data_type;
    typedef _Tp mapped_type;
    typedef pair<const _Key, _Tp> value_type;
    typedef _Compare key_compare;
private:
    typedef _Rb_tree<key_type, value_type, _Select1st<value_type>, key_compare, _Alloc> _Rep_type;
    _Rep_type _M_t;
public:
    typedef typename _Rep_type::iterator iterator;
    typedef typename _Rep_type::const_iterator const_iterator;
    typedef typename _Rep_type::size_type size_type;
    iterator begin() { return _M_t.begin(); }
    const_iterator begin() const { return _M_t.begin(); }
    iterator end() { return _M_t.end(); }
    const_iterator end() const { return _M_t.end(); }
    bool empty() const { return _M_t.empty(); }
    size_type size() const { return _M_t.size(); }
};
}   // namespace std
typedef std::map<long, CRegion> CRegionMap;
typedef CRegionMap::iterator CRegionIterator;
namespace NUISystem {
struct CStateFlags {
    virtual void s00(); virtual void s01(); virtual void s02();
    virtual void SetFlag(long flag);                    // slot 3 (+0x0C)
    virtual void ClearFlag(long flag);                  // slot 4 (+0x10)
};
struct CListItemBase {
    virtual void b00();
};
struct CListItem : CListItemBase, CStateFlags {
};
struct CListItemVector {
    CListItem** _M_start;
    CListItem** _M_finish;
    CListItem** _M_end_of_storage;
    CListItem*& operator[](unsigned long n) { return *(_M_start + n); }
};
class CZoomingComponent { public:   // Ego_r.pdb pmap_real type; FableWin ?Pan / ?GetZoomCenter@CZoomingComponent@NUISystem, ?GetZoom@CComponent@NUISystem (slot names descriptive)
    virtual void z000(); virtual void z001(); virtual void z002(); virtual void z003(); virtual void z004(); virtual void z005();
    virtual void z006(); virtual void z007(); virtual void z008(); virtual void z009(); virtual void z010(); virtual void z011();
    virtual C2DVector GetZoom();                               // slot 12 (+0x30)
    virtual void z013(); virtual void z014(); virtual void z015();
    virtual C2DVector GetMousePos();                           // slot 16 (+0x40)
    virtual void z017(); virtual void z018(); virtual void z019(); virtual void z020(); virtual void z021(); virtual void z022();
    virtual void z023(); virtual void z024(); virtual void z025(); virtual void z026(); virtual void z027(); virtual void z028();
    virtual void z029(); virtual void z030(); virtual void z031(); virtual void z032(); virtual void z033(); virtual void z034();
    virtual void z035(); virtual void z036(); virtual void z037(); virtual void z038(); virtual void z039(); virtual void z040();
    virtual void z041(); virtual void z042(); virtual void z043(); virtual void z044(); virtual void z045(); virtual void z046();
    virtual void z047(); virtual void z048(); virtual void z049(); virtual void z050(); virtual void z051(); virtual void z052();
    virtual void z053(); virtual void z054(); virtual void z055(); virtual void z056(); virtual void z057(); virtual void z058();
    virtual void z059(); virtual void z060(); virtual void z061(); virtual void z062(); virtual void z063(); virtual void z064();
    virtual void z065(); virtual void z066(); virtual void z067(); virtual void z068(); virtual void z069(); virtual void z070();
    virtual void z071(); virtual void z072(); virtual void z073(); virtual void z074(); virtual void z075(); virtual void z076();
    virtual void z077(); virtual void z078(); virtual void z079(); virtual void z080(); virtual void z081(); virtual void z082();
    virtual void z083(); virtual void z084(); virtual void z085(); virtual void z086(); virtual void z087(); virtual void z088();
    virtual void z089(); virtual void z090(); virtual void z091(); virtual void z092(); virtual void z093(); virtual void z094();
    virtual void z095(); virtual void z096(); virtual void z097(); virtual void z098(); virtual void z099(); virtual void z100();
    virtual void z101(); virtual void z102(); virtual void z103(); virtual void z104(); virtual void z105(); virtual void z106();
    virtual void z107(); virtual void z108(); virtual void z109(); virtual void z110(); virtual void z111(); virtual void z112();
    virtual void z113(); virtual void z114(); virtual void z115(); virtual void z116(); virtual void z117(); virtual void z118();
    virtual void z119(); virtual void z120(); virtual void z121(); virtual void z122(); virtual void z123(); virtual void z124();
    virtual void z125(); virtual void z126(); virtual void z127(); virtual void z128(); virtual void z129(); virtual void z130();
    virtual void z131(); virtual void z132(); virtual void z133(); virtual void z134(); virtual void z135(); virtual void z136();
    virtual void z137(); virtual void z138(); virtual void z139(); virtual void z140(); virtual void z141();
    virtual void Pan(C2DVector delta);                         // slot 142 (+0x238)
    virtual void z143(); virtual void z144(); virtual void z145();
    virtual C2DVector GetCentre();                             // slot 146 (+0x248)
};
class CList { public:   // Ego_r.pdb MapList type (NUISystem::CList; slot names descriptive)
    virtual void l000(); virtual void l001(); virtual void l002(); virtual void l003(); virtual void l004(); virtual void l005();
    virtual void l006(); virtual void l007(); virtual void l008(); virtual void l009(); virtual void l010(); virtual void l011();
    virtual void l012(); virtual void l013(); virtual void l014(); virtual void l015(); virtual void l016(); virtual void l017();
    virtual void l018(); virtual void l019(); virtual void l020(); virtual void l021(); virtual void l022(); virtual void l023();
    virtual void l024(); virtual void l025(); virtual void l026(); virtual void l027(); virtual void l028(); virtual void l029();
    virtual void l030(); virtual void l031(); virtual void l032(); virtual void l033(); virtual void l034(); virtual void l035();
    virtual void l036(); virtual void l037(); virtual void l038(); virtual void l039(); virtual void l040(); virtual void l041();
    virtual void l042(); virtual void l043(); virtual void l044(); virtual void l045(); virtual void l046(); virtual void l047();
    virtual void l048(); virtual void l049(); virtual void l050(); virtual void l051(); virtual void l052(); virtual void l053();
    virtual void l054(); virtual void l055(); virtual void l056(); virtual void l057(); virtual void l058(); virtual void l059();
    virtual void l060(); virtual void l061(); virtual void l062(); virtual void l063(); virtual void l064(); virtual void l065();
    virtual void l066(); virtual void l067(); virtual void l068(); virtual void l069(); virtual void l070(); virtual void l071();
    virtual void l072(); virtual void l073(); virtual void l074(); virtual void l075(); virtual void l076(); virtual void l077();
    virtual void l078(); virtual void l079(); virtual void l080(); virtual void l081(); virtual void l082(); virtual void l083();
    virtual void l084(); virtual void l085();
    virtual long GetState();                                   // slot 86 (+0x158)
    virtual void l087(); virtual void l088(); virtual void l089(); virtual void l090(); virtual void l091(); virtual void l092();
    virtual void l093(); virtual void l094(); virtual void l095(); virtual void l096(); virtual void l097(); virtual void l098();
    virtual void l099(); virtual void l100(); virtual void l101(); virtual void l102(); virtual void l103(); virtual void l104();
    virtual void l105(); virtual void l106(); virtual void l107(); virtual void l108(); virtual void l109(); virtual void l110();
    virtual void l111(); virtual void l112(); virtual void l113(); virtual void l114(); virtual void l115(); virtual void l116();
    virtual void l117(); virtual void l118(); virtual void l119(); virtual void l120(); virtual void l121(); virtual void l122();
    virtual void l123(); virtual void l124(); virtual void l125(); virtual void l126(); virtual void l127(); virtual void l128();
    virtual void l129(); virtual void l130(); virtual void l131(); virtual void l132(); virtual void l133(); virtual void l134();
    virtual void l135(); virtual void l136(); virtual void l137(); virtual void l138(); virtual void l139(); virtual void l140();
    virtual void l141();
    virtual long GetSelectedValue();                           // slot 142 (+0x238)
    virtual void l143(); virtual void l144(); virtual void l145(); virtual void l146(); virtual void l147(); virtual void l148();
    virtual void l149(); virtual void l150(); virtual void l151(); virtual void l152(); virtual void l153(); virtual void l154();
    virtual void l155(); virtual void l156(); virtual void l157(); virtual void l158(); virtual void l159(); virtual void l160();
    virtual void l161(); virtual void l162();
    virtual CListItemVector* GetItems();                       // slot 163 (+0x28C)
    virtual void l164(); virtual void l165(); virtual void l166(); virtual void l167(); virtual void l168(); virtual void l169();
    virtual void l170(); virtual void l171(); virtual void l172();
    virtual void SelectItem(CListItem* item);                  // slot 173 (+0x2B4)
};
}
using NUISystem::CZoomingComponent;
using NUISystem::CList;
class CTCInventoryMap { public:
    unsigned char _pad00[0x188];
    CZoomingComponent* pmap_real;                       // +0x188
    unsigned char _pad18c[0x214 - 0x18c];
    CList*         MapList;                             // +0x214
    CRegionMap     MapRegions;                          // +0x218
    void PanMap(C2DVector delta);
};

void CTCInventoryMap::PanMap(C2DVector delta)
{
    if (MapList && pmap_real && MapList->GetState() == 3) {
        pmap_real->Pan(delta);
        if (sqrt(delta.X * delta.X + delta.Y * delta.Y) < 0.01)
            return;
        C2DVector zoom = pmap_real->GetZoom();
        C2DVector centre = pmap_real->GetCentre();
        C2DVector pos = (pmap_real->GetMousePos() - centre) * -1.0f;
        pos.X = pos.X / pmap_real->GetZoom().X;
        pos.Y = pos.Y / pmap_real->GetZoom().Y;
        if (MapRegions.size() == 0)
            return;
        bool first = true;
        float best = 0.0f;
        CRegionIterator bestIt;
        for (CRegionIterator it = MapRegions.begin(); it != MapRegions.end(); ++it) {
            C2DVector offset = it->second.GetWorldMapOffset();
            C2DVector d = offset - pos;
            float dist = (float)sqrt(d.X * d.X + d.Y * d.Y);
            if ((dist < best || first) && dist < 128.0f) {
                best = dist;
                first = false;
                bestIt = it;
            }
        }
        if (first == true)
            return;
        long key = bestIt->first;
        if (key == MapList->GetSelectedValue())
            return;
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(2);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(3);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(4);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->ClearFlag(5);
        MapList->SelectItem((*MapList->GetItems())[key]);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(2);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(3);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(4);
        (*MapList->GetItems())[MapList->GetSelectedValue()]->SetFlag(5);
    }
}