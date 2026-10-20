// CEngineGraphicVector::operator=(const CEngineGraphicVector& __x) -- STLport vector<CEngineGraphic> assignment: reallocate when the source outgrows the capacity, else copy over the live elements and copy-construct the tail; returns *this
// Leaf: ??4CEngineGraphicVector
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5F70, length 280 bytes; size-optimised def TU.
// Pragma: #pragma optimize("s", on) -- ebp frame, `push 1Ch ; pop edi` constants, element copies as `movsd ; movsd ; movsd ; movsw`.
// Callees: CEngineGraphicVector::_M_allocate_and_copy @ 0x004E6088, CEngineGraphicVector::_M_clear @ 0x004E60CA (both out of line).
// Identity: the manifest label "CActiveFile::operator_assign" is propagated. The only caller is CReplaceableMeshDef::operator= @ 0x004E5F51
// (slot 19 of vtable 0x012430BC = CReplaceableMeshDef), which assigns the Meshes (CReplaceableMeshes::Vector) vector at +0x28; the PDB
// element CEngineGraphic is 0xe bytes, the divisor of every size() here.
// Shape: STLport 4.x `vector::operator=` verbatim (retail vectors are 12-byte _M_start/_M_finish/
// _M_end_of_storage): `__xlen > capacity()` reallocates through _M_allocate_and_copy + _M_clear, else
// `size() >= __xlen` copies with `__copy(random_access_iterator_tag)` (counted loop, no destroy for a
// trivial destructor), else copies size() elements and placement-copies the rest through
// `__uninitialized_copy` (hence the null test). The helpers must be __forceinline: the size-optimised
// TU otherwise emits them out of line.
#pragma optimize("s", on)
#include <new>

#pragma pack(push, 2)
struct CEngineGraphic {                        // PDB: 14 bytes, 2-byte packed (3 dwords + a word)
    unsigned long BankIndex;                   // +0x00
    float         AnimStep;                    // +0x04
    float         RenderSizeX;                 // +0x08
    bool          AdditiveAlpha;               // +0x0c
    unsigned char Type;                        // +0x0d
};
#pragma pack(pop)

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
        new (&*__cur) CEngineGraphic(*__first);
    return __cur;
}

struct CEngineGraphicVector {                     // retail std::vector<CEngineGraphic> (12 bytes)
    typedef CEngineGraphic* pointer;
    typedef const CEngineGraphic* const_pointer;
    pointer _M_start;
    pointer _M_finish;
    pointer _M_end_of_storage;

    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    unsigned int capacity() const { return (unsigned int)(_M_end_of_storage - _M_start); }
    pointer _M_allocate_and_copy(unsigned int __n, const_pointer __first, const_pointer __last);   // 0x004E6088
    void _M_clear();                                                                              // 0x004E60CA
    CEngineGraphicVector& operator=(const CEngineGraphicVector& __x);
};

CEngineGraphicVector& CEngineGraphicVector::operator=(const CEngineGraphicVector& __x)
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