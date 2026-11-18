// std::_Rb_tree<CCharString, pair<CCharString const, CBankFileManager::CBankInfo >, _Select1st<pair<CCharString const, CBankFileManager::CBankInfo > >, less<CCharString >, allocator<pair<CCharString const, CBankFileManager::CBankInfo > > >::insert_unique -- (map<CCharString, CBankFileManager::CBankInfo>) STLport 4.6 hinted unique insert over CCharString keys: the begin / end / interior hint cases, falling back on the unhinted insert_unique when the hint does not bracket v
// Leaf: ?insert_unique@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@3@@std@@QAE?AU?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@2@@2@U32@ABU?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@2@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ABB10, 668 bytes (+ int3 padding to 672), thiscall `ret 0Ch` (hidden iterator result,
//   position, v).
// Identity (proven): map<CCharString, CBankFileManager::CBankInfo>; evidence: its caller is the landed map<CCharString, CBankFileManager::CBankInfo>::operator[] 0x009AC5D0 (CBankFileManager::Banks); its insert_unique(v) 0x009AB1F0 is agent951's proven map<CCharString, CBankFileManager::CBankInfo> one (0x28-byte nodes); _M_insert 0x009AB120, operator++ 0x009A9E20, operator-- 0x009AA320.
// Label: the manifest's "map_operator_square_bracket_CCharString_2" is descriptive (family H006: seven rows that differ only in their call displacements;
//   retail has no identical-code folding, so each address is one instantiation); this is STLport 4.6
//   `_Rb_tree::insert_unique(iterator, const value_type&)` with a CCharString key at node +0x10.
// Callees (this instantiation): insert_unique(v) @ 0x009AB1F0 (pair result through a hidden pointer), _M_insert @ 0x009AB120,
//   iterator operator++ @ 0x009A9E20 / operator-- @ 0x009AA320 (out of line, thiscall on the iterator); shared:
//   CBasicString<char>::operator< 0x00429950 and less<CCharString>::operator() 0x0042AAB3 (VC7.1 inlines the key compare at
//   five sites and calls the out-of-line copy at the other three).
// Model: H003's hinted insert_unique (agent951 G028, the landed /O2 twin 0x00AD5030) with the CCharString key of agent951
//   G016: CCharString::operator< inline (equal data -> false, null left -> true, null right -> false) over the real
//   CBasicString<char>::operator< body as a same-TU noinline function.
// NF85: moved off the row-local CBasicString / CCharString / pair / tree models onto stlport/fable_map.h (the NF82 0x008ACA90 recipe: FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY with the iterator ++ / -- kept out of line, explicit
//   instantiation) and fable_string.h: the _M_insert callee is now bound under the tree's private STLport AAE spelling (its NF82 self
//   row), was the public QAE stand-in.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_INSERT_UNIQUE_HINT_BODY
#define FABLE_RETAIL_STLP_TREE_DEC_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_INC_OUTOFLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"
namespace std {
template _Rb_tree<CCharString, pair<CCharString const, CBankFileManager::CBankInfo >, _Select1st<pair<CCharString const, CBankFileManager::CBankInfo > >, less<CCharString >, allocator<pair<CCharString const, CBankFileManager::CBankInfo > > >::iterator _Rb_tree<CCharString, pair<CCharString const, CBankFileManager::CBankInfo >, _Select1st<pair<CCharString const, CBankFileManager::CBankInfo > >, less<CCharString >, allocator<pair<CCharString const, CBankFileManager::CBankInfo > > >::insert_unique(iterator, const pair<CCharString const, CBankFileManager::CBankInfo >&);
}
