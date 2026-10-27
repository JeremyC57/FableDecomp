// std::__copy_ptrs(CCardDef* first, CCardDef* last, CCardDef* result, const __false_type&) -- STLport assignment copy of def records: n = last - first, then `*result = *first` n times; returns the end of the copy
// Flags: /Gr
// Leaf: ??$__copy_ptrs@PAVCCardDef@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FA22C, 75 bytes, __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`,
//   frame pointer.
// Identity (proven): reloc_bindings ??$__copy_ptrs@PAVCCardDef@@PAV1@@std@@YIPAVCCardDef@@PAV1@00ABU__false_type@0@@Z; caller the landed vector<CCardDef>::erase 0x004FA1F0.
// Callees: CParentDefClassBase::operator= 0x00431ED0 per element (the implicit CCardDef::operator= inline: the base part, then the two own dwords).
// Shape (family K124, representative 0x004EABE5): STLport 4.6 `__copy_ptrs(..., const __false_type&)` -> `__copy(first, last, result,
//   random_access_iterator_tag(), (int*)0)` (__forceinline); frame/size TU (`push 30h ; pop esi ; idiv esi`, the count in the
//   result slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
class CParentDefClassBase {                              // retail 0x39 bytes, byte-packed (CDefClassBase 0x25, InstantiationName, SubDefInfoMap)
public:
    virtual void v00();
    unsigned char _body[0x35];                           // +0x04 (not touched here)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)
class CCardDef : public CParentDefClassBase {            // retail 0x44 bytes (Ego_r.pdb - 4, RTTI base CParentDefClassBase)
public:
    long CardName;                                       // +0x3C
    long CardVal;                                        // +0x40
    // the implicit memberwise assignment, written out: size mode would otherwise emit it out of line
    __forceinline CCardDef& operator=(const CCardDef& other)
    {
        CParentDefClassBase::operator=(other);
        CardName = other.CardName;
        CardVal = other.CardVal;
        return *this;
    }
};
namespace std {
struct __false_type { };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}
template <class _InputIter, class _OutputIter>
inline _OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template CCardDef* __fastcall __copy_ptrs<CCardDef*, CCardDef*>(CCardDef*, CCardDef*, CCardDef*, const __false_type&);
}