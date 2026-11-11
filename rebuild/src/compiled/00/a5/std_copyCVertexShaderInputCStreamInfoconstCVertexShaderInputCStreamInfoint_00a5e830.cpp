// _global::Copy_PairLongVectorHolder_00a5e830(SPairLongVectorHolder*, SPairLongVectorHolder*, SPairLongVectorHolder*, const random_access_iterator_tag&, int*) -- SGI __copy over 0x14-byte (key, polymorphic vector holder) pairs
// Leaf: Copy_PairLongVectorHolder_00a5e830
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A5E830, length 75 bytes.
// SGI-STL random-access `__copy(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first
// in ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T) by reciprocal multiply, the loop counts it down and the advanced result is returned.
// Element: 0x14 bytes = a long key + a polymorphic 0x10-byte holder (vtable +4, never copied by the implicit
// operator=) whose only member is a 12-byte vector (+8) assigned through the out-of-line vector operator= @ 0x00A5E4D0
// (first/last/end-of-storage, catalogued as StdVector_Assign_PairFloatEnum_v2). The manifest's
// `CTCSkeletalMorph::CSkeletalMorph` label is propagated (12 bytes); the record stays descriptive.
struct SVector {                             // 12-byte STL vector (first, last, end of storage)
    void* m_pFirst;                          // +0x00
    void* m_pLast;                           // +0x04
    void* m_pEnd;                            // +0x08
    SVector& operator=(const SVector& other);              // 0x00A5E4D0
};

class CVectorHolder {                        // 0x10 bytes
public:
    virtual void v00();                      // +0x00 vtable
    SVector Items;                           // +0x04
};

struct SPairLongVectorHolder {               // 0x14 bytes
    long          first;                     // +0x00
    CVectorHolder second;                    // +0x04
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SPairLongVectorHolder* __fastcall Copy_PairLongVectorHolder_00a5e830(SPairLongVectorHolder* first, SPairLongVectorHolder* last, SPairLongVectorHolder* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}