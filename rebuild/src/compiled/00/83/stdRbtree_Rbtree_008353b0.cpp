// std::_Rb_tree<void*, void*, std::_Identity<void*>, std::less<void*>, std::allocator<void* > >::_Rb_tree(const std::less<void*>&, const std::allocator<void* >&) -- STLport 4.6 tree constructor _Rb_tree(const _Compare&, const allocator_type&): allocate the header node, colour it red, root 0, leftmost/rightmost = header
// Leaf: ??0?$_Rb_tree@PAXPAXU?$_Identity@PAX@std@@U?$less@PAX@2@V?$allocator@PAX@2@@std@@QAE@ABU?$less@PAX@1@ABV?$allocator@PAX@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008353B0, 56 bytes.
// Types (proven, agentFAM11, 2026-11-12): set<void*> (agentFAM07 proved this tree for 0x008356C0 set<void*>::~set, RENAMES 2026-11-11): CThingAICreature::Suspenders (Ego_r.pdb set<void*>, the creature's suspend tokens): the landed ~CThingAICreature 0x008320C0 calls this tree's _M_erase 0x00834FD0 and binds it as _Rb_tree<void*, void*, _Identity<void*>, ...>; node 0x14..
//   This _Rb_tree constructor 0x008353B0 is the COMDAT right before the tree's ~_Rb_tree 0x008353F0, which calls that _M_erase 0x00834FD0.
//   Was the placeholder std::_Rb_tree<CUnprovenKey_00834fd0,CUnprovenValue_00834fd0,CUnprovenKeyOfValue_00834fd0,std::less<CUnprovenKey_00834fd0>,std::allocator<CUnprovenValue_00834fd0> >::_Rb_tree (leaf ??0?$_Rb_tree@UCUnprovenKey_00834fd0@@UCUnprovenValue_00834fd0@@UCUnprovenKeyOfValue_00834fd0@@U?$less@UCUnprovenKey_00834fd0@@@std@@V?$allocator@UCUnprovenValue_00834fd0@@@5@@std@@QAE@ABU?$less@UCUnprovenKey_00834fd0@@@1@ABV?$allocator@UCUnprovenValue_00834fd0@@@1@@Z); name-only change: the same member, the per-instantiation placeholder types replaced by
//   the proven ones.
// Model: the shared stlport/fable_map.h (STLport 4.6 _Rb_tree / set; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits
//   the member as its own COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"

template std::_Rb_tree<void*, void*, std::_Identity<void*>, std::less<void*>, std::allocator<void* > >::_Rb_tree(const std::less<void*>&, const std::allocator<void* >&);
