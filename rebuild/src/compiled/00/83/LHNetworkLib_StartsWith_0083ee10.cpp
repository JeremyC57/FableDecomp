// LHNetworkLib::StartsWith(const string& text, const string& prefix) -- case-insensitive prefix test: the lower-cased text cut to the prefix length equals the lower-cased prefix
// Leaf: ?StartsWith@LHNetworkLib@@YI_NABV?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083EE10, 194 bytes, __fastcall (text in ecx, prefix in edx), bool in al, `ret`.
// Identity: Ego_r.pdb ?StartsWith@LHNetworkLib@@YI_NABV?$basic_string@...@std@@0@Z (YI: __fastcall, _N: bool), the third of
//   the Ego_r run toLowerCase / toUpperCase / StartsWith; TLC's toLowerCase is the callee 0x0083EBD0 (YI, hidden return in
//   ecx, the string in edx). Parameter names are descriptive. The manifest label "HTTP_CompareStrings" is an autoname.
// Callees: LHNetworkLib::toLowerCase 0x0083EBD0 (twice), STLport basic_string<char>::_M_range_initialize(const char*,
//   const char*, const forward_iterator_tag&) 0x00567D00 (the inline substr's result), free 0x00BFEA14 (the three
//   temporaries).
// Shape: `return toLowerCase(text).substr(0, prefix.size()) == toLowerCase(prefix);` -- prefix.size() spilled first (the
//   STLport `min(__n, size() - __pos)` takes it by reference), substr inline (the `__pos > size()` test folds away; the
//   out-of-range thrower is `__declspec(noreturn)`, which lets substr inline), operator== inline (size test, then the
//   memcmp intrinsic as `repe cmpsb`), the bool kept on the frame while the three temporaries are freed. Two shape
//   details: the free inlines are `__fastcall` (/Gr: STLport's `_STLP_CALL` is empty), which makes VC7.1 evaluate the
//   left operand of == first (a default-convention operator== calls toLowerCase(prefix) first); and the result string
//   is built from (start + pos, len) through the (const char*, size_type) constructor, whose `__s + __n` sits in the
//   same argument list as the iterator tag, so the add follows the tag push as in retail (the (first, last)
//   constructor computes the end pointer before the tag: one swapped instruction, 194 v 194).
// Port: the shared engine/LHStringFunctions_bodies.h body on engine/LHHttp2.h (stlport/fable_string.h).
#define FABLE_RETAIL_LHHTTP2
#define FABLE_RETAIL_STLP_STRING_THROW_INLINE
#include "engine/LHHttp2.h"
#include "engine/LHStringFunctions_bodies.h"
