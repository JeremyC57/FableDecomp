// _STL::map::operator[](const CCharString& k) -- STLport subscript of map<CCharString, pair<long, long> >: lower_bound, insert (k, (0, 0)) when absent, return the mapped pair
// Leaf: ??A?$map@VCCharString@@U?$pair@JJ@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00756BE0, 127 bytes (+ int3 padding), __fastcall `ret 4`.
// Identity: Ego_r.pdb CTCHeroRememberCrimes::RegionNamesToCrimeDurationAndFine (+0xC) is map<CCharString, pair<long, long> >;
//   both callers sit in the one setter 0x00755680 (`m[k].first = duration; m[k].second = fine;` on this +0xC). The
//   manifest's map<CCharString, C2DVector> is propagated.
// Callees: _M_lower_bound @ 0x00756100 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950, CCharString copy
//   ctor @ 0x0099EC30 / dtor @ 0x0099EAE0, _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00756790.
// Shape: STLport 4.6 map::operator[] verbatim; STLport's pair default ctor value-initialises both longs (the two zero stores).
template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;     // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                           // +0x00
    CCharString();                                       // 0x0099E4B0
    CCharString(const CCharString& other);               // 0x0099EC30
    ~CCharString();                                      // 0x0099EAE0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
};

namespace _STL {

template <class _T1, class _T2> struct pair {             // STLport 4.6 (member templates on)
    _T1 first;
    _T2 second;
    pair() : first(_T1()), second(_T2()) {}
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) {}
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) {}
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    bool operator==(const _Rb_tree_iterator& __y) const { return _M_node == __y._M_node; }
};

template <class _Key, class _Tp, class _Compare = less<_Key> > class map {
public:
    typedef pair<const _Key, _Tp> value_type;
    typedef _Rb_tree_iterator<value_type> iterator;
    _Rb_tree_node_base* _M_header;                       // +0x00
    unsigned int        _M_node_count;                   // +0x04
    _Compare            _M_key_compare;                  // +0x08
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x00756100
    iterator insert_unique(iterator __position, const value_type& __x);  // 0x00756790 (_Rb_tree::insert_unique)
    iterator insert(iterator __position, const value_type& __x) { return insert_unique(__position, __x); }
    iterator lower_bound(const _Key& __k) { return iterator(_M_lower_bound(__k)); }
    iterator end() { return iterator(_M_header); }
    _Compare key_comp() const { return _M_key_compare; }
    _Tp& operator[](const _Key& __k)
    {
        iterator __i = lower_bound(__k);
        if (__i == end() || key_comp()(__k, (*__i).first))
            __i = insert(__i, value_type(__k, __default_constructed((_Tp*)0)));
        return (*__i).second;
    }
};

template pair<long, long> & map<CCharString, pair<long, long> >::operator[](const CCharString&);

} // namespace _STL