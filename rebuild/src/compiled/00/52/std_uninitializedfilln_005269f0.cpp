// std::__uninitialized_fill_n(CUnprovenElement16_005269f0* first, unsigned int n, const CUnprovenElement16_005269f0& x, const __false_type&) -- copy-construct n copies of one 16-byte element into raw storage; returns the end of the run
// Leaf: ??$__uninitialized_fill_n@PAUCUnprovenElement16_005269f0@@IU1@@std@@YIPAUCUnprovenElement16_005269f0@@PAU1@IABU1@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005269F0, 58 bytes, __fastcall (first ecx, n edx), `ret 8` (x, the tag reference).
// Identity: STLport 4.6 __uninitialized_fill_n (stl/_uninitialized.h) -- the fill_len != 1 path of a vector's
//   _M_insert_overflow (its only caller, 0x0052B030 at 0x0052B0B8, passes the `atend` slot as the __false_type tag); that
//   _M_insert_overflow is reached only from an unreferenced push_back (0x0052B970), so the 16-byte element type is
//   unproven: CUnprovenElement16_005269f0 (descriptive), a trivially copyable 16-byte record (STLport still takes the
//   __false_type path: every user class is non-POD to its type traits).
// Shape: `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` with an unsigned count (`test edx,edx ; jbe`), the
//   placement-new null test on the destination, the implicit (trivial) copy inline as a block copy: source and
//   destination re-based into esi / edi each pass, one dword at a time through ebx (a memberwise user copy constructor
//   copies straight from ecx / eax instead), the end pointer returned in eax.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

struct CUnprovenElement16_005269f0 {           // 16 bytes, trivially copyable (implicit copy constructor)
    unsigned long Word0;                        // +0x00
    unsigned long Word1;                        // +0x04
    unsigned long Word2;                        // +0x08
    unsigned long Word3;                        // +0x0C
};

namespace std {

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

template CUnprovenElement16_005269f0* __fastcall __uninitialized_fill_n(CUnprovenElement16_005269f0*, unsigned int,
                                                                         const CUnprovenElement16_005269f0&, const __false_type&);

} // namespace std