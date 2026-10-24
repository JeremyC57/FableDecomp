// std::__copy_backward_ptrs<CCardDef*,CCardDef*>(CCardDef* first, CCardDef* last, CCardDef* result, const __false_type&) -- STLport backward assignment copy of card defs: assign [first, last) onto the range ending at result, last element first, return the new start
// Leaf: ??$__copy_backward_ptrs@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FA3E3, 81 bytes, `ret 8` (__fastcall: first in ecx, last in edx, result and the
//   tag reference on the stack -- a /Gr algorithm TU).
// Caller: 0x004FA318, vector<CCardDef>::_M_fill_insert (shift the tail up before filling the gap). Catalog label
//   "ShoveHaPennyRegionDef_Copy" is propagated.
// Element: CCardDef (Ego_r.pdb; retail 0x44: CParentDefClassBase 0x39 bytes, long CardName +0x3C, long CardVal +0x40);
//   its assignment runs CParentDefClassBase::operator= @ 0x00431ED0 then the two dword copies, written as a
//   `__forceinline` memberwise operator= (the size TU calls the implicit one out of line). The engine header
//   CParentDefClassBase.h carries the Ego_r donor size 0x3D, so a local model stands in.
// Shape: STLport `__copy_backward_ptrs(first, last, result, __false_type) { return __copy_backward(first, last, result,
//   random_access_iterator_tag(), (ptrdiff_t*)0); }` with the counted `__copy_backward` inlined (`*--result = *--last`);
//   the count is an `idiv` by 0x44. Size TU with a frame pointer (`optimize("y", off)` + `("s", on)`: result lives in
//   its argument slot [ebp+8]), like this TU's _M_insert_overflow 0x004FA434.
#pragma optimize("y", off)
#pragma optimize("s", on)

namespace std {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
__forceinline BidirectionalIter2 __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last,
                                                 BidirectionalIter2 result, const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

template <class BidirectionalIter1, class BidirectionalIter2>
BidirectionalIter2 __fastcall __copy_backward_ptrs(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                                   const __false_type&)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

}

class CParentDefClassBase {                         // local model, retail 0x39 bytes (the engine header carries the donor 0x3D)
public:
    unsigned char _body[0x39];
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

class CCardDef : public CParentDefClassBase {      // Ego_r.pdb, retail sizeof 0x44
public:
    long CardName;                                  // +0x3C
    long CardVal;                                   // +0x40
    __forceinline CCardDef& operator=(const CCardDef& other)
    {
        CParentDefClassBase::operator=(other);
        CardName = other.CardName;
        CardVal = other.CardVal;
        return *this;
    }
};

template CCardDef* __fastcall std::__copy_backward_ptrs<CCardDef*, CCardDef*>(CCardDef* first, CCardDef* last,
    CCardDef* result, const std::__false_type&);