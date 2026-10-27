// std::__copy_ptrs(CPrizeScoreDef* first, CPrizeScoreDef* last, CPrizeScoreDef* result, const __false_type&) -- STLport assignment copy of def records: n = last - first, then `*result = *first` n times; returns the end of the copy
// Flags: /Gr
// Leaf: ??$__copy_ptrs@PAVCPrizeScoreDef@@PAV1@@std@@YI
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDAF3, 75 bytes, __fastcall (first ecx, last edx, result and the empty tag on the stack), `ret 8`,
//   frame pointer.
// Identity (proven): reloc_bindings ??$__copy_ptrs@PAVCPrizeScoreDef@@PAV1@@std@@YIPAVCPrizeScoreDef@@PAV1@00ABU__false_type@0@@Z; caller the landed vector::erase 0x004EDAB7.
// Callees: CDefClassBase::operator= 0x00431F10 per element (the implicit CPrizeScoreDef::operator= inline: the base part, then the two own dwords).
// Shape (family K124, representative 0x004EABE5): STLport 4.6 `__copy_ptrs(..., const __false_type&)` -> `__copy(first, last, result,
//   random_access_iterator_tag(), (int*)0)` (__forceinline); frame/size TU (`push 30h ; pop esi ; idiv esi`, the count in the
//   result slot).
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
class CDefClassBase {                                    // retail 0x25 bytes, byte-packed (vptr + CDefPointeeBase/CResource parts)
public:
    virtual void v00();
    unsigned char _body[0x21];                           // +0x04 (not touched here)
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
class CSubDefClassBase : public CDefClassBase {         // retail 0x28 with the derived alignment
public:
    __forceinline CSubDefClassBase& operator=(const CSubDefClassBase& other) { CDefClassBase::operator=(other); return *this; }
};
class CPrizeScoreDef : public CSubDefClassBase {         // retail 0x30 bytes (Ego_r.pdb, RTTI base CSubDefClassBase)
public:
    float Score;                                         // +0x28
    long Mult;                                           // +0x2C
    // the implicit memberwise assignment, written out: size mode would otherwise emit it out of line
    __forceinline CPrizeScoreDef& operator=(const CPrizeScoreDef& other)
    {
        CSubDefClassBase::operator=(other);
        Score = other.Score;
        Mult = other.Mult;
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
template CPrizeScoreDef* __fastcall __copy_ptrs<CPrizeScoreDef*, CPrizeScoreDef*>(CPrizeScoreDef*, CPrizeScoreDef*, CPrizeScoreDef*, const __false_type&);
}