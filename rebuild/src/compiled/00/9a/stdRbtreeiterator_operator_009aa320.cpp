// std::_Rb_tree_iterator::operator--() -- map<CCharString, CBankFileManager::CBankInfo> iterator step to the in-order predecessor (the header steps to the maximum; else rightmost of the left subtree, else climb past left children) (STLport 4.6, _Nonconst_traits)
// Leaf: ??F?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@2@@std@@QAEAAU01@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AA320, thiscall on the iterator, returns it in eax.
// NF87 (2026-11-17): the catalog stand-in CRetailTreeIterator::Decrement was a generated name shared by every byte-identical body of
//   this shape; retail has no identical-code folding, so this address is one instantiation. Its only retail E8 callers are the
//   CBankInfo tree's insert_unique overloads (0x009ABB10 hinted, 0x009AB1F0 unhinted), whose landed rows bind this exact spelling.
// Shape: STLport 4.6 _tree.h `_Self& operator--() { _M_node = _Rb_global_inst::_M_decrement(_M_node); return *this; }` with the template
//   static inlined, out of line per instance in retail (stlport/fable_map.h FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE), emitted here by explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"

typedef std::pair<const CCharString, CBankFileManager::CBankInfo> _BankTreeValue;
template std::_Rb_tree_iterator<_BankTreeValue, std::_Nonconst_traits<_BankTreeValue> >&
std::_Rb_tree_iterator<_BankTreeValue, std::_Nonconst_traits<_BankTreeValue> >::operator--();
