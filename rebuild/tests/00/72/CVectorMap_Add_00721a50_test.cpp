// Behaviour test for 0x00721A50 (source body copied from src/00721a50.cpp; out-of-line callees stubbed below).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _MSC_VER
inline void* operator new(unsigned int, void* p) { return p; }
#else
#include <new>
#define __fastcall
#define __cdecl
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#endif

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

namespace std {
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
__declspec(noinline) void std::vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
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
    typedef std::pair<K, V> value_type;
    std::vector<value_type> Data;                 // +0x00 (retail STLport vector, 12 bytes)
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

// ---- test harness ----
typedef CVectorMap<long, CTCHeroMorph::CTextureLayer, CKeyPairCompareLess<long, CTCHeroMorph::CTextureLayer> > TMap;
typedef std::pair<long, CTCHeroMorph::CTextureLayer> TPair;
static unsigned char g_layerBuf[sizeof(CTCHeroMorph::CTextureLayer)];
static const CTCHeroMorph::CTextureLayer& MakeLayer(long id, unsigned long tex)
{
    CTCHeroMorph::CTextureLayer& l = *(CTCHeroMorph::CTextureLayer*)g_layerBuf;
    memset(&l, 0, sizeof l);
    l.TextureID = id; l.Blend.TextureIndex = tex; l.Blend.BlendType = COMPOSITE_BLEND_NONE;
    l.Blend.DiffuseColour.R = 7; l.Blend.SpecularColour.A = 9;
    return l;
}
int main()
{
    int ok = 1;
    static unsigned char store[4 * sizeof(TPair)];
    TMap m;
    m.Data._M_start = m.Data._M_finish = (TPair*)store;
    m.Data._M_end_of_storage._M_data = (TPair*)store + 2;
    m.Dirty = false;
    long k1 = 11, k2 = 22, k3 = 33;
    m.Add(k1, MakeLayer(101, 1001));
    if (!m.Dirty || m.Data._M_finish != (TPair*)store + 1) ok = 0;
    m.Dirty = false;
    m.Add(k2, MakeLayer(202, 2002));
    if (!m.Dirty || m.Data._M_finish != (TPair*)store + 2) ok = 0;
    m.Dirty = false;
    m.Add(k3, MakeLayer(303, 3003));            // full: the growth path
    if (!m.Dirty || m.Data._M_finish != (TPair*)store + 3 || m.Data._M_end_of_storage._M_data != m.Data._M_finish) ok = 0;
    TPair* p = (TPair*)store;
    if (p[0].first != 11 || p[0].second.TextureID != 101 || p[0].second.Blend.TextureIndex != 1001 ||
        p[0].second.Blend.DiffuseColour.R != 7 || p[0].second.Blend.SpecularColour.A != 9) ok = 0;
    if (p[1].first != 22 || p[1].second.TextureID != 202 || p[1].second.Blend.TextureIndex != 2002) ok = 0;
    if (p[2].first != 33 || p[2].second.TextureID != 303 || p[2].second.Blend.TextureIndex != 3003) ok = 0;
    printf("00721a50_TEST %s\n", ok ? "PASS" : "FAIL");
    return ok ? 0 : 1;
}