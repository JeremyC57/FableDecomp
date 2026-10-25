// _STL::bitset::test(unsigned int pos) const -- range-checked test of one bit of CThing::UsingComponents (bitset<274>: the interface types a thing uses)
// Leaf: ?test@?$bitset@$0BBC@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00410DE0, 63 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: bitset<274> (0x112 bits, nine words) -- the UsingComponents bitset of the landed bitset::set 0x004CC090 /
//   reset 0x004CC0D0 and CThingFilter_HasTC::operator() 0x006A80A0; 760 call sites (the out-of-line copy every
//   GetInterface/HasTC caller reaches). The catalog's "std::bitset::test" is the right name.
// Callees: MSVCR71 puts 0x00BFEAF2 and abort 0x00BFEAEC (import thunks); the literal 0x0122E068 is "bitset".
// Shape: STLport 4.x `test(pos)`: `if (pos >= _Nb) __stl_throw_out_of_range("bitset")` with the no-exceptions
//   _STLP_THROW_MSG = `puts(msg); abort();` (abort noreturn: the in-range path is the jump target), then
//   `_Unchecked_test` = `(_M_getword(pos) & _S_maskbit(pos)) != 0` (`shl eax,cl` mask, `shr edx,5` word, the bool by
//   `neg/sbb/neg`); `this` is kept in esi because the shift needs cl.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC (MSVCR71 import thunk)

namespace _STL {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.x bitset (exceptions off)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) {                                 // __stl_throw_out_of_range("bitset") without exceptions
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};
template bool bitset<274>::test(unsigned int) const;
}