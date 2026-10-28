// std::swap(pair<unsigned long, C3DHelperPoint>& a, pair<unsigned long, C3DHelperPoint>& b) -- STLport swap of two helper-point map entries: copy a to a temporary, assign b to a, the temporary to b
// Leaf: ??$swap@U?$pair@KVC3DHelperPoint@@@std@@@std@@YIXAAU?$pair@KVC3DHelperPoint@@@0@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A31220, 95 bytes, `__fastcall` (/Gr: a in ecx, b in edx), `ret`.
// Identity (probable): std::swap<std::pair<unsigned long, C3DHelperPoint> > -- the 20-byte pair of the CPreAllocVectorMap
//   <ulong, C3DHelperPoint> sort in the C3DMeshFile TU (_Push_heap over that pair at 0x00A31380; landed 56-byte DummyObject
//   swaps 0x00A312E0 / 0x00A32390 beside it), and only a value with C3DHelperPoint's shape (a C3DVector with the FableWin
//   user copy ctor, then a long) reproduces retail's member-by-member temporary. Its only reference is the tail jump of
//   the unreferenced COMDAT 0x00A32330. The catalog label "CDebugScriptElement::CDebugScriptElement" is propagated (a swap
//   writes both arguments and returns nothing).
// Shape: STLport 4.6 `_Tp __tmp = __a; __a = __b; __b = __tmp;` over STLport's pair, whose user copy constructor copies
//   `first` (kept in eax) and then `second` -- C3DHelperPoint's implicit copy through C3DVector's user copy ctor, member by
//   member from [ecx+4] -- into the 0x14-byte temporary; the implicit assignments copy five dwords. (Opaque 16-byte values
//   are block-copied through `lea esi,[ecx+4]` instead.)
#pragma pack(push, 1)
class C3DVector {                                          // FableWin.pdb ??0C3DVector@@QAE@ABV0@@Z: a user copy ctor
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
#pragma pack(pop)
class C3DHelperPoint {                                     // Ego_r.pdb, 16 bytes
public:
    C3DVector Position;                                    // +0x00
    long      BoneIndex;                                   // +0x0c
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<pair<unsigned long, C3DHelperPoint> >(pair<unsigned long, C3DHelperPoint>&,
                                                                                  pair<unsigned long, C3DHelperPoint>&);
}