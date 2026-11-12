// std::_Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*, std::_Identity<CBankFileAsyncData*>, std::less<CBankFileAsyncData*>, std::allocator<CBankFileAsyncData*> >::erase(std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData*> >, std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData*> >) -- STLport 4.6 erase (stlport/fable_map.h)
// Leaf: ?erase@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@std@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@std@@QAEXU?$_Rb_tree_iterator@PAVCBankFileAsyncData@@U?$_Nonconst_traits@PAVCBankFileAsyncData@@@std@@@2@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D7760, oracle length 189 bytes.
// Identity: (1) the landed caller 0x009D7F40 (std::_Rb_tree<CBankFileAsyncData *, CBankFileAsyncData *, std::_Identity<CBankFileAsyncData *>, std::less<CBankFileAsyncData *>, std::allocator<CBankFileAsyncData *>>::erase) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the set<PAVCBankFileAsyncData@@> instantiation (_Tset_traits@PAVCBankFileAsyncData@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_STLP_TREE_POSTINC
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#include "stlport/fable_map.h"
class CBankFileAsyncData;
template void std::_Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*, std::_Identity<CBankFileAsyncData*>, std::less<CBankFileAsyncData*>, std::allocator<CBankFileAsyncData*> >::erase(std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData*> >, std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData*> >);
