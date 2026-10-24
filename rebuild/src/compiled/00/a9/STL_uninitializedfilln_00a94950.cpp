// _STL::__uninitialized_fill_n(SKeyRecord32* first, unsigned int n, const SKeyRecord32& x, const __false_type&) -- copy-construct one 32-byte key record into n raw slots starting at first; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A94950, 80 bytes, `ret 8`: /Gr `__fastcall` (first in ecx -> eax as the
//   cursor, n in edx -> ecx counted down, the value reference and the empty __false_type tag on the stack).
// Identity: callers 0x00A95AAA (the public uninitialized_fill_n forwarder 0x00A95AA0, which homes the tag in its dead
//   value slot), 0x00A96BD2 (vector _M_insert_overflow 0x00A96B30, the fill_len != 1 arm) and 0x00A9710C (0x00A97040):
//   the 0x20-byte record vector of the animation/script-camera key-frame TU, whose __uninitialized_copy 0x00A95A10 is
//   landed with the same record model. The catalog/BSim label `fill<C3DAnimationStateAlpha*, ...>` is wrong
//   (C3DAnimationStateAlpha is 20 bytes and fill has no placement null test); the record type is descriptive.
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned n: `test edx,edx ; jbe`), the placement-new null test, the record's memberwise user copy constructor around
//   an inner 3-long record with its own copy constructor (+0x0c): the second induction pointer at +0x14, every word
//   through edi.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (null-tested)

struct SRecord12 {                                 // 3-long record with its own user copy constructor
    long m_a, m_b, m_c;
    SRecord12(const SRecord12& o) : m_a(o.m_a), m_b(o.m_b), m_c(o.m_c) { }
};

struct SKeyRecord32 {                              // 0x20 bytes (as the landed __uninitialized_copy 0x00A95A10)
    long      m_0, m_4, m_8;                       // +0x00 .. +0x08
    SRecord12 m_record;                            // +0x0c
    long      m_18, m_1c;                          // +0x18, +0x1c
    SKeyRecord32(const SKeyRecord32& o) : m_0(o.m_0), m_4(o.m_4), m_8(o.m_8), m_record(o.m_record), m_18(o.m_18), m_1c(o.m_1c) { }
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template SKeyRecord32* __fastcall __uninitialized_fill_n(SKeyRecord32* __first, unsigned int __n, const SKeyRecord32& __x, const __false_type&);
}