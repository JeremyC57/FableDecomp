// std::__uninitialized_fill_n(CDecalEntry* first, unsigned int n, const CDecalEntry& x, const __false_type&) -- STLport non-POD fill of n copies of a ((long, long), CEngineTextureDecal) decal-map entry into raw storage: each copy is the key pair, the decal's two words and a deep copy of its blend vector; returns the end
// Leaf: ??$__uninitialized_fill_n@PAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@std@@IU12@@std@@YIPAU?$pair@U?$pair@JJ@std@@VCEngineTextureDecal@@@0@PAU10@IABU10@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEE710, length 155 bytes (`__fastcall`: ecx = first, edx = n;
//   `ret 8`: the value reference and the empty tag reference).
// Identity: renamed (sweep1, 2026-10-25) from CRandomAppearanceMorph::UninitializedFillN (CMorphRecord; the manifest
//   CRandomAppearanceMorph::CopyBodyPartMeshes is propagated). Its caller is the landed
//   std::vector<pair<pair<long,long>,CEngineTextureDecal> >::_M_insert_overflow 0x00BEF830 (call 0x00BEF8D6), which
//   carries this body as its same-TU __uninitialized_fill_n; the element is that vector's 0x1C-byte CDecalEntry
//   (FINDINGS chunk 432; agent432 NOTES): the key pair<long, long>, then Ego_r.pdb CEngineTextureDecal
//   {OriginalTextureIndex, DecalID, vector<CCompositeBlendDesc> OverlaidBlends} (20 bytes in retail: the STLport vector
//   is 12).
// Callees: malloc @ 0x00BFEA0E (the allocator), the blend descriptors' __uninitialized_copy @ 0x005BAF28 (landed;
//   `__fastcall` first/last in ecx/edx, then result and the tag reference).
// Shape: STLport 4.6 `for (; n > 0; --n, ++cur) _Construct(&*cur, x)` (null-checked placement new) over STLport's explicit
//   memberwise pair copy and the decal's implicit copy: two key words, two decal words, then the vector copy constructor
//   (_Vector_base(n, alloc) with the alloc proxy, then the inner __uninitialized_copy with a fresh __false_type tag, which
//   VC places in the dead value-argument slot `[esp+18h]`). `__fastcall` on the template definition.
#include <new>
#include <stddef.h>

extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

class CRGBColour {                                  // Ego_r.pdb: union of {B, G, R, A} and the packed word
public:
    unsigned char B, G, R, A;
};
class CCompositeBlendDesc {                         // Ego_r.pdb, 16 bytes
public:
    unsigned long TextureIndex;                     // +0x00
    long          BlendType;                        // +0x04 ECompositeBlendType
    CRGBColour    DiffuseColour;                    // +0x08
    CRGBColour    SpecularColour;                   // +0x0c
    CCompositeBlendDesc(const CCompositeBlendDesc& other);   // 0x005BA48A
};

namespace std {
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }   // STLport's explicit memberwise copy
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result,
                                            const __false_type&);      // 0x005BAF28 (blend descriptors)

template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
};
template <class Value, class T, class Alloc> class _STLP_alloc_proxy : public Alloc {
public:
    Value _M_data;
    _STLP_alloc_proxy(const Alloc& a, Value p) : Alloc(a), _M_data(p) { }
};
template <class T, class Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int n, const Alloc& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0)
    {
        _M_start = _M_end_of_storage.allocate(n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + n;
    }
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*, T, Alloc> _M_end_of_storage;
};
// The decals' blend list: STLport vector<CCompositeBlendDesc> (12 bytes in retail); the copy constructor inline.
template <class T, class Alloc> class vector : public _Vector_base<T, Alloc> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    Alloc get_allocator() const { return Alloc(); }
    vector(const vector& x) : _Vector_base<T, Alloc>(x.size(), x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const T*)x._M_start, (const T*)x._M_finish, this->_M_start, __false_type());
    }
};
}   // namespace std

// Ego_r.pdb CEngineTextureDecal (20 bytes in retail: the STLport vector is 12).
class CEngineTextureDecal {
public:
    long OriginalTextureIndex;                      // +0x00
    long DecalID;                                   // +0x04
    std::vector<CCompositeBlendDesc, std::allocator<CCompositeBlendDesc> > OverlaidBlends;   // +0x08
};

typedef std::pair<long, long> CDecalKey;
typedef std::pair<CDecalKey, CEngineTextureDecal> CDecalEntry;   // 0x1c bytes

template CDecalEntry* __fastcall std::__uninitialized_fill_n(CDecalEntry*, unsigned int, const CDecalEntry&,
                                                              const std::__false_type&);
