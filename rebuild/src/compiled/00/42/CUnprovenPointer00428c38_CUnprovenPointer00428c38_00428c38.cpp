// CUnprovenPointer_00428c38::CUnprovenPointer_00428c38(const CUnprovenPointer_00428c38& other) -- copy constructor of a one-pointer wrapper: copy the pointer, return this
// Leaf: ??0CUnprovenPointer_00428c38@@QAE@ABV0@@Z
// Retail: Fable.exe @ 0x00428C38, 13 bytes, __fastcall `ret 4`: `mov eax,ecx ; mov ecx,[other] ; mov ecx,[ecx] ;
// mov [eax],ecx`.
// Identity: descriptive. A user-declared copy constructor (an implicit one is never emitted as a function) of a 4-byte
// class holding one pointer, emitted as an unreferenced inline COMDAT in a packed (no alignment padding) run of
// one-word helpers: CIVCountedPointer releases 0x00428C02 / 0x00428C1A, `return &node->value` accessors at
// 0x00428C32 / 0x00428C45 (+0x10, an _Rb_tree node) and 0x00428C56 (+8, a list node), and the from-pointer constructor
// 0x00428C4B. Those neighbours suggest an STLport iterator copy constructor, but no caller or symbol proves which, so
// the class is a per-address placeholder. A `T& operator=(const T&)` compiles to the same bytes.
// Shape: size-optimised TU (the packed run proves /Os): /Os reuses ecx for the loaded word, the speed build takes edx.
#pragma optimize("s", on)
class CUnprovenPointer_00428c38 {
public:
    void* Pointer;                                          // +0x00
    CUnprovenPointer_00428c38(const CUnprovenPointer_00428c38& other);
};

CUnprovenPointer_00428c38::CUnprovenPointer_00428c38(const CUnprovenPointer_00428c38& other)
    : Pointer(other.Pointer)
{
}