// std::allocator<unsigned short>::allocate(unsigned int n, const void* hint) -- STLport: n ? malloc(n * 2) : 0
// Leaf: ?allocate@?$allocator@G@std@@QAEPAGIPBX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0050B550, 27 bytes, `ret 8` (this and the hint unused): test n ; je -> xor eax,eax ;
//   add eax,eax ; push ; call malloc 0x00BFEA0E ; add esp,4.
// Identity: the STLport 4.6 allocator<_Tp>::allocate(size_type, const void*) body (as the landed 0x005B9DE7 / 0x0040DB10)
//   for a 2-byte element. The element type is probable, not proven: the row has no caller (the one xref.py hit, 0x00492F2C,
//   is the operand of a call) and sits between CDrawMapInfo::CDrawMapInfo 0x0050B490 and the CComboMultiplierData code;
//   the CWorldMap / CDrawMapInfo code around it keeps the 2-byte STLport vector<unsigned short> MapPlacement (Ego_r.pdb
//   CWorldMap +0x7C), so `unsigned short`.
// Shape: STLport's `__n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0` with two separate `ret 8` exits; the result is
//   returned, so `add esp,4` after the cdecl call.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

}   // namespace std

template unsigned short* std::allocator<unsigned short>::allocate(unsigned int, const void*);