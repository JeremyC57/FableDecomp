// std::basic_string<char>::_M_append_dispatch<char*>(char* first, char* last, const __false_type&) -- STLport string append dispatch for a non-integer iterator pair: append(first, last, iterator category)
// Leaf: ??$_M_append_dispatch@PAD@?$basic_string@DV?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEAAV01@PAD0ABU__false_type@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567E80, 13 bytes. Callee: the private range append(first, last, const forward_iterator_tag&)
// @ 0x00567400 (the `<char*>` instance; label "CTCElectrocutedEffect::InsertStringRange" is propagated).
// Unreferenced in the image (recovered from a mis-cut catalog row).
// Identity: STLport 4.6 _string.h `template <class _InputIter> _Self& _M_append_dispatch(_InputIter __f, _InputIter __l,
//   const __false_type&) { return append(__f, __l, _STLP_ITERATOR_CATEGORY(__f, _InputIter)); }` (private), the `char*` instance:
//   three stack arguments (the jump target returns `ret 0Ch`) and a tail jump to the `<char*>` append 0x00567400 (the `<const char*>`
//   instance would reach 0x00567D60). Descriptive-by-library: no PDB carries it (both PDBs are Dinkumware builds); the earlier
//   CRetailString stand-in model was descriptive.
// Shape: a sibling tail jump -- the arguments stay in place, the empty iterator-category temporary is homed
// in the dead `last` slot and its address overwrites the dead tag slot (`lea eax,[esp+8] ; mov [esp+0Ch],eax ; jmp`). The append
//   body is in the TU (stlport/fable_string_bodies.h, out of line); with the declaration alone VC7.1 makes a call, not the jump.
#include "stlport/fable_string_bodies.h"

template std::basic_string<char>& std::basic_string<char>::_M_append_dispatch<char*>(char*, char*, const std::__false_type&);
