// _global::Copy_SVectorRecord1C_006f4470(SVectorRecord1C*, SVectorRecord1C*, SVectorRecord1C*, SRandomAccessIteratorTag, long*) -- SGI __copy over 28-byte {16 bytes of data, vector} records, returns the advanced result
// Leaf: Copy_SVectorRecord1C_006f4470
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F4470, length 97 bytes.
// Callees: vector<28-byte record>::operator= @ 0x006F3C20.
// SGI-STL `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the two tag arguments on the stack, `ret 0Ch`). The signed count (last - first) / 28
// (0x92492493 multiply + add + `sar 4`) is computed once and counted down in ebx. The element's implicit operator=
// moves the four leading dwords, then calls the out-of-line STLport vector operator= of the member at +0x10
// (0x006F3C20 itself divides by 28, so the vector holds records of this same size -- a nested tree of records).
// The advanced result is returned (the untouched result for an empty range). The manifest label
// (`_Move<pair<unsigned __int64, CPersistInfo>*>`) is propagated across bodies of many element sizes.
template <class T>
struct CRetailVector {                          // STLport vector<T>: 12 bytes
    T* _M_start;                                // +0
    T* _M_finish;                               // +4
    T* _M_end_of_storage;                       // +8
    CRetailVector& operator=(const CRetailVector& other);
};

struct SVectorRecord1C {                        // 28 bytes
    unsigned long                  m_word0;     // +0x00
    unsigned long                  m_word4;     // +0x04
    unsigned long                  m_word8;     // +0x08
    unsigned long                  m_wordC;     // +0x0C
    CRetailVector<SVectorRecord1C> m_children;  // +0x10 (operator= @ 0x006F3C20)
};

struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SVectorRecord1C* __fastcall Copy_SVectorRecord1C_006f4470(SVectorRecord1C* first, SVectorRecord1C* last, SVectorRecord1C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}