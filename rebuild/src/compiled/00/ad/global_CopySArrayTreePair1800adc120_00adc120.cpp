// _global::Copy_SArrayTreePair18_00adc120(const SArrayTreePair18_00adc120* first, const SArrayTreePair18_00adc120* last, SArrayTreePair18_00adc120* result, const random_access_iterator_tag&, int*) -- STLport __copy<SArrayTreePair18_00adc120*>: counted forward assignment of 24-byte elements, returns the advanced result
// Leaf: Copy_SArrayTreePair18_00adc120
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00ADC120, length 79 bytes, `ret 0Ch`.
// Callees: SArray0C_0045bc09::operator= @ 0x0045BC09 (m_values); CRetailTree_0045c811::operator= @ 0x0045C79E (m_tree).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (imul 0x2aaaaaab ; sar 2 (/24)), then the element's implicit operator= inlined memberwise (each member with its own assignment is a `push src ; mov ecx,dst ; call`, a plain word a direct move),
// both pointers stepped by 0x18; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; same element shape as 0x00ADC0D0 (a second instantiation in the same TU: identical body, separate COMDAT).
struct SArray0C_0045bc09 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_0045bc09& operator=(const SArray0C_0045bc09& other);   // 0x0045BC09
};
struct CRetailTree_0045c811 {                    // 12 bytes, assigned out of line
    void* m_words[3];
    CRetailTree_0045c811& operator=(const CRetailTree_0045c811& other);   // 0x0045C79E
};
struct SArrayTreePair18_00adc120 {               // 24 bytes; implicit memberwise operator=
    SArray0C_0045bc09 m_values;        // +0x00
    CRetailTree_0045c811 m_tree;        // +0x0c
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SArrayTreePair18_00adc120* __fastcall Copy_SArrayTreePair18_00adc120(const SArrayTreePair18_00adc120* first, const SArrayTreePair18_00adc120* last, SArrayTreePair18_00adc120* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}