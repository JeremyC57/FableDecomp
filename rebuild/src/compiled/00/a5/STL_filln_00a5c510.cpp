// std::fill_n(C4DVector* first, unsigned int n, const C4DVector& value) -- assign value to n consecutive 4D vectors starting at first and return the end of the filled range
// Leaf: ??$fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5C510, 54 bytes, __fastcall (first ecx, n edx; the value pointer on the stack, `ret 4`).
// Callers: one, 0x00A5C350 -- GFClear4DVectors 0x00A5C300 (the non-SSE path copies the value to a local and fills with it).
// Element: Ego_r.pdb C4DVector (16 bytes: float X, Y, Z, W), implicit assignment (four dword moves).
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated; the body is STLport's fill_n (no placement new, one
//   stack argument).
// Shape: STLport 4.6 `for (; __n > 0; --__n, ++__first) *__first = __val; return __first;`.
class C4DVector {
public:
    float X;                                    // +0x0
    float Y;                                    // +0x4
    float Z;                                    // +0x8
    float W;                                    // +0xC
};

namespace std {
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter __fastcall fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for (; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}

template C4DVector* __fastcall fill_n(C4DVector*, unsigned int, const C4DVector&);
}