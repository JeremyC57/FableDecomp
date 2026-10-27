// CUnprovenCompare_00c09d20::Equal(const CUnprovenObject_00c09d20* a, const CUnprovenObject_00c09d20* b) const -- whether the two objects' getter values are equal
// Leaf: ?Equal@?$CUnprovenCompare_00c09d20@VCUnprovenObject_00c09d20@@K$1??_9@$B3AE@@UBEHPBVCUnprovenObject_00c09d20@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c09d20, 36 bytes, thiscall (`this` unused), `ret 8`, int result.
// Identity (descriptive): slot 0 of the non-RTTI vtables 0x012B1B78 / 0x012B1D98 (no class name anywhere); it calls the getter at virtual slot 1 of both arguments through the vcall thunk ??_9@$B3AE 0x0043E430 and compares the referenced dwords; its sibling slot 1 (0x00C09D50) is the unsigned less-than over the same getter. Nothing names the comparator, the object class or the getter.
// Callees: the vcall thunk ??_9@$B3AE 0x0043E430 (`mov eax,[ecx] ; jmp [eax+1*4]`) for the constant member pointer.
// Shape (family L118): a class template over a pointer to a virtual getter (a constant template argument, so the call
//   goes through the shared vcall thunk); the first value is copied before the second call; `int` result (`xor ecx,ecx
//   ; sete cl ; mov eax,ecx`; a bool result ends with `mov al,cl`).
class CUnprovenObject_00c09d20 {
public:
    virtual void Slot0();
    virtual const unsigned long& Get() const;             // slot 1
};
template <class T, class V, const V& (T::*Getter)() const>
class CUnprovenCompare_00c09d20 {
public:
    virtual int Equal(const T* a, const T* b) const;
};
template <class T, class V, const V& (T::*Getter)() const>
int CUnprovenCompare_00c09d20<T, V, Getter>::Equal(const T* a, const T* b) const
{
    V x = (a->*Getter)();
    V y = (b->*Getter)();
    return x == y;
}
template class CUnprovenCompare_00c09d20<CUnprovenObject_00c09d20, unsigned long, &CUnprovenObject_00c09d20::Get>;