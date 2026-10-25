// _STL::map::operator[](const pair<long, long>& k) -- STLport subscript of CEngineSurfaceCompositionManager's map<pair<long, long>, CEngineTextureMorph>: lower_bound, insert (k, CEngineTextureMorph()) when absent, return the morph record
// Leaf: ??A?$map@U?$pair@JJ@_STL@@VCEngineTextureMorph@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2F5E0, 171 bytes, __fastcall `ret 4`.
// Identity: Ego_r.pdb CEngineSurfaceCompositionManager::GeneratedMorphs (+0xB38) is map<pair<long, long>,
//   CEngineTextureMorph>; only caller CEngineSurfaceCompositionManager::UpdateDiskCacheTexture (0x00B2D921). In TLC the
//   record is 20 bytes: OriginalTextureIndex +0, MorphID +4, vector<CCompositeBlendDesc> Blends +8 (12-byte STLport
//   vector of the 16-byte blend descs; the PDB's 24 bytes are a later 16-byte vector). The manifest type agrees.
// Callees: _M_lower_bound @ 0x00B2E830 (node pointer in eax), vector<CCompositeBlendDesc> copy ctor @ 0x005BB346 (a
//   size-TU COMDAT), _Rb_tree::insert_unique(iterator, const value_type&) @ 0x00B2F240 (result through the dead `k`
//   slot), free @ 0x00BFEA14.
// Shape: STLport 4.6 map::operator[] with the pair key's STLport operator< inline (`jg`/`jl` on first, `jge` on second);
//   the default record (two zero longs, empty vector) is copied memberwise into the value_type temporary (the longs fold
//   to zero stores, the vector through its copy ctor); the value_type's ~vector frees the null-tested start.
//   Same-TU stand-in (as 0x009D4810): the vector copy constructor's body sits in this TU under auto_inline(off), so
//   VC7.1 sees it only reads its argument and folds the default record's ~vector away, as retail's whole-program
//   knowledge did.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

struct CRGBColour { unsigned long Value; };
enum ECompositeBlendType { COMPOSITE_BLEND_NONE };
class CCompositeBlendDesc {                                 // 16 bytes (Ego_r.pdb; PDB class)
public:
    unsigned long       TextureIndex;                       // +0x00
    ECompositeBlendType BlendType;                          // +0x04
    CRGBColour          DiffuseColour;                      // +0x08
    CRGBColour          SpecularColour;                     // +0x0c
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

template <class _T1, class _T2>
inline bool operator<(const pair<_T1, _T2>& __x, const pair<_T1, _T2>& __y)
{
    return __x.first < __y.first || (!(__y.first < __x.first) && __x.second < __y.second);
}

template <class _Tp> inline _Tp __default_constructed(_Tp*) { return _Tp(); }   // _STLP_DEFAULT_CONSTRUCTED

template <class _Tp> struct less {
    bool operator()(const _Tp& __x, const _Tp& __y) const { return __x < __y; }
};

template <class _Tp> inline _Tp* __uninitialized_copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    for (; __first != __last; ++__first, ++__result)
        new (__result) _Tp(*__first);
    return __result;
}

template <class _Tp> class vector {                      // STLport 4.6 vector, malloc allocator
public:
    _Tp* _M_start;                                       // +0
    _Tp* _M_finish;                                      // +4
    _Tp* _M_end_of_storage;                              // +8
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
    vector(const vector<_Tp>& __x);                      // 0x005BB346 (body below)
    ~vector()
    {
        if (_M_start != 0)
            free(_M_start);
    }
};

// Same-TU copy of the out-of-line copy constructor 0x005BB346 (allocate exactly x.size() slots, then copy the
// elements); under auto_inline(off) it stays a call, and VC7.1 sees that it only reads `x`.
#pragma auto_inline(off)
template <class _Tp> vector<_Tp>::vector(const vector<_Tp>& __x)
{
    unsigned int __n = (unsigned int)(__x._M_finish - __x._M_start);
    _M_start = 0;
    _M_finish = 0;
    _M_end_of_storage = 0;
    _M_start = __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0;
    _M_finish = _M_start;
    _M_end_of_storage = _M_start + __n;
    _M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, _M_start);
}
#pragma auto_inline(on)

} // namespace _STL

class CEngineTextureMorph {                                 // 20 bytes in TLC (PDB class)
public:
    long                                OriginalTextureIndex;   // +0x00
    long                                MorphID;                // +0x04
    _STL::vector<CCompositeBlendDesc>   Blends;                 // +0x08
    CEngineTextureMorph() : OriginalTextureIndex(0), MorphID(0) {}
};

namespace _STL {

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
    _Rb_tree_node_base* _M_lower_bound(const _Key& __k) const;          // 0x00B2E830
    iterator insert_unique(iterator __position, const value_type& __x);  // 0x00B2F240 (_Rb_tree::insert_unique)
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

template CEngineTextureMorph& map<pair<long, long>, CEngineTextureMorph>::operator[](const pair<long, long>&);

} // namespace _STL