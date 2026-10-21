// CRetailVector_SRecord24::InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned long count, bool atEnd) -- STLport vector<24-byte record>::_M_insert_overflow: allocate old_size + max(old_size, count) records, copy the head, insert `count` copies of value, copy the tail unless inserting at the end, free the old block
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B6D0B0, 404 bytes; `ret 14h`. Callees: malloc @ 0x00BFEA0E, the vector's
//          deallocation helper @ 0x00B6D250 (`if (first) free(first)`; landed under the propagated label
//          CActiveFile::OnReadFinished). Label correction: the catalog name "CActiveFile::ExpandAndFillVertices" is
//          propagated; the body is the growth insert the landed push_back 0x00BE4380 calls as InsertGrow (same model).
// Layout: the retail 12-byte STLport vector {first, last, end}; the element is 24 bytes (six dwords) with an IMPLICIT
//         copy constructor here (an explicit memberwise one, as in the landed push_back 0x00BE4380, folds the
//         `mov edx,ecx ; mov esi,eax` pointer copies of each placement copy and shrinks the frame).
// Shape: STLport 4.6 _M_insert_overflow(__false_type): `len = size + max(size, n)` through the reference-returning max,
//        allocate(len) is null for zero, each copy is a placement-new `_Construct` (null-tested), the single-element case
//        is separate from uninitialized_fill_n, then _M_clear and _M_set.
// Leaf: InsertGrow
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
#include <new>

struct SScalarTag { SScalarTag() { } };

struct SRecord24 {                     // 24-byte element: 6 dwords, explicit memberwise copy constructor
    long m_a, m_b, m_c, m_d, m_e, m_f;
    SRecord24() { }

};

template <class T> __forceinline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T1, class T2> __forceinline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
__forceinline ForwardIter __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const SScalarTag&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class ForwardIter, class Size, class T>
__forceinline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const SScalarTag&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}

struct CRetailVector_SRecord24 {
    SRecord24* first;
    SRecord24* last;
    SRecord24* end;
    unsigned long size() const { return (unsigned long)(last - first); }
    SRecord24* allocate(unsigned long n) { return n != 0 ? (SRecord24*)malloc(n * sizeof(SRecord24)) : 0; }
    void Deallocate();                 // 0x00B6D250 _M_clear: release the old block
    void InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned long count, bool atEnd);
};

void CRetailVector_SRecord24::InsertGrow(SRecord24* where, const SRecord24& value, const SScalarTag& tag, unsigned long count, bool atEnd)
{
    const unsigned long oldSize = size();
    const unsigned long len = oldSize + (max)(oldSize, count);
    SRecord24* newStart = allocate(len);
    SRecord24* newFinish = newStart;
    newFinish = __uninitialized_copy(first, where, newStart, tag);
    if (count == 1) {
        _Construct(newFinish, value);
        ++newFinish;
    }
    else
        newFinish = __uninitialized_fill_n(newFinish, count, value, tag);
    if (!atEnd)
        newFinish = __uninitialized_copy(where, last, newFinish, tag);
    Deallocate();
    first = newStart;
    last = newFinish;
    end = newStart + len;
}