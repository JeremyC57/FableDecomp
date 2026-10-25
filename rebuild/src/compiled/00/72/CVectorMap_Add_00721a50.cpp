// CVectorMap::Add(const long& key, const CTCHeroMorph::CTextureLayer& value) -- append a {key, texture layer} pair to the map's vector (in place, or through the out-of-line growth path) and mark the map dirty (re-sorted on the next lookup)
// Leaf: ?Add@?$CVectorMap@JVCTextureLayer@CTCHeroMorph@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00721A50, 125 bytes, `ret 8`. No direct callers (an out-of-line copy of the
//   inline Add). Instantiation: CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> >
//   (Ego_r.pdb: CTCHeroMorph::AddCompositeTexture / UpdateTattoos take this map).
// Label correction: the catalog "Vector_InsertRange_CCompositeBlendDesc" is descriptive; the body is the CVectorMap::Add
//   shape of the landed 0x006FCF40 / 0x00759570 (`Data.push_back(pair(key, value)); Dirty = true;`).
// Callees: CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc&) @ 0x005BA48A (x2: the pair temporary,
//   then the in-place element), vector::_M_insert_overflow(pos, x, const __false_type&, fill_len, atend) @ 0x00721060.
// Layout: CVectorMap = STLport vector (12 bytes retail) + empty Compare (+0xc) + bool Dirty (+0xd); the element is
//   pair<long, CTextureLayer { long TextureID; CCompositeBlendDesc Blend; }> (24 bytes).
// Shape: the pair temporary is built by STLport's `pair(const T1& a, const T2& b)` in a 24-byte frame; push_back is
//   inlined (placement copy through pair's memberwise copy constructor behind the null test, `add [esi+4],18h`; the
//   growth call's empty tag on the dead key slot); the Dirty store is duplicated into both arms.
// Recipe (chunk 422): both out-of-line callees that receive the temporary's address (the blend copy constructor and
//   _M_insert_overflow) get same-TU `auto_inline(off)` + `__declspec(noinline)` stand-in bodies that do not capture it;
//   VC7.1 then treats the temporary's leading words as uncaptured and loads key and TextureID before the first store
//   of the placement copy, as retail. With either callee opaque the store lands between the two loads.
inline void* operator new(unsigned int, void* p) { return p; }

enum ECompositeBlendType { COMPOSITE_BLEND_NONE = 0 };
struct CRGBColour {                                // D3DCOLOR byte order
    unsigned char B, G, R, A;
};
struct CCompositeBlendDesc {                       // Ego_r.pdb, 16 bytes
    unsigned long       TextureIndex;              // +0x00
    ECompositeBlendType BlendType;                 // +0x04
    CRGBColour          DiffuseColour;             // +0x08
    CRGBColour          SpecularColour;            // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

class CTCHeroMorph {
public:
    class CTextureLayer {                          // Ego_r.pdb, 20 bytes
    public:
        long                TextureID;             // +0x00
        CCompositeBlendDesc Blend;                 // +0x04
    };
};

template <class K, class V> class CKeyPairCompareLess {   // empty
public:
    CKeyPairCompareLess() { }
};

namespace _STL {
class __false_type { public: __false_type() { } };

template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage; // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend);   // 0x00721060
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

#pragma auto_inline(off)
// Same-TU stand-in for 0x005BA48A (memberwise; never keeps `other`).
__declspec(noinline) CCompositeBlendDesc::CCompositeBlendDesc(const CCompositeBlendDesc& other)
    : TextureIndex(other.TextureIndex), BlendType(other.BlendType), DiffuseColour(other.DiffuseColour), SpecularColour(other.SpecularColour)
{
}
// Same-TU stand-in for 0x00721060 (constructs the element in place and adopts the end; never keeps `__x`).
template <class _Tp, class _Alloc>
__declspec(noinline) void _STL::vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                                                        size_type __fill_len, bool __atend)
{
    _Construct(__position, __x);
    this->_M_finish = __position + __fill_len;
    if (__atend)
        this->_M_end_of_storage._M_data = this->_M_finish;
}
#pragma auto_inline(on)

template <class K, class V, class C> class CVectorMap {
public:
    typedef _STL::pair<K, V> value_type;
    _STL::vector<value_type> Data;                 // +0x00 (retail STLport vector, 12 bytes)
    C                        Compare;              // +0x0c (empty)
    bool                     Dirty;                // +0x0d
    void Add(const K& key, const V& value);
};

template <class K, class V, class C>
void CVectorMap<K, V, C>::Add(const K& key, const V& value)
{
    Data.push_back(value_type(key, value));
    Dirty = true;
}

template void CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> >::Add(
    const long& key, const CTCHeroMorph::CTextureLayer& value);