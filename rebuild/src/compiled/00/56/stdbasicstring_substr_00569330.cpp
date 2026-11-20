// std::basic_string<char>::substr(size_type pos, size_type n) const -- STLport basic_string<char>::substr: bounds-check pos, then return a new string over [start+pos, start+pos+len) with len = min(n, size()-pos) named first
// Leaf: ?substr@?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@QBE?AV12@II@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00569330, length 86 bytes.
// Callees: _String_base::_M_throw_out_of_range() const @ 0x00565360 (puts("basic_string") + abort with exceptions
// off; `this` still in ecx), the private _M_range_initialize<char*> @ 0x00567D00 (out of line).
// Shape: the hidden result pointer is the first stack argument (returned in eax, `ret 0Ch`). STLport
//   if (pos > size()) _M_throw_out_of_range();
//   return basic_string(start + pos, start + pos + min(n, size() - pos));
// min is the const-reference template (`b < a ? b : a`): `size()-pos` is a temporary homed in the dead `pos`
// argument slot and the selected reference is dereferenced (`lea eax,[esp+0Ch] ; jb ; lea eax,[esp+10h] ; mov eax,[eax]`).
// The range constructor zeroes the three words of the result and passes the empty iterator tag homed in the
// dead `n` slot (`lea edx,[esp+10h]`). Landed callers 0x0083F508.. are the HTTP URL accessors. The manifest
// label "CTCElectrocutedEffect::CopyDataRange" is propagated; the earlier CNetString_Retail::Substr model was
// descriptive. Identity: STLport 4.6 _string.h substr (public, const); the landed callers 0x0083F420 / 0x0083F650 bind this
// spelling. Body: the shared stlport/fable_string.h member, explicitly instantiated, with the STLport _string.c bodies in the TU
// (stlport/fable_string_bodies.h: out of line; with the declarations alone VC7.1 keeps `this` live across the out-of-range
// call and the function grows to 94 bytes).
#include "stlport/fable_string_bodies.h"

template std::basic_string<char> std::basic_string<char>::substr(size_type, size_type) const;
