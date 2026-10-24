// _STL::uninitialized_fill_n(C3DTriangle2* first, unsigned int n, const C3DTriangle2& x) -- STLport three-argument front end: placement copy-construct n triangles from x, return first + n
// Leaf: ??$uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A520B0, length 48 bytes, `ret 4`.
// Caller: 0x00A5536E (the vector<C3DTriangle2> fill-insert helper). No callees.
// Identity: the catalog's `_Fill_n<unsigned long*, unsigned int, unsigned long>` is propagated. Element (Ego_r.pdb):
//   C3DTriangle2 = unsigned short VertexIndex[3] (6 bytes). STLport's type traits do not flag the class as POD, so the
//   `__false_type` path (placement construct with its null test) is taken.
// Shape: /Gr TU (`__fastcall` template: first in ecx -> eax, n in edx counted down, &x on the stack -> ecx); the inner
//   `__uninitialized_fill_n(..., __false_type())` is `__forceinline`; `__n > 0` on the unsigned count is the `jbe` entry
//   test; the implicit copy of the 6-byte record is a block copy through esi/edi (dword then word).
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)

class C3DTriangle2 {                                 // 6 bytes
public:
    unsigned short VertexIndex[3];                   // +0x00
};

namespace _STL {
struct __false_type { __false_type() { } };          // STLport _IS_POD_ITER result for a class element

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
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

template C3DTriangle2* __fastcall uninitialized_fill_n(C3DTriangle2*, unsigned int, const C3DTriangle2&);
}