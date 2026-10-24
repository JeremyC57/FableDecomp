// std::__copy_backward_ptrs(CQuickAccessItemButtonGuiDef* first, CQuickAccessItemButtonGuiDef* last, CQuickAccessItemButtonGuiDef* result, const __false_type&) -- STLport counted backward assignment of the 0x50-byte quick-access button definitions, returns the moved-back result
// Retail: Fable.exe (TLC, VC7.1) @ 0x00476497, 110 bytes, __fastcall (first ecx, last edx; result + the tag reference
// on the stack, `ret 8`).
// Callers: 0x004762AF, inside the vector<CQuickAccessItemButtonGuiDef> fill-insert (it zero-fills the tag byte and
// pushes it with the result; the vector's resize 0x00476123 and erase-copy 0x004761AA sit beside it).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0 (the implicit CQuickAccessItemButtonGuiDef::operator= calls it,
// then copies ButtonGraphic +0x3c, Position +0x40 and Offset +0x48 word by word).
// Layout: as the landed CQuickAccessItemButtonGuiDef::Copy 0x0046EF87 (retail sizeof 0x50, the base 0x3c bytes).
// engine/CParentDefClassBase.h is the flattened packed PDB layout (0x3d bytes, no operator=), so the def is modelled
// locally, as the landed relatives do.
// Shape: `ret 8` -- STLport `__copy_backward_ptrs(first, last, result, const __false_type&)` with the SGI
// `__copy_backward(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined (`__forceinline`: this /Os TU calls a
// plain inline helper out of line); the signed count `(last - first) / 0x50` (`idiv`) runs down a frame slot and the
// result stays in its argument slot. Frame pointer kept (`optimize("y",off)` + `("s",on)`, the def TU settings).
// Label correction: catalog "Vector_ShiftQuickAccessButtonsReverse" is a guess (the element is right).
// Leaf: __copy_backward_ptrs
#pragma optimize("y", off)
#pragma optimize("s", on)

struct C2DVector {
    float X;
    float Y;
};

struct CDefClassBase {
    virtual void v00();
};

struct CParentDefClassBase : CDefClassBase {
    unsigned char _body[0x3c - 4];                          // +0x04
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

struct CQuickAccessItemButtonGuiDef : CParentDefClassBase {   // vtable 0x012360A4, retail sizeof 0x50
    long      ButtonGraphic;                                // +0x3c
    C2DVector Position;                                     // +0x40
    C2DVector Offset;                                       // +0x48
    // The memberwise assignment, spelled out: the /Os TU calls the compiler-generated one out of line.
    __forceinline CQuickAccessItemButtonGuiDef& operator=(const CQuickAccessItemButtonGuiDef& other)
    {
        CParentDefClassBase::operator=(other);
        ButtonGraphic = other.ButtonGraphic;
        Position = other.Position;
        Offset = other.Offset;
        return *this;
    }
};

namespace std {
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct __false_type { __false_type() { } };

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
__forceinline _BidirectionalIter2 __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                  _BidirectionalIter2 __result, const random_access_iterator_tag&,
                                                  _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CQuickAccessItemButtonGuiDef* __fastcall
__copy_backward_ptrs<CQuickAccessItemButtonGuiDef*, CQuickAccessItemButtonGuiDef*>(CQuickAccessItemButtonGuiDef*,
                                                                                   CQuickAccessItemButtonGuiDef*,
                                                                                   CQuickAccessItemButtonGuiDef*,
                                                                                   const __false_type&);
}