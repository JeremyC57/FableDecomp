// std::map<CCharString, CBankFileManager::CBankInfo>::operator[](const CCharString& key) -- STLport 4.6 _map.h: lower_bound, then insert(i, value_type(key, CBankInfo())) when the key is absent; returns the mapped CBankInfo (CBankFileManager::Banks)
// Flags: /O2 /Oy /W3
// Leaf: ??A?$map@VCCharString@@VCBankInfo@CBankFileManager@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCBankInfo@CBankFileManager@@@std@@@5@@std@@QAEAAVCBankInfo@CBankFileManager@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC5D0, 302 bytes, thiscall, `ret 4`.
// Identity: the landed CBankFileManager::OpenIniFile 0x009A8170 (Ego_r.pdb) binds 0x009AC5D0 under this spelling for Banks[...]
//   (Ego_r CBankFileManager +0x04 map<CCharString, CBankInfo>, fable_retail_banks.h). Callees: the tree's _M_lower_bound 0x009AADB0,
//   CBasicString<char>::operator< 0x00429950 (through the inline CCharString::operator<), the CWideString default / copy / destructor
//   (0x0099AED0 / 0x0099B720 / 0x0099B510), CCharString copy / destructor (0x0099EC30 / 0x0099EAE0) and the tree's hinted
//   insert_unique 0x009ABB10.
// NF80 (q10923): moved off the stand-in namespace N009ac5d0 (local CCharString / CWideString / CBankInfo / map / node models) onto
//   stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[]; FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE for the out-of-line _M_lower_bound),
//   fable_retail_banks.h (FABLE_RETAIL_CBANKFILEMANAGER: CBankFileManager::CBankInfo) and fable_string.h (CBASICSTRING_LESS_BODY).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_retail_banks.h"
template CBankFileManager::CBankInfo& std::map<CCharString, CBankFileManager::CBankInfo, std::less<CCharString>, std::allocator<std::pair<const CCharString, CBankFileManager::CBankInfo> > >::operator[](const CCharString&);
