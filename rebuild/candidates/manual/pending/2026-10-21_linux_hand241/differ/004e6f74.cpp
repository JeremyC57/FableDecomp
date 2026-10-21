// CTextureMorphEntryVector::operator=(const CTextureMorphEntryVector&) -- STLport vector<CTextureMorphEntry> assignment: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Grade: DIFFER(ours v retail) -- 290v333: retail spills x._M_start/_M_finish to frame slots and keeps element size in esi (POD vector operator=); 3 forms tried
// Leaf: ??4CTextureMorphEntryVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E6F74, length 331 bytes (`ret 4`); size-optimised def TU (`push N ; pop reg` constants, ebp frame).
// Pragma: #pragma optimize("s", on)
// Callees: CTextureMorphEntryVector::_M_allocate_and_copy @ 0x004E70CF and _M_clear @ 0x004E70C1 (both out of line; manifest labels
//          "CActiveFile::operator_assign" / "CActiveFile::OnReadFinished" are propagated across these byte-twin bodies).
// Shape: the landed CEngineGraphicVector::operator= (0x004E5F70) model: STLport 4.x `vector::operator=` verbatim over the
//          12-byte retail vector; `__copy` is the counted random-access loop (whole-element copies), `__uninitialized_copy`
//          placement-copies the tail through the implicit member-wise copy constructor (first word, then the rest), hence
//          the null test on the destination. The element is 24 bytes (`push 18h ; pop esi`, `lea eax,[edi+edi*2] ; shl 3`), copied with `rep movsd` (6).
#pragma optimize("s", on)
#include <new>

struct CTextureMorphEntry {                  // 24 bytes: a word then a 20-byte block (the placement copy writes them apart)
    long          Index;                        // +0x00
    unsigned char Data[0x14];                   // +0x04 (descriptive: four words + the morph value)
};

struct random_access_iterator_tag {};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last,
                                 _OutputIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        new (&*__cur) CTextureMorphEntry(*__first);
    return __cur;
}

struct CTextureMorphEntryVector {                     // retail std::vector<CTextureMorphEntry> (12 bytes)
    typedef CTextureMorphEntry* pointer;
    typedef const CTextureMorphEntry* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x004E70CF
    void _M_clear();                                                                              // 0x004E70C1
    CTextureMorphEntryVector& operator=(const CTextureMorphEntryVector& __x);
};

CTextureMorphEntryVector& CTextureMorphEntryVector::operator=(const CTextureMorphEntryVector& __x)
{
    if (&__x != this) {
        const unsigned int __xlen = __x.size();
        if (__xlen > capacity()) {
            pointer __tmp = _M_allocate_and_copy(__xlen, (const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0);
            _M_clear();
            _M_start = __tmp;
            _M_end_of_storage = _M_start + __xlen;
        }
        else if (size() >= __xlen) {
            __copy((const_pointer)__x._M_start + 0, (const_pointer)__x._M_finish + 0, _M_start, random_access_iterator_tag(), (int*)0);
        }
        else {
            __copy((const_pointer)__x._M_start, (const_pointer)__x._M_start + size(), _M_start, random_access_iterator_tag(), (int*)0);
            __uninitialized_copy((const_pointer)__x._M_start + size(), (const_pointer)__x._M_finish + 0, _M_finish);
        }
        _M_finish = _M_start + __xlen;
    }
    return *this;
}
