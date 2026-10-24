// std::copy_backward(long* _First, long* _Last, long* _Dest) -- VC7.1 Dinkumware copy_backward over a 4-byte scalar element: the scalar-pointer `_Copy_backward` inlined, one memmove of the whole range to end at _Dest
// Leaf: ??$copy_backward@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13D70, 33 bytes, __fastcall (_First in ecx, _Last in edx, _Dest on
//   the stack, `ret 4`).
// Caller: the audio library's vector<T>::_Insert_n @ 0x00C14270 (call 0x00C14431: `copy_backward(_Where, _Oldend - _Count,
//   _Oldend)`), the vector at +0x18 of the second detail cache (0x00C14630). Callee: MSVCR71 memmove (import 0x01440144).
// Identity: this library (libpk, the audio/streaming code) uses VC7.1's own Dinkumware STL -- MSVCP71 basic_string
//   imports, 16-byte vectors (allocator word + first/last/end), `_Xlen` "vector<T> too long" @ 0x00C13840. The
//   Ego_r.pdb name set holds `std::_Copy_backward<T*,T*>(T*, T*, T*, _Scalar_ptr_iterator_tag)` for the same body,
//   but the tag would be a fourth (stack) argument (`ret 8`, probed): `ret 4` gives three arguments, so the
//   out-of-line copy is the public `copy_backward` with the tag dispatch inlined. The element type is not
//   recoverable (any 4-byte scalar gives these bytes); its twin @ 0x00C13D40 serves the first detail cache's _Insert_n.
// Shape: `sub edx,ecx ; sar edx,2` is the element count, `lea eax,[edx*4]` its byte size, the destination
//   `_Dest - _Off` formed after the first two pushes; the memmove result is returned directly.
// Label correction: the catalog's `_Copy_backward<EWeaponClass*,EWeaponClass*>` is the Ego_r helper propagated across
//   the byte-identical body (EWeaponClass is a game enum, not an audio-library type).
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x01440144

namespace std {
struct _Scalar_ptr_iterator_tag { };

template<class _BidIt1, class _BidIt2> inline
_BidIt2 _Copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest, _Scalar_ptr_iterator_tag)
{
    int _Off = _Last - _First;
    return ((_BidIt2)memmove(&*_Dest - _Off, &*_First, _Off * sizeof(*_First)));
}

template<class _BidIt1, class _BidIt2>
_BidIt2 __fastcall copy_backward(_BidIt1 _First, _BidIt1 _Last, _BidIt2 _Dest)
{
    return (_Copy_backward(_First, _Last, _Dest, _Scalar_ptr_iterator_tag()));
}

template long* __fastcall copy_backward<long*, long*>(long*, long*,
                                                                                long*);
}