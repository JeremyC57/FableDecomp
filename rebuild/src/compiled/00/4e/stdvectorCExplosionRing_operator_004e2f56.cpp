// std::vector<CExplosionRing>::operator=(const vector&) -- STLport vector assignment of 16-byte CExplosionRing records: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CExplosionRingVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004e2f56, length 241 bytes; size-optimised def TU (unaligned entry).
// Pragma: #pragma optimize("s", on) -- ebp frame, the 16-byte element copied as four movsd.
// Callees: _M_allocate_and_copy @ 0x004E3047, _M_clear @ 0x004E3088 (both out of line).
// Identity: the manifest label "CActiveFile::operator=" is propagated (wrong). Single caller CExplosionDef::Copy @ 0x004E2ECA (slot 19 of vtable 0x012425F4 = CExplosionDef) assigns the vector at +0x58 = ExplosionRings (PDB +0x58); CExplosionRing is 16 bytes (four dwords, see its TransferBinaryIn @ 0x004EB2B8).
// Shape: STLport 4.x `vector::operator=` verbatim (retail vectors are 12-byte _M_start/_M_finish/_M_end_of_storage),
//   the same body as the landed 0x00461017 (CReverbEnvironmentEntry) and 0x004E29BA (ParticleAttachmentInfo) rows:
//   `__xlen > capacity()` reallocates, `size() >= __xlen` copies with the counted random-access `__copy`, else copies
//   size() elements and placement-copies the rest (hence the null test). Helpers are __forceinline (size TU).
#pragma optimize("s", on)
#include <new>

struct CExplosionRing {             // 16 bytes: PDB RingRadius, NumExplosions, AngleOffset, SecondsBeforeSpawnNewRing (TransferBinaryIn 0x004EB2B8 reads +0/+4/+8/+c)
    float RingRadius;
    long  NumExplosions;
    float AngleOffset;
    float SecondsBeforeSpawnNewRing;
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
        new (&*__cur) CExplosionRing(*__first);
    return __cur;
}

struct CExplosionRingVector {                                   // retail std::vector<CExplosionRing> (12 bytes)
    typedef CExplosionRing* pointer;
    typedef const CExplosionRing* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x004E3047
    void _M_clear();                                                                              // 0x004E3088
    CExplosionRingVector& operator=(const CExplosionRingVector& __x);
};

CExplosionRingVector& CExplosionRingVector::operator=(const CExplosionRingVector& __x)
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