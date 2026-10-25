// _STL::bitset::set(unsigned int pos) -- range-checked set of one bit of CThing::UsingComponents (bitset<274>: the interface types a thing uses); returns *this
// Leaf: ?set@?$bitset@$0BBC@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CC090, 62 bytes (+ int3 padding), thiscall `ret 4`, *this in eax.
// Caller: CThing::AddTC 0x004C9D60 (landed; 0x004C9F17 `UsingComponents.set(type)`).
// Callees: MSVCR71 puts 0x00BFEAF2 and abort 0x00BFEAEC (import thunks); the literal 0x0122E068 is "bitset".
// Identity: bitset<274> (0x112 bits, nine words) -- the same UsingComponents bitset the landed
//   CThingFilter_HasTC::operator() 0x006A80A0 tests. The catalog's "std::bitset_set" is the right name.
// Shape: STLport 4.x `set(pos)`: `if (pos >= _Nb) __stl_throw_out_of_range("bitset")` with the no-exceptions
//   _STLP_THROW_MSG = `puts(msg); abort();` (abort noreturn, so the in-range path is the jump target and `this` stays
//   in eax), then `_Unchecked_set` = `_M_getword(pos) |= _S_maskbit(pos)` (`shr edx,5` word, `shl esi,cl` mask).
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