// CUnprovenCompare_00c24e70::EqualToValue(const unsigned long& value, const CUnprovenObject_00c24e70* b) const -- whether b's getter value equals value
// Leaf: ?EqualToValue@?$CUnprovenCompare_00c24e70@VCUnprovenObject_00c24e70@@K$1??_9@$BA@AE@@UBEHABKPBVCUnprovenObject_00c24e70@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c24ec0, 27 bytes, thiscall (`this` unused), `ret 8`, int result.
// Identity (descriptive): slot 2 of the non-RTTI vtables 0x012B3C48 / 0x012B3F18 (no class name anywhere); it calls the getter at virtual slot 0 of the second argument through the vcall thunk ??_9@$BA@AE 0x0043E420 and compares the referenced dword with the dword the first argument points at; slot 0 (0x00C24E70) is the equality over the same getter (family L118), so the class is named after it. Nothing names the comparator, the object class or the getter.
// Callees: the vcall thunk ??_9@$BA@AE 0x0043E420 (`mov eax,[ecx] ; jmp [eax+0*4]`) for the constant member pointer.
// Shape (family L120 = slot 2 of family L118's class): one getter call, then `sub / neg / sbb / inc` for the int equality.
class CUnprovenObject_00c24e70 {
public:
    virtual const unsigned long& Get() const;             // slot 0
};
template <class T, class V, const V& (T::*Getter)() const>
class CUnprovenCompare_00c24e70 {
public:
    virtual int Equal(const T* a, const T* b) const;     // slot 0 (family L118)
    virtual int Less(const T* a, const T* b) const;      // slot 1 (family L119)
    virtual int EqualToValue(const V& value, const T* b) const;   // slot 2
};
template <class T, class V, const V& (T::*Getter)() const>
int CUnprovenCompare_00c24e70<T, V, Getter>::EqualToValue(const V& value, const T* b) const
{
    return value == (b->*Getter)();
}
template class CUnprovenCompare_00c24e70<CUnprovenObject_00c24e70, unsigned long, &CUnprovenObject_00c24e70::Get>;