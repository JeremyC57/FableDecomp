// std::allocator<CDateAndTime>::allocate(unsigned int __n, const void* hint) -- STLport: n ? malloc(n * 28) : 0
// Leaf: ?allocate@?$allocator@VCDateAndTime@@@std@@QAEPAVCDateAndTime@@IPBX@Z
// Retail: Fable.exe @ 0x0040DB10, 28 bytes, __fastcall `ret 8` (this and the hint unused): `test ; je ; imul 1Ch ;
// push ; call malloc ; add esp,4`, else `xor eax,eax`.
// Identity (probable): the out-of-line STLport 4.6 allocator<T>::allocate (same body as the landed
// allocator<CEngineTextureMorph>::allocate 0x005B9DE7 with a 20-byte stride). The 0x1C stride is Ego_r.pdb's 28-byte
// CDateAndTime (Year .. Millisecond), and this TU's neighbours are the vector<CDateAndTime> helpers
// (std::__uninitialized_copy<CDateAndTime*> 0x0040DED0, __uninitialized_fill_n<CDateAndTime*> 0x0040DF00); no direct
// caller exists in retail (gap-scan COMDAT), so the element type rests on the stride and the neighbours.
// Callee: malloc @ 0x00BFEA0E (MSVCR71 import thunk).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CDateAndTime {
public:
    long          Year;                                 // +0x00
    unsigned long Month;                                // +0x04
    unsigned long Day;                                  // +0x08
    unsigned long Hour;                                 // +0x0C
    unsigned long Minute;                               // +0x10
    unsigned long Second;                               // +0x14
    unsigned long Millisecond;                          // +0x18
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};
}

template CDateAndTime* std::allocator<CDateAndTime>::allocate(unsigned int, const void*);