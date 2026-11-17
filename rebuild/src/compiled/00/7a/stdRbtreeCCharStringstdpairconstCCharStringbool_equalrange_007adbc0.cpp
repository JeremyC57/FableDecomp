// std::_Rb_tree<CCharString, pair<const CCharString, bool>, ..>::equal_range(const CCharString&) -- STLport 4.6 equal_range of the CCharString -> bool flag map (map<CCharString, bool>)'s tree: the out-of-line private _M_lower_bound / _M_upper_bound pair
// Leaf: ?equal_range@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@QAE?AU?$pair@U?$_Rb_tree_iterator@U?$pair@$$CBVCCharString@@_N@std@@U?$_Nonconst_traits@U?$pair@$$CBVCCharString@@_N@std@@@2@@std@@U12@@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ADBC0, 42 bytes.
// Identity: STLport 4.6 _tree.h equal_range over the CCharString -> bool flag map (map<CCharString, bool>); its callees _M_upper_bound 0x007ACD30 and _M_lower_bound 0x007ACBB0 are the same tree's private
//   const members (ABE: NF82 / NF87 self rows; the landed 0x007ADE60 binds both so).
// NF87: moved off the row-local CCharString / tree models onto stlport/fable_map.h with NF85's 0x00756B60 opt-in set, so the bounds bind
//   their private spellings; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "fable_string.h"
template std::pair<std::_Rb_tree<CCharString, std::pair<CCharString const, bool >, std::_Select1st<std::pair<CCharString const, bool > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, bool > > >::iterator, std::_Rb_tree<CCharString, std::pair<CCharString const, bool >, std::_Select1st<std::pair<CCharString const, bool > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, bool > > >::iterator> std::_Rb_tree<CCharString, std::pair<CCharString const, bool >, std::_Select1st<std::pair<CCharString const, bool > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, bool > > >::equal_range(const CCharString&);
