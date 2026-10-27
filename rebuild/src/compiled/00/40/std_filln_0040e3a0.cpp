// std::fill_n(unsigned char* __first, unsigned int __n, const unsigned char& __val) -- STLport one-byte fill_n: memset n bytes to val, return first + n
// Leaf: ??$fill_n@I@std@@YIPAEPAEIABE@Z
// Retail: Fable.exe @ 0x0040E3A0, 53 bytes, register convention modelled as `__fastcall` (first in ecx, n in edx, &val
// on the stack, `ret 4`), as this TU's landed STLport helpers (0x0040DD00 _Construct, 0x0040DED0 __uninitialized_copy).
// Identity: STLport 4.6 `fill_n(unsigned char*, _Size, const unsigned char&)` -> `fill(first, first + n, val)` ->
// `memset(first, tmp, last - first)` (the /Oi intrinsic: byte replicated into eax, `rep stosd` + `rep stosb`), returning
// first + n (`lea eax,[esi+ebp]`); the value is read with movzx (unsigned char). Same chain as the landed
// vector<unsigned char> fill constructor 0x00567560. No direct caller in retail (gap-scan COMDAT); _Size = unsigned int
// is the size_type every STLport caller passes (descriptive choice: the body is identical for int).
extern "C" void* __cdecl memset(void* dst, int value, unsigned int size);
#pragma intrinsic(memset)

namespace std {
inline void fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    unsigned char __tmp = __val;
    memset(__first, __tmp, __last - __first);
}

template <class _Size>
unsigned char* __fastcall fill_n(unsigned char* __first, _Size __n, const unsigned char& __val)
{
    fill(__first, __first + __n, __val);
    return __first + __n;
}

template unsigned char* __fastcall fill_n<unsigned int>(unsigned char*, unsigned int, const unsigned char&);
}