// std::vector<CMusicEntry>::operator=(const vector&) -- STLport vector assignment of 16-byte CMusicEntry records: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CMusicEntryVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00460d75, length 241 bytes; size-optimised def TU (unaligned entry).
// Pragma: #pragma optimize("s", on) -- ebp frame, the 16-byte element copied as four movsd.
// Callees: _M_allocate_and_copy @ 0x00460E66, _M_clear @ 0x00460EA7 (both out of line).
// Identity: the manifest label "CActiveFile::CopyAssign_16" is propagated (wrong). Single caller CSoundDef::operator= @ 0x00460A61 (slot 19 of vtable 0x012349B4 = CSoundDef) assigns the vector at retail +0x54 = MusicEntries (PDB +0x60; retail 12-byte vectors from SoundBankEntries +0x3c); PDB CMusicEntry is 16 bytes (ID, Filename, Gain, CacheToHD).
// Shape: STLport 4.x `vector::operator=` verbatim (retail vectors are 12-byte _M_start/_M_finish/_M_end_of_storage),
//   the same body as the landed 0x00461017 (CReverbEnvironmentEntry) and 0x004E29BA (ParticleAttachmentInfo) rows:
//   `__xlen > capacity()` reallocates, `size() >= __xlen` copies with the counted random-access `__copy`, else copies
//   size() elements and placement-copies the rest (hence the null test). Helpers are __forceinline (size TU).
#pragma optimize("s", on)
#include <new>

struct CMusicEntry {                // PDB: 16 bytes
    unsigned long ID;
    long          Filename;             // CDefString (a 4-byte string-table index)
    float         Gain;
    bool          CacheToHD;
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
        new (&*__cur) CMusicEntry(*__first);
    return __cur;
}

struct CMusicEntryVector {                                   // retail std::vector<CMusicEntry> (12 bytes)
    typedef CMusicEntry* pointer;
    typedef const CMusicEntry* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x00460E66
    void _M_clear();                                                                              // 0x00460EA7
    CMusicEntryVector& operator=(const CMusicEntryVector& __x);
};

CMusicEntryVector& CMusicEntryVector::operator=(const CMusicEntryVector& __x)
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