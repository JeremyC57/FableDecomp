// _STL::map::operator[](const pair<long,long>& k) -- STLport subscript of the quadric optimiser's edge map (vertex pair -> counted contraction pointer): lower_bound, insert (k, null pointer) when absent, return the mapped pointer
// Leaf: ??A?$map@U?$pair@JJ@_STL@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD6740, 137 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: map<pair<long,long>, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> > (Ego_r.pdb; the
//   landed _M_lower_bound 0x00AD24F0 documents the family); single caller 0x00ACB913 indexes it with the (min, max)
//   unique-vertex pair of each primitive edge and fills an empty slot with a new contraction. The catalog label
//   "std::map_InsertOrFind" is descriptive.
// Callees: _Rb_tree::_M_lower_bound(k) @ 0x00AD24F0 (node pointer in eax), _Rb_tree::insert_unique(iterator, const
//   value_type&) @ 0x00AD5630 (the iterator result returned through the dead `k` argument slot, the hint iterator built
//   in its own argument slot by the iterator copy constructor), operator delete @ 0x00BFE9BC (the counted pointer's info
//   block when the temporary's release drops the last reference; the deleter is called through Info->DeleteFunc).
// Shape: STLport 4.6 map::operator[] verbatim: `i = lower_bound(k); if (i == end() || key_comp()(k, (*i).first)) i =
//   insert(i, value_type(k, __default_constructed((_Tp*)0))); return (*i).second;` -- the pair<long,long> operator<
//   is the signed lexicographic `jg`/`jl` test; the default CCountedPointer temporary folds away, the value_type
//   temporary's release re-reads Info after the call (insert_unique is out of line); &(*i).second is node + 0x18.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

struct CCPPointerInfo {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        CCPPointerInfo* info = Info;
        if (info && --info->RefCount == 0) {
            info->DeleteFunc(info->Data);
            delete info;
        }
    }
};

namespace QuadricOptimiserInternals {
class CVertexPairContraction;
}

namespace _STL {

template <class _T1, class _T2> struct pair {             // STLport 4.6 (member templates on)
    typedef _T1 first_type;
    typedef _T2 second_type;
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

struct _Rb_tree_node_base {
    bool                _M_color;                        // +0x00
    _Rb_tree_node_base* _M_parent;                       // +0x04
    _Rb_tree_node_base* _M_left;                         // +0x08
    _Rb_tree_node_base* _M_right;                        // +0x0C
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;                               // +0x10
};
template <class _Value> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Rb_tree_iterator& __it) : _M_node(__it._M_node) { }
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
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x00AD24F0
    iterator insert_unique(iterator __position, const value_type& __x);  // 0x00AD5630 (_Rb_tree::insert_unique; stand-in below)
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

#pragma auto_inline(off)
// Same-TU stand-in for insert_unique(hint, v) 0x00AD5630 (chunk-422 recipe, as the landed CCountedPointer map 0x00A2D810):
// reads v and MAY write its counted pointer (so VC7.1 keeps the pair's release after the call), never keeps the address.
template <class _Key, class _Tp, class _Compare>
__declspec(noinline) typename map<_Key, _Tp, _Compare>::iterator
map<_Key, _Tp, _Compare>::insert_unique(iterator __position, const value_type& __x)
{
    _M_node_count += *(const unsigned char*)&__x.first;
    if (__position._M_node == 0)                         // never (the hint is a node or the header): a possible write to v
        const_cast<_Tp&>(__x.second) = _Tp();
    return __position;
}
#pragma auto_inline(on)

template CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction>&
map<pair<long, long>, CCountedPointer<QuadricOptimiserInternals::CVertexPairContraction> >::operator[](const pair<long, long>&);

} // namespace _STL