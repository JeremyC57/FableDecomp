// std::__uninitialized_copy<CCompositeBlendDesc*, CCompositeBlendDesc*>(first, last, result, const __false_type&) --
// STLport: copy-construct [first, last) into raw storage at result (implicit CCompositeBlendDesc copy, colours through
// CRGBColour::Set), return the end
// Leaf: ??$__uninitialized_copy@PAVCCompositeBlendDesc@@PAV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CAC20, length 139 bytes, `ret 8`.
// Identity (FINDINGS chunk 387; the landed leaf named a const first argument in a private namespace): the callers pass
//   mutable pointers -- vector<CCompositeBlendDesc>::_M_insert_overflow 0x006CAF10 (0x006CAF59 `(_M_start, position)`,
//   0x006CAFA6 `(position, _M_finish)`) and vector<CCompositeBlendDesc>::push_back 0x006CB010 (0x006CB083, its
//   _M_insert_overflow inline) -- while the const instantiation <const CCompositeBlendDesc*, CCompositeBlendDesc*> is the
//   separate body 0x005BAF28 (vector copy ctor 0x005BB346, operator= 0x006CADE0, _M_allocate_and_copy 0x006CAD30: the
//   STLport `(const_pointer)__x._M_start` arms). No identical-code folding in retail, so the two bodies are two
//   template instantiations. CCompositeBlendDesc is a class (Ego_r.pdb ??0CCompositeBlendDesc@@QAE@ABV0@@Z).
// Element: CCompositeBlendDesc (Ego_r.pdb, 16 bytes: TextureIndex +0, BlendType +4, DiffuseColour +8, SpecularColour
//   +0xC). The implicit copy runs CRGBColour's copy constructor, which goes through the inline Set(r, g, b, a): all four
//   channel bytes are loaded (arguments right to left, one spilled to the `push ecx` frame word) before they are stored.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
//   in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
//   ++cur) _Construct(&*cur, *first);` -- the placement new null-tests the destination.
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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CCompositeBlendDesc* __fastcall __uninitialized_copy(CCompositeBlendDesc*, CCompositeBlendDesc*,
    CCompositeBlendDesc*, const __false_type&);
}
