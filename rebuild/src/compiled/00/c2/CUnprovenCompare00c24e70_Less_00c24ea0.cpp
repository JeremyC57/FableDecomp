// CUnprovenCompare_00c24e70::Less(const CUnprovenObject_00c24e70* a, const CUnprovenObject_00c24e70* b) const -- whether a's getter value is below b's (unsigned)
// Leaf: ?Less@?$CUnprovenCompare_00c24e70@VCUnprovenObject_00c24e70@@K$1??_9@$BA@AE@@UBEHPBVCUnprovenObject_00c24e70@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c24ea0, 31 bytes, thiscall (`this` unused), `ret 8`, int result.
// Identity (descriptive): slot 1 of the non-RTTI vtables 0x012B3C44 / 0x012B3F14 (no class name anywhere); it calls the getter at virtual slot 0 of both arguments through the vcall thunk ??_9@$BA@AE 0x0043E420 and compares the referenced dwords unsigned (`sbb`); slot 0 (0x00C24E70) is the equality over the same getter (family L118), so the class is named after it. Nothing names the comparator, the object class or the getter.
// Callees: the vcall thunk ??_9@$BA@AE 0x0043E420 (`mov eax,[ecx] ; jmp [eax+0*4]`) for the constant member pointer.
// Shape (family L119 = slot 1 of family L118's class): the first value is copied before the second call and compared
//   with the second in place (`cmp esi,[eax] ; sbb eax,eax ; neg eax`); `int` result.
class CUnprovenObject_00c24e70 {
public:
    virtual const unsigned long& Get() const;             // slot 0
};
template <class T, class V, const V& (T::*Getter)() const>
class CUnprovenCompare_00c24e70 {
public:
    virtual int Equal(const T* a, const T* b) const;     // slot 0 (family L118)
    virtual int Less(const T* a, const T* b) const;      // slot 1
};
template <class T, class V, const V& (T::*Getter)() const>
int CUnprovenCompare_00c24e70<T, V, Getter>::Less(const T* a, const T* b) const
{
    V x = (a->*Getter)();
    return x < (b->*Getter)();
}
template class CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>;