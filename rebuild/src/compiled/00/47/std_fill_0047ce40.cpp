// std::fill(unsigned char* first, unsigned char* last, const unsigned char& val) -- STLport byte fill: copy the value, memset the range
// Retail: Fable.exe (TLC, VC7.1) @ 0x0047CE40, 46 bytes, `ret 4`.
// Callees: none (the memset intrinsic: the byte is splatted into ebx/eax, `rep stosd` for size/4 and
// `rep stosb` for the remainder).
// Shape: STLport 4.x's `unsigned char` overload of fill -- `unsigned char __tmp = __val; memset(__first,
// __tmp, __last - __first);` -- in a `/Gr` algorithm TU, so the free template is `__fastcall` (first in
// ecx, last in edx, the value by const reference on the stack). It is also what the inlined POD
// `fill_n` / `uninitialized_fill_n` of vector<unsigned char> call (callers 0x00485BB6, 0x00485F97/A8 in
// _M_insert_overflow / _M_fill_insert). The manifest label "Memory_FillBytes" names the effect only.
// Leaf: ?fill@std@@
#include <string.h>

namespace std {

void __fastcall fill(unsigned char* __first, unsigned char* __last, const unsigned char& __val)
{
    unsigned char __tmp = __val;
    memset(__first, __tmp, __last - __first);
}

}