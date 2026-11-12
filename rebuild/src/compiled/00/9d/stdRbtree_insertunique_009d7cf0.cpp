// std::_Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*, std::_Identity<CBankFileAsyncData*>, std::less<CBankFileAsyncData*>, std::allocator<CBankFileAsyncData*> >::insert_unique(CBankFileAsyncData* const const&) -- STLport 4.6 insert_unique (stlport/fable_map.h)
// Leaf: ?insert_unique@?$_Rb_tree@PAVCBankFileAsyncData@@PAV1@U?$_Identity@PAVCBankFileAsyncData@@@std@@U?$less@PAVCBankFileAsyncData@@@3@V?$allocator@PAVCBankFileAsyncData@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@PAVCBankFileAsyncData@@U?$_Nonconst_traits@PAVCBankFileAsyncData@@@std@@@std@@_N@2@ABQAVCBankFileAsyncData@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D7CF0, oracle length 180 bytes.
// Identity: (1) the landed caller 0x009D5D30 (CBankFileAsyncData::CBankFileAsyncData) binds this exact spelling (rebuild/oracles/reloc_bindings.tsv); (2) Ego_r.pdb / FableWin.pdb carry the set<PAVCBankFileAsyncData@@> instantiation (_Tset_traits@PAVCBankFileAsyncData@@U?$less@).
// Model: the shared stlport/fable_map.h (STLport 4.6 _tree.h / _tree.c; FABLE_RETAIL_STLP_MAP); the explicit instantiation emits the member as
//   its own COMDAT. Replaces the generated stand-in (was _unattributed, a descriptive placeholder class).
#define FABLE_RETAIL_STLP_MAP
#include "stlport/fable_map.h"
class CBankFileAsyncData;
template std::pair<std::_Rb_tree_iterator<CBankFileAsyncData*, std::_Nonconst_traits<CBankFileAsyncData*> >, bool> std::_Rb_tree<CBankFileAsyncData*, CBankFileAsyncData*, std::_Identity<CBankFileAsyncData*>, std::less<CBankFileAsyncData*>, std::allocator<CBankFileAsyncData*> >::insert_unique(CBankFileAsyncData* const const&);
