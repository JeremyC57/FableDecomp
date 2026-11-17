// map<CCharString, NBankFileManager::CContainedBank, less<CCharString>, allocator<pair<const CCharString, NBankFileManager::CContainedBank > > >::operator[](const CCharString& k) -- STLport subscript on a retail bank's contained-bank map: lower_bound, insert (k, CContainedBank()) when absent, return the value
// Leaf: ??A?$map@VCCharString@@VCContainedBank@NBankFileManager@@U?$less@VCCharString@@@std@@V?$allocator@U?$pair@$$CBVCCharString@@VCContainedBank@NBankFileManager@@@std@@@5@@std@@QAEAAVCContainedBank@NBankFileManager@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009AC530, 156 bytes, `ret 4`.
// Name: FableWin.pdb ??A?$map@VCCharString@@VCContainedBank@NBankFileManager@@...@std@@QAEAAVCContainedBank@NBankFileManager@@ABVCCharString@@@Z
//       (the catalog's "CContainedBankMap::operator[]" is the map typedef); the map is CRetailBank::ContainedBanks (+0x18, ctor
//       0x009AC4D0). Only caller 0x009A8D87.
// Callees: _M_lower_bound @ 0x009AAD60 (node pointer in eax), CBasicString<char>::operator< @ 0x00429950 (landed), CCharString copy
//          ctor @ 0x0099EC30 and dtor @ 0x0099EAE0 (the value_type temporary), _Rb_tree::insert_unique(iterator, const value_type&) @ 0x009AB870
//          (iterator argument built in its slot, the result returned through the dead `k` slot).
// Shape: STLport 4.x verbatim, as the landed map<CCharString, unsigned long>::operator[] 0x0042D131 -- but in an /O2 TU, where
//        less<CCharString> and CCharString::operator< are inline (pointer-equality, null tests, then the CBasicString compare);
//        CContainedBank() (FableWin.pdb has a user default ctor) zeroes the five words of the pair's second; map::insert is the
//        STLport inline forwarder to the tree's insert_unique, whose by-value result is copied back from its slot (a direct
//        out-of-line insert reads the result through eax instead).
// NF85: moved off the row-local CBasicString / CCharString / STLport map and tree models onto the shared headers (the landed
//   0x009AC5D0 map<CCharString, CBankFileManager::CBankInfo>::operator[] recipe): stlport/fable_map.h (FABLE_RETAIL_STLP_MAP operator[],
//   FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE: lower_bound over the tree's private out-of-line _M_lower_bound) and fable_string.h. The own
//   symbol is the four-argument STLport map spelling (was the three-argument stand-in without the allocator), and _M_lower_bound binds the
//   private _Rb_tree spelling of its NF82 self row (was a map stand-in). FABLE_RETAIL_NBANKFILEMANAGER_CCONTAINEDBANK_CTOR (NF85 opt-in in
//   fable_retail_banks.h): the FableWin.pdb user default constructor ??0CContainedBank@NBankFileManager@@QAE@XZ, which the old row modelled.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_NBANKFILEMANAGER_CCONTAINEDBANK_CTOR
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILEMANAGER
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "fable_retail_banks.h"
template NBankFileManager::CContainedBank& std::map<CCharString, NBankFileManager::CContainedBank, std::less<CCharString>, std::allocator<std::pair<const CCharString, NBankFileManager::CContainedBank > > >::operator[](const CCharString&);
