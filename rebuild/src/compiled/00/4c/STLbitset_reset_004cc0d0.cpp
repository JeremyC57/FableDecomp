// std::bitset::reset(unsigned int pos) -- range-checked clear of one bit of CThing::UsingComponents (bitset<274>: the interface types a thing uses); returns *this
// Leaf: ?reset@?$bitset@$0BBC@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC0D0, 64 bytes, thiscall `ret 4`, *this in eax.
// Caller: CThing::RemoveTC 0x004C9840 (landed; 0x004C988D `UsingComponents.reset(type)`).
// Callees: MSVCR71 puts 0x00BFEAF2 and abort 0x00BFEAEC (import thunks); the literal 0x0122E068 is "bitset".
// Identity: bitset<274> (0x112 bits), the sibling of set 0x004CC090. The catalog's "std::bitset_reset" is the right name.
// Shape: STLport 4.x `reset(pos)`: the same range check (`puts("bitset"); abort();`), then `_Unchecked_reset` =
//   `_M_getword(pos) &= ~_S_maskbit(pos)` (`not esi ; and ecx,esi`); VC7.1 schedules `push esi` before `mov edx,ecx` here
//   but after it in set -- both from the same text.
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2 (MSVCR71 import thunk)
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC (MSVCR71 import thunk)

namespace std {
template <unsigned int _Nb>
class bitset {                                              // STLport 4.x bitset (exceptions off)
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long& _M_getword(unsigned int __pos) { return _M_w[_S_whichword(__pos)]; }
    bitset<_Nb>& _Unchecked_set(unsigned int __pos)
    {
        _M_getword(__pos) |= _S_maskbit(__pos);
        return *this;
    }
    bitset<_Nb>& _Unchecked_reset(unsigned int __pos)
    {
        _M_getword(__pos) &= ~_S_maskbit(__pos);
        return *this;
    }
    bitset<_Nb>& set(unsigned int __pos)
    {
        if (__pos >= _Nb) {                                 // __stl_throw_out_of_range("bitset") without exceptions
            puts("bitset");
            abort();
        }
        return _Unchecked_set(__pos);
    }
    bitset<_Nb>& reset(unsigned int __pos)
    {
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_reset(__pos);
    }
};
template class bitset<274>;
}