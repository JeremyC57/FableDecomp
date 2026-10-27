// std::__uninitialized_fill_n(CCompositeBlendDesc* first, unsigned int n, const CCompositeBlendDesc& x, const __false_type&) -- STLport: copy-construct n raw blend descriptors from x, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCCompositeBlendDesc@@IV1@@std@@YIPAVCCompositeBlendDesc@@PAV1@IABV1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CACB0, length 128 bytes, `__fastcall` (first in ecx, n in edx, x and
//   the empty tag reference on the stack), `ret 8`.
// Identity: single caller vector<CCompositeBlendDesc>::_M_insert_overflow 0x006CAF10 (0x006CAF8A, the `fill_len != 1`
//   arm: `ecx = position, edx = fill_len, push x, push &tag`; the `fill_len == 1` arm calls the out-of-line copy
//   constructor ??0CCompositeBlendDesc@@QAE@ABV0@@Z 0x005BA48A directly, and the next callee is __uninitialized_copy
//   0x006CAC20, landed). Element and copy as in the landed 0x006CAC20: 16 bytes, the colours copied through
//   CRGBColour's copy constructor -> inline Set(r, g, b, a) (all four channel bytes loaded before they are stored).
// Shape: STLport 4.x verbatim, `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x); return __cur;` from a /Gr TU;
//   the placement new null-tests the slot, two channel bytes spill to the `push ecx` word and the dead `x` slot.
inline void* operator new(unsigned int, void* where) { return where; }

class CRGBColour {                                      // D3DCOLOR byte order (Ego_r.pdb CRGBColour union: B, G, R, A)
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};

enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0 };

class CCompositeBlendDesc {                             // Ego_r.pdb, 16 bytes
public:
    unsigned long       TextureIndex;                   // +0x00
    ECompositeBlendType BlendType;                      // +0x04
    CRGBColour          DiffuseColour;                  // +0x08
    CRGBColour          SpecularColour;                 // +0x0C
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __value) { new ((void*)__p) _T1(__value); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CCompositeBlendDesc* __fastcall __uninitialized_fill_n(CCompositeBlendDesc*, unsigned int,
    const CCompositeBlendDesc&, const __false_type&);
}