// std::fill(C3DTriangle2* first, C3DTriangle2* last, const C3DTriangle2& val) -- assign val to every triangle of [first, last)
// Leaf: ??$fill@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A52080, length 45 bytes (+3 int3 pad), `ret 4`.
// Callers: 0x00A55350 and 0x00A553AB (the vector<C3DTriangle2> fill-insert helper). No callees.
// Element (Ego_r.pdb): C3DTriangle2 = unsigned short VertexIndex[3] (6 bytes).
// Shape: STLport `fill` verbatim in a /Gr TU (`__fastcall` template: first in ecx, last in edx, &val on the stack);
//   `for (; first != last; ++first) *first = val;` -- the implicit operator= of the 6-byte array record is a block copy
//   through esi/edi (dword then word, the source pointer reloaded each pass); the /O2 loop-head pad.
class C3DTriangle2 {                                 // 6 bytes
public:
    unsigned short VertexIndex[3];                   // +0x00
};

namespace std {
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template void __fastcall fill(C3DTriangle2*, C3DTriangle2*, const C3DTriangle2&);
}