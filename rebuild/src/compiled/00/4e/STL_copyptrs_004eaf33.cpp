// _STL::__copy_ptrs(const CShoveHaPennyRegionDef* first, const CShoveHaPennyRegionDef* last, CShoveHaPennyRegionDef* result, const __false_type&) -- STLport __copy_ptrs for the non-POD shove-ha'penny region def: counted forward assignment of 48-byte elements, returns the advanced result
// Leaf: __copy_ptrs
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EAF33, 75 bytes, `ret 8` (/Gr __fastcall: first in ecx, last in edx; result and the
//   tag reference on the stack).
// Callers: vector<CShoveHaPennyRegionDef>::operator= @ 0x004EAE20.. (both copy branches, 0x004EAEA6 / 0x004EAED8).
// Label correction: the catalog's "VectorReallocateCopyConstruct" is descriptive; the element is CShoveHaPennyRegionDef
//   (0x30 bytes: the 0x28 CDefClassBase, Start +0x28, Score +0x2C, Ego_r.pdb) -- its _Construct 0x004EAF7E stores vtable
//   0x0124383C = CShoveHaPennyRegionDef (rtti-vtables.tsv).
// Callee: CDefClassBase::operator=(const CDefClassBase&) @ 0x00431F10 (catalogued CThingBuildingDef::operator=, propagated).
// Shape: the landed __copy_ptrs<CShopItemDef> 0x004E9BDA recipe: `__copy(first, last, result, random_access_iterator_tag(),
//   (ptrdiff_t*)0)` inlined (`n = (last - first) / 0x30`, the count kept in the dead result slot `dec [ebp+8]`, the
//   destination walked as `src + (result - first)`); CShoveHaPennyRegionDef's implicit operator= spelled out and
//   __forceinline (base operator= out of line, then Start, then Score). Frame-pointer size TU: `optimize("y", off)` +
//   `("s", on)`.
// Model: no engine header for CShoveHaPennyRegionDef; CDefClassBase as in the landed 0x004E9BDA (0x25 bytes, pack 1, the
//   derived members at +0x28).
#pragma optimize("y", off)
#pragma optimize("s", on)

#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                       // +0x04..+0x25
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
class CShoveHaPennyRegionDef : public CDefClassBase {   // vtable 0x0124383C, 0x30 bytes
public:
    float Start;                                     // +0x28
    float Score;                                     // +0x2c
    virtual void v00();
    __forceinline CShoveHaPennyRegionDef& operator=(const CShoveHaPennyRegionDef& o)
    {
        CDefClassBase::operator=(o);
        Start = o.Start;
        Score = o.Score;
        return *this;
    }
};
namespace _STL {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
template <class _Tp>
_Tp* __fastcall __copy_ptrs(const _Tp* first, const _Tp* last, _Tp* result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}
template CShoveHaPennyRegionDef* __fastcall __copy_ptrs<CShoveHaPennyRegionDef>(const CShoveHaPennyRegionDef* first,
    const CShoveHaPennyRegionDef* last, CShoveHaPennyRegionDef* result, const __false_type&);
}