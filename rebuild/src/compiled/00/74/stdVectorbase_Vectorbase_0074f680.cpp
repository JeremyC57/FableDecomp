// std::_Vector_base<std::pair<long,CCharString>, std::allocator<std::pair<long,CCharString> > >::_Vector_base(const allocator&) -- STLport 4.6: zero the start / finish / end-of-storage words and return `this` (a folded COMDAT: `ret 4`, three zero stores)
// Leaf: ??0?$_Vector_base@U?$pair@JVCCharString@@@std@@V?$allocator@U?$pair@JVCCharString@@@std@@@2@@std@@QAE@ABV?$allocator@U?$pair@JVCCharString@@@std@@@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074F680.
// Name: descriptive-by-library (both PDBs are Dinkumware builds). Its only caller, 0x00D17AB0 inside NScript::CQ_BountyHuntScript::
//   WatchForBoasts 0x00D17A80 (callers.py), is the _Vector_base constructor relocation of that byte-matching object, which binds exactly
//   this spelling (the vector<pair<long,CCharString> > it builds). The body has no type-specific code, so the folded address takes the
//   name its only caller binds; the propagated catalog label CIndexBuffer::CIndexBuffer has no caller.
// NF41 (2026-11-16): was a local `struct T { int f0, f1, f2; T* Init(int); }` stand-in; now one explicit instantiation of the shared
//   stlport/fable_containers.h _Vector_base.
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
template class std::_Vector_base<std::pair<long, CCharString>, std::allocator<std::pair<long, CCharString> > >;
