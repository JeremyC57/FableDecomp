// std::_Rb_tree<CCharString, pair<const CCharString, CBankFileManager::CBankInfo>, ...>::_M_erase(_Rb_tree_node*) -- STLport 4.6 _tree.c: frees a subtree without rebalancing (recurse right, destroy the value, free the node, continue left)
// Leaf: ?_M_erase@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@@std@@AAEXPAU?$_Rb_tree_node@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA7C0, 99 bytes, thiscall, `ret 4`, self-recursive.
// Identity: the out-of-line _M_erase of CBankFileManager::Banks (Ego_r.pdb member Banks +0x04, map<CCharString, CBankInfo>):
//   STLport 4.6 _tree.h clear() calls _M_erase(root) out of line, and the landed OpenIniFile 0x009A8170 (`Banks.clear()`,
//   0x009A8194) binds this address as this spelling; _M_erase is private in STLport 4.6 _tree.h (AAE). The value destructor is
//   ~pair<const CCharString, CBankInfo>: four ~CWideString 0x0099B510 (RetailHeaderFilePath .. DevBankFilePath) then ~CCharString
//   0x0099EAE0; node freed with free 0x00BFEA14.
// NF132 (q6186): was the stand-in CRetailTree_erase009aa7c0::Erase over a local node / SBankInfo model; now the explicit
//   instantiation of the shared stlport/fable_map.h tree over the retail fable_retail_banks.h CBankInfo.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_retail_banks.h"

template void std::_Rb_tree<CCharString, std::pair<const CCharString, CBankFileManager::CBankInfo>,
    std::_Select1st<std::pair<const CCharString, CBankFileManager::CBankInfo> >, std::less<CCharString>,
    std::allocator<std::pair<const CCharString, CBankFileManager::CBankInfo> > >::_M_erase(
    std::_Rb_tree_node<std::pair<const CCharString, CBankFileManager::CBankInfo> >*);
