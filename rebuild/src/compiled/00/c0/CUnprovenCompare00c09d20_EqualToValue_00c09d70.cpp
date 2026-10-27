// CUnprovenCompare_00c09d20::EqualToValue(const unsigned long& value, const CUnprovenObject_00c09d20* b) const -- whether b's getter value equals value
// Leaf: ?EqualToValue@?$CUnprovenCompare_00c09d20@VCUnprovenObject_00c09d20@@K$1??_9@$B3AE@@UBEHABKPBVCUnprovenObject_00c09d20@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00c09d70, 27 bytes, thiscall (`this` unused), `ret 8`, int result.
// Identity (descriptive): slot 2 of the non-RTTI vtables 0x012B1B80 / 0x012B1DA0 (no class name anywhere); it calls the getter at virtual slot 1 of the second argument through the vcall thunk ??_9@$B3AE 0x0043E430 and compares the referenced dword with the dword the first argument points at; slot 0 (0x00C09D20) is the equality over the same getter (family L118), so the class is named after it. Nothing names the comparator, the object class or the getter.
// Callees: the vcall thunk ??_9@$B3AE 0x0043E430 (`mov eax,[ecx] ; jmp [eax+1*4]`) for the constant member pointer.
// Shape (family L120 = slot 2 of family L118's class): one getter call, then `sub / neg / sbb / inc` for the int equality.
class CUnprovenObject_00c09d20 {
public:
    virtual void Slot0();
    virtual const unsigned long& Get() const;             // slot 1
};
template <class T, class V, const V& (T::*Getter)() const>
class CUnprovenCompare_00c09d20 {
public:
    virtual int Equal(const T* a, const T* b) const;     // slot 0 (family L118)
    virtual int Less(const T* a, const T* b) const;      // slot 1 (family L119)
    virtual int EqualToValue(const V& value, const T* b) const;   // slot 2
};
template <class T, class V, const V& (T::*Getter)() const>
int CUnprovenCompare_00c09d20<T, V, Getter>::EqualToValue(const V& value, const T* b) const
{
    return value == (b->*Getter)();
}
template class CUnprovenCompare_00c09d20<CUnprovenObject_00c09d20, unsigned long, &CUnprovenObject_00c09d20::Get>;