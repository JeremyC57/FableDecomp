// std::fill(C3DGroup2* first, C3DGroup2* last, const C3DGroup2& value) -- STLport fill: assign value to every element of [first, last)
// Leaf: ??$fill@PAVC3DGroup2@@V1@@std@@YIXPAVC3DGroup2@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ae79f0, 46 bytes, /Gr __fastcall (first in ecx, last in edx, &value on the stack), `ret 4`.
// Identity (probable): the element's assignment is BoneIndex then vector<C3DVertexBlend2>::operator= 0x00AE6690 (bound) on +4 = Ego_r.pdb C3DGroup2; its caller is the fill insert 0x00AE82C0 of the vector<C3DGroup2> whose __copy 0x00AE7660 / __uninitialized_copy 0x00AE7530 are families L049 / L011.
// Callee: the second member's operator= 0x00AE6690 (thiscall).
// Shape (family L052): `for (; first != last; ++first) *first = value;` -- the element's implicit assignment copies the
//   leading dword through eax and calls the member assignment with &value + 4 kept in ebp; stride 16.
namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector {                                             // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                        // +0x00
    _Tp* _M_finish;                                       // +0x04
    _Tp* _M_end_of_storage;                               // +0x08
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // out of line
};
}
template <class T> class CArray : public std::vector<T> { };   // retail CArray<T> = the 12-byte STLport vector
class C3DVertexBlend2 {                                    // Ego_r.pdb, 8 bytes
public:
    unsigned long VertexIndex;                            // +0x00
    float BlendValue;                                     // +0x04
};
class C3DGroup2 {                                          // Ego_r.pdb, 16 bytes in retail
public:
    long BoneIndex;                                       // +0x00
    CArray<C3DVertexBlend2> VertexBlends;                 // +0x04
};
namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}
}
template void __fastcall std::fill(C3DGroup2*, C3DGroup2*, const C3DGroup2&);