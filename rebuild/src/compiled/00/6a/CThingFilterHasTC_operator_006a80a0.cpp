// CThingFilter_HasTC::operator()(const CThing& thing) const -- does the thing use the filter's component type (UsingComponents.test(Type), range-checked)
// Leaf: ??RCThingFilter_HasTC
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A80A0, length 56 bytes (+ int3 padding), `ret 4`.
// Callees: MSVCR71 puts @ 0x00BFEAF2 and abort @ 0x00BFEAEC (import thunks; the literal 0x0122E068 is "bitset").
// Identity: Ego_r.pdb `??RCThingFilter_HasTC@@QBE_NABVCThing@@@Z` with layout CThingFilter_HasTC { ETCInterfaceType Type; }
//   (4 bytes) -- the `mov eax,[ecx]` is this->Type. The catalog's "Bitset_Test" is the inlined callee's name; 77 call
//   sites (thing-search template instantiations across the game) reach this one body.
// Layout: CThing::UsingComponents (bitset<274>, 9 words) at retail +0x20 (`test [ecx+eax*4+20h],edx`).
// Shape: STLport 4.x bitset::test(pos) without exceptions: `if (pos >= _Nb) __stl_throw_out_of_range("bitset")` with
//   _STLP_THROW_MSG = `puts(msg), abort()` (abort noreturn: the in-range path is the jump target), then
//   _Unchecked_test = `(_M_w[pos / 32] & (1UL << (pos % 32))) != 0` (`setne al`).
extern "C" int __cdecl puts(const char* text);              // 0x00BFEAF2
extern "C" __declspec(noreturn) void __cdecl abort(void);   // 0x00BFEAEC

enum ETCInterfaceType { TC_NONE = 0 };

template <unsigned int _Nb>
class bitset {
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
        if (__pos >= _Nb) {
            puts("bitset");
            abort();
        }
        return _Unchecked_test(__pos);
    }
};

class CThing {
public:
    unsigned char _pad00[0x20];
    bitset<274> UsingComponents;                            // +0x20
};

class CThingFilter_HasTC {
public:
    ETCInterfaceType Type;                                  // +0x00
    bool operator()(const CThing& thing) const;
};

bool CThingFilter_HasTC::operator()(const CThing& thing) const
{
    return thing.UsingComponents.test(Type);
}