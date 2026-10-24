// _STL::uninitialized_fill_n(C3DTriangle2* first, unsigned int n, const C3DTriangle2& x) -- copy-construct one 6-byte triangle into n raw slots starting at first; returns the end of the run
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A95970, 46 bytes (+ int3 padding), `ret 4`: /Gr `__fastcall` (first in
//   ecx -> eax as the cursor, n in edx counted down, the value reference on the stack).
// Identity: the only caller is 0x00A96FD0, the C3DTriangle2 vector's fill-insert path that also calls
//   fill<C3DTriangle2*, C3DTriangle2> 0x00A945D0 (0x00A96FA9), so the 6-byte element is C3DTriangle2 (Ego_r.pdb:
//   ushort VertexIndex[3]). The catalog/BSim label `_Fill_n<unsigned long*, unsigned int, unsigned long>` is a
//   Dinkumware-era guess (the body copies 6-byte elements). `ret 4` = three arguments, no tag: the public
//   uninitialized_fill_n with STLport's tag-dispatched __uninitialized_fill_n inlined into it.
// Shape: STLport _uninitialized.h -- `cur = first; for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;`
//   (unsigned n: `test edx,edx ; jbe`), the placement-new null test, the implicit copy as a dword + word block copy.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (null-tested)

class C3DTriangle2 {
public:
    unsigned short VertexIndex[3];                 // +0x00
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}

template C3DTriangle2* __fastcall uninitialized_fill_n(C3DTriangle2* __first, unsigned int __n, const C3DTriangle2& __x);
}