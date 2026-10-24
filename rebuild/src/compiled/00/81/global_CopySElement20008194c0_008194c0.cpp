// _global::Copy_SElement20_008194c0(SElement20*, SElement20*, SElement20*, SRandomAccessIteratorTag, long*) -- SGI __copy over 20-byte POD elements, returns the advanced result
// Leaf: Copy_SElement20_008194c0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008194C0, length 86 bytes. No callees.
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed element count
// (last - first) / 20 is computed once with the reciprocal multiply and counted down in edx; each iteration
// assigns one 20-byte trivially copyable element (an inline dword block through ebx) and advances both
// pointers; the advanced result is returned (the untouched result for an empty range). The manifest label
// (`_Move<pair<unsigned __int64, CPersistInfo>*>`) is propagated across byte-identical bodies of different
// element sizes, so the element is modelled as an opaque 20-byte record (same shape as the landed 0x00AE50E0).
struct SElement20 {
    unsigned char _body[0x14];
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SElement20* __fastcall Copy_SElement20_008194c0(SElement20* first, SElement20* last, SElement20* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}