// std::vector<CMusicSetEntry>::operator=(const vector&) -- STLport vector assignment of 12-byte CMusicSetEntry records: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CMusicSetEntryVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460EB5, length 273 bytes; size-optimised def TU.
// Pragma: #pragma optimize("s", on) -- ebp frame, the 12-byte element copied as three movsd, `push 0Ch ; pop edi` divisors.
// Callees: _M_allocate_and_copy @ 0x00460FC6, _M_clear @ 0x00461009 (both out of line; the manifest's
//          "OnReadFinished" label on 0x00461009 is propagated across a byte-identical body).
// Identity: the manifest label "CActiveFile::CopyAssign_12" is propagated. Single caller is CSoundDef::operator=
//          @ 0x00460A61 (slot 19 of vtable 0x012349B4 = CSoundDef), which assigns the vector at retail +0x60 =
//          MusicSetEntries (landed source 00/46/CSoundDef_operator_00460a61.cpp names this callee). PDB
//          CMusicSetEntry is 12 bytes: NormalMusicID, DangerMusicID, LoopCount (three longs).
// Shape: STLport 4.x `vector::operator=` verbatim (retail vectors are 12-byte _M_start/_M_finish/_M_end_of_storage),
//   the same body as the landed 0x00460D75 (CMusicEntry, 16 bytes) row: `__xlen > capacity()` reallocates,
//   `size() >= __xlen` copies with the counted random-access `__copy`, else copies size() elements and
//   placement-copies the rest (hence the null test of the placement new). Helpers are __forceinline (size TU).
#pragma optimize("s", on)
#include <new>

struct CMusicSetEntry {             // PDB: 12 bytes
    long NormalMusicID;
    long DangerMusicID;
    long LoopCount;
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
        new (&*__cur) CMusicSetEntry(*__first);
    return __cur;
}

struct CMusicSetEntryVector {                                // retail std::vector<CMusicSetEntry> (12 bytes)
    typedef CMusicSetEntry* pointer;
    typedef const CMusicSetEntry* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x00460FC6
    void _M_clear();                                                                              // 0x00461009
    CMusicSetEntryVector& operator=(const CMusicSetEntryVector& __x);
};

CMusicSetEntryVector& CMusicSetEntryVector::operator=(const CMusicSetEntryVector& __x)
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