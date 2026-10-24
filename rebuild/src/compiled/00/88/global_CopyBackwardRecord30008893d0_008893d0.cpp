// _global::CopyBackward_Record30_008893d0(SRecord30*, SRecord30*, SRecord30*, const random_access_iterator_tag&, int*) -- SGI __copy_backward over 0x30-byte records holding a vector and a double
// Leaf: CopyBackward_Record30_008893d0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008893D0, length 112 bytes.
// SGI-STL `__copy_backward(first, last, result, random_access_iterator_tag, Distance*)`, /Gr TU (__fastcall: first in
// ecx, last in edx, result + the tag reference + the distance pointer on the stack, `ret 0Ch`); the count is
// (last - first) / sizeof(T), both pointers step back one element and the moved-back result is returned.
// Element: 0x30 bytes (8-aligned by the double) = long +0, byte +4, a 12-byte vector +8 assigned through its
// out-of-line operator= @ 0x00888EE0 (catalogued as Vector_Reallocate), longs +0x14/+0x18, byte +0x1C, double +0x20
// (`fld qword ; fstp qword`), long +0x28. The manifest's `pair<CCharString,C3DHelperPoint>` label is propagated (no
// CCharString is assigned here); the record stays descriptive.
struct SVector {                             // 12-byte STL vector (first, last, end of storage)
    void* m_pFirst;                          // +0x00
    void* m_pLast;                           // +0x04
    void* m_pEnd;                            // +0x08
    SVector& operator=(const SVector& other);              // 0x00888EE0
};

struct SRecord30 {                           // 0x30 bytes
    long    Value0;                          // +0x00
    bool    Flag0;                           // +0x04
    SVector Items;                           // +0x08
    long    Value1;                          // +0x14
    long    Value2;                          // +0x18
    bool    Flag1;                           // +0x1C
    double  Time;                            // +0x20
    long    Value3;                          // +0x28
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SRecord30* __fastcall CopyBackward_Record30_008893d0(SRecord30* first, SRecord30* last, SRecord30* result, const random_access_iterator_tag& tag, int* distance)
{
    for (int n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}