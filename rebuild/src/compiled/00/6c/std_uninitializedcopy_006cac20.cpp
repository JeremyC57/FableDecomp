// std::__uninitialized_copy(const CCompositeBlendDesc* first, const CCompositeBlendDesc* last, CCompositeBlendDesc* result, const __false_type&) -- CCompositeBlendDesc elements: two dwords, then both colours through CRGBColour::Set, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCCompositeBlendDesc@FableCopy_006cac20@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CAC20, length 139 bytes, `ret 8`.
// Element: CCompositeBlendDesc (Ego_r.pdb, 16 bytes: TextureIndex +0, BlendType +4, DiffuseColour +8, SpecularColour
// +0xC), the element of CEngineTextureDecal::OverlaidBlends / CEngineTextureMorph::Blends; the caller is the landed
// vector<CCompositeBlendDesc>::operator= 0x006CADE0. The implicit copy runs CRGBColour's copy constructor, which goes
// through the inline Set(r, g, b, a): all four channel bytes are loaded (arguments right to left, one spilled to the
// `push ecx` frame word) before they are stored, which gives retail's byte shuffles and the `[esp+18h]` spills.
// Callees: none (everything inline).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006cac20 {
struct CRGBColour {                                     // D3DCOLOR byte order (Ego_r.pdb CRGBColour union: B, G, R, A)
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
enum ECompositeBlendType { COMPOSITE_BLEND_TYPE_0 };
struct CCompositeBlendDesc {                            // Ego_r.pdb, 16 bytes
    unsigned long       TextureIndex;                   // +0x00
    ECompositeBlendType BlendType;                      // +0x04
    CRGBColour          DiffuseColour;                  // +0x08
    CRGBColour          SpecularColour;                 // +0x0C
};
}
namespace std {
FableCopy_006cac20::CCompositeBlendDesc* __fastcall __uninitialized_copy(const FableCopy_006cac20::CCompositeBlendDesc* first, const FableCopy_006cac20::CCompositeBlendDesc* last,
    FableCopy_006cac20::CCompositeBlendDesc* result, const __false_type&)
{
    FableCopy_006cac20::CCompositeBlendDesc* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006cac20::CCompositeBlendDesc(*first);
    return cur;
}
}