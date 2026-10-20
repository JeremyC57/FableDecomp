// CDegradableInfoVector::operator=(const CDegradableInfoVector& __x) -- STLport vector<CDegradableInfo> assignment: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CDegradableInfoVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00454467, length 287 bytes; size-optimised def TU.
// Pragma: #pragma optimize("s", on) -- ebp frame, `push 1Ch ; pop edi` constants, element copies as `push 7 ; pop ecx ; rep movsd`.
// Callees: CDegradableInfoVector::_M_allocate_and_copy @ 0x00454586, CDegradableInfoVector::_M_clear @ 0x004545CA (both out of line).
// Identity: the manifest label "CActiveFile::operator=" is propagated. The only caller is CDegradableDef::operator= @ 0x00454434
// (slot 19 of vtable 0x01232D1C = CDegradableDef), which assigns the Degradations vector at +0x30; the PDB
// element CDegradableInfo is 0x1c bytes, the divisor of every size() here.
// Shape: STLport 4.x `vector::operator=` verbatim (retail vectors are 12-byte _M_start/_M_finish/
// _M_end_of_storage): `__xlen > capacity()` reallocates through _M_allocate_and_copy + _M_clear, else
// `size() >= __xlen` copies with `__copy(random_access_iterator_tag)` (counted loop, no destroy for a
// trivial destructor), else copies size() elements and placement-copies the rest through
// `__uninitialized_copy` (hence the null test). The helpers must be __forceinline: the size-optimised
// TU otherwise emits them out of line.
#pragma optimize("s", on)
#include <new>

#pragma pack(push, 2)
struct CEngineGraphic {                        // PDB: 14 bytes, 2-byte packed
    unsigned long BankIndex;                   // +0x00
    float         AnimStep;                    // +0x04
    float         RenderSizeX;                 // +0x08
    bool          AdditiveAlpha;               // +0x0c
    unsigned char Type;                        // +0x0d
};
#pragma pack(pop)

struct CDegradableInfo {                       // PDB: 28 bytes, a POD block-copied as 7 dwords
    float          HealthPercentage;           // +0x00
    CEngineGraphic EngineGraphic;              // +0x04
    long           SmashParticleEmitter;       // +0x14
    bool           BlocksNavigation;           // +0x18
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
        new (&*__cur) CDegradableInfo(*__first);
    return __cur;
}

struct CDegradableInfoVector {                     // retail std::vector<CDegradableInfo> (12 bytes)
    typedef CDegradableInfo* pointer;
    typedef const CDegradableInfo* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x00454586
    void _M_clear();                                                                              // 0x004545CA
    CDegradableInfoVector& operator=(const CDegradableInfoVector& __x);
};

CDegradableInfoVector& CDegradableInfoVector::operator=(const CDegradableInfoVector& __x)
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