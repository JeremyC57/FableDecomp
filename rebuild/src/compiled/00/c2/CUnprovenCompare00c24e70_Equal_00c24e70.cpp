// CUnprovenCompare_00c24e70::Equal(const CUnprovenObject_00c24e70* a, const CUnprovenObject_00c24e70* b) const -- whether the two objects' getter values are equal
// Leaf: ?Equal@?$CUnprovenCompare_00c24e70@VCUnprovenObject_00c24e70@@K$1??_9@$BA@AE@@UBEHPBVCUnprovenObject_00c24e70@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c24e70, 36 bytes, thiscall (`this` unused), `ret 8`, int result.
// Identity (descriptive): slot 0 of the non-RTTI vtables 0x012B3C40 / 0x012B3F10 (no class name anywhere); it calls the getter at virtual slot 0 of both arguments through the vcall thunk ??_9@$BA@AE 0x0043E420 and compares the referenced dwords; its sibling slot 1 (0x00C24EA0) is the unsigned less-than over the same getter. Nothing names the comparator, the object class or the getter.
// Callees: the vcall thunk ??_9@$BA@AE 0x0043E420 (`mov eax,[ecx] ; jmp [eax+0*4]`) for the constant member pointer.
// Shape (family L118): a class template over a pointer to a virtual getter (a constant template argument, so the call
//   goes through the shared vcall thunk); the first value is copied before the second call; `int` result (`xor ecx,ecx
//   ; sete cl ; mov eax,ecx`; a bool result ends with `mov al,cl`).
class CUnprovenObject_00c24e70 {
public:
    virtual const unsigned long& Get() const;             // slot 0
};
template <class T, class V, const V& (T::*Getter)() const>
class CUnprovenCompare_00c24e70 {
public:
    virtual int Equal(const T* a, const T* b) const;
};
template <class T, class V, const V& (T::*Getter)() const>
int CUnprovenCompare_00c24e70<T, V, Getter>::Equal(const T* a, const T* b) const
{
    V x = (a->*Getter)();
    V y = (b->*Getter)();
    return x == y;
}
template class CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>;