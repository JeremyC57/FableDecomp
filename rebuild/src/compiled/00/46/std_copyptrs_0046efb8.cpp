// std::__copy_ptrs(CQuickAccessItemButtonGuiDef* first, CQuickAccessItemButtonGuiDef* last, CQuickAccessItemButtonGuiDef* result, const __false_type&) -- STLport: assign [first, last) over result element by element (the non-memmove copy), return the end of the destination
// Leaf: __copy_ptrs
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046EFB8, 104 bytes, `ret 8` (__fastcall: first in ecx, last in edx; result and the
//   __false_type tag on the stack).
// Identity: the catalog's "Vector_CopyElements" is behavioural. Both callers are vector<CQuickAccessItemButtonGuiDef>::operator=
//   @ 0x0046ED15 (catalog "CActiveFile_AssignQuickAccessItems"; stride 0x50, PDB vector<CQuickAccessItemButtonGuiDef> in
//   CPlayerGuiDef), which builds the 1-byte __false_type answer with `stosb` at [ebp+0Bh] and passes its address.
// Callees: CParentDefClassBase::operator=(const CParentDefClassBase&) @ 0x00431ED0 (compiler-generated in retail, emitted
//   out of line; landed under that name).
// Model: STLport 4.x `__copy_ptrs(.., const __false_type&)` forwarding to the inlined random-access
//   `__copy(first, last, result, random_access_iterator_tag(), (ptrdiff_t*)0)`: `for (n = last - first; n > 0; --n)
//   *result++ = *first++` (the `idiv` by 0x50). The element's implicit operator= is the base assignment followed by
//   ButtonGraphic +0x3c, Position +0x40 and Offset +0x48 (layout as the landed CQuickAccessItemButtonGuiDef::Copy
//   0x0046EF87). engine/CParentDefClassBase.h is the 0x3d-byte PDB packing with the vptr flattened, so the retail
//   0x3c-byte base is local, with the out-of-line assignment declared.
// Shape: `push 50h ; pop esi` = size TU with a frame pointer (`optimize("y", off)` + `("s", on)`); the `result` value
//   parameter is re-homed in its own slot by the inlined __copy (`mov ebx,[ebp+8] ; mov [ebp+8],ebx`).
#pragma optimize("y", off)
#pragma optimize("s", on)

class C2DVector { public:
    float X;                                        // +0x00
    float Y;                                        // +0x04
};

class CParentDefClassBase {
public:
    unsigned char _body[0x3c];                      // +0x00 vptr, CDefClassBase, InstantiationName, SubDefInfoMap
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};

class CQuickAccessItemButtonGuiDef : public CParentDefClassBase {   // retail sizeof 0x50
public:
    long      ButtonGraphic;                        // +0x3c
    C2DVector Position;                             // +0x40
    C2DVector Offset;                               // +0x48
    // the compiler-generated member-wise assignment, spelled out: a size TU leaves the implicit one out of line
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
struct __false_type { };
struct random_access_iterator_tag { };

template <class RandomAccessIter, class OutputIter, class Distance>
__forceinline OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                                const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

template <class InputIter, class OutputIter>
OutputIter __fastcall __copy_ptrs(InputIter first, InputIter last, OutputIter result, const __false_type&)
{
    return __copy(first, last, result, random_access_iterator_tag(), (int*)0);
}

template CQuickAccessItemButtonGuiDef* __fastcall __copy_ptrs(CQuickAccessItemButtonGuiDef* first, CQuickAccessItemButtonGuiDef* last,
                                                              CQuickAccessItemButtonGuiDef* result, const __false_type&);
}