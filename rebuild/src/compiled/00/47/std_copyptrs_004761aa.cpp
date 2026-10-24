// std::__copy_ptrs(CQuickAccessItemButtonGuiDef* first, CQuickAccessItemButtonGuiDef* last, CQuickAccessItemButtonGuiDef* result, const __false_type&) -- STLport counted forward assignment of the 0x50-byte quick-access button definitions, returns the advanced result
// Retail: Fable.exe (TLC, VC7.1) @ 0x004761AA, 104 bytes, __fastcall (first ecx, last edx; result + the tag reference
// on the stack, `ret 8`).
// Callers: 0x00476189, inside vector<CQuickAccessItemButtonGuiDef>::erase(first, last) (`__copy(last, finish, first)`;
// the vector's resize 0x00476123 and the vtable 0x012360A4 element constructor 0x00430370 sit beside it).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0 (the implicit CQuickAccessItemButtonGuiDef::operator= calls it,
// then copies ButtonGraphic +0x3c, Position +0x40 and Offset +0x48 word by word).
// Layout: as the landed CQuickAccessItemButtonGuiDef::Copy 0x0046EF87 (retail sizeof 0x50, the base 0x3c bytes).
// engine/CParentDefClassBase.h is the flattened packed PDB layout (0x3d bytes, no operator=), so the def is modelled
// locally, as the landed relatives do.
// Shape: `ret 8` -- STLport `__copy_ptrs(first, last, result, const __false_type&)` with the SGI
// `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)` inlined (`__forceinline`: this /Os TU calls a
// plain inline helper out of line); the signed count `(last - first) / 0x50` (`idiv`) runs down a frame slot and the
// result stays in its argument slot. Frame pointer kept (`optimize("y",off)` + `("s",on)`, the def TU settings).
// Label correction: catalog "Vector_ShiftCopyElements" is a guess.
// Leaf: __copy_ptrs
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
_OutputIter __fastcall __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template CQuickAccessItemButtonGuiDef* __fastcall
__copy_ptrs<CQuickAccessItemButtonGuiDef*, CQuickAccessItemButtonGuiDef*>(CQuickAccessItemButtonGuiDef*,
                                                                          CQuickAccessItemButtonGuiDef*,
                                                                          CQuickAccessItemButtonGuiDef*,
                                                                          const __false_type&);
}