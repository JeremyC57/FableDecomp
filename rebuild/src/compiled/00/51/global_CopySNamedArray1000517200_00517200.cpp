// _global::Copy_SNamedArray10_00517200(const SNamedArray10* first, const SNamedArray10* last, SNamedArray10* result, const random_access_iterator_tag&, int*) -- STLport __copy<SNamedArray10*>: counted forward assignment of 16-byte elements, returns the advanced result
// Leaf: Copy_SNamedArray10_00517200
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00517200, length 65 bytes, `ret 0Ch`.
// Callees: CCharString::operator= @ 0x0099EFB0 (m_name); SArray0C_00454886::operator= @ 0x00454886 (m_values).
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (sar 4 (/16)), then the element's implicit operator= inlined memberwise (each member with its own assignment is a `push src ; mov ecx,dst ; call`, a plain word a direct move),
// both pointers stepped by 0x10; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated; the element is a {CCharString, 12-byte vector} pair (0x00454886 is a vector<4-byte>::operator= body; catalog "CActiveFile::operator=").
class CCharString { public:                         // 4 bytes: the string-data pointer
    void* PStringData;
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct SArray0C_00454886 {                       // 12 bytes, assigned out of line
    void* m_words[3];
    SArray0C_00454886& operator=(const SArray0C_00454886& other);   // 0x00454886
};
struct SNamedArray10 {                           // 16 bytes; implicit memberwise operator=
    CCharString m_name;                    // +0x00
    SArray0C_00454886 m_values;        // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SNamedArray10* __fastcall Copy_SNamedArray10_00517200(const SNamedArray10* first, const SNamedArray10* last, SNamedArray10* result, const random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}