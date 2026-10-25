// std::__uninitialized_fill_n(SCountedPointerLong*, unsigned int, const SCountedPointerLong&, const __false_type&) -- placement copy-construct n copies of an 8-byte {intrusive counted pointer, long} record (add-ref each copy), return the advanced pointer
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CA500, length 47 bytes (+ int3 padding), `ret 8`.
// Caller: the STLport vector _M_insert_overflow at 0x009CB4E0 (fill_len != 1 branch, 0x009CB567), whose other branch
// inlines the same single construct; the landed sibling __uninitialized_copy over the same record is 0x009CA4C0.
// Identity: the catalog label `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` is a propagated Dinkumware name.
// The record is the 0x009CA4C0 one (a CIVCountedPointer-style pointer, count at +4 of the pointee, then a long); the
// class is not named by a landed relative, so it keeps the descriptive name.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, const __false_type&)` under /Gr (__fastcall: first in ecx,
// n in edx, x and the tag reference on the stack): `n > 0` on the unsigned count is the `jbe` guard, the count moves
// to esi because edx carries the copied pointer; the placement-new null test guards each construct.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

namespace std
{
    struct __false_type { __false_type() { } };     // STLport empty tag, passed by const reference

    template <class _ForwardIter, class _Size, class _Tp>
    _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
    {
        _ForwardIter __cur = __first;
        for (; __n > 0; --__n, ++__cur)
            new (&*__cur) _Tp(__x);
        return __cur;
    }
}

struct SRefCounted                       // intrusively counted pointee
{
    void* __vftable;                     // +0x00
    long  RefCount;                      // +0x04
};

struct SIVCountedPointer                 // CIVCountedPointer<T>
{
    SRefCounted* P;
    SIVCountedPointer(const SIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};

struct SCountedPointerLong               // 8 bytes
{
    SIVCountedPointer m_pointer;         // +0x00
    long              m_value;           // +0x04
};

template SCountedPointerLong* __fastcall std::__uninitialized_fill_n(SCountedPointerLong*, unsigned int, const SCountedPointerLong&, const std::__false_type&);