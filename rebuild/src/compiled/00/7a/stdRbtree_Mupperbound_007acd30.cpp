// std::_Rb_tree<CCharString, pair<const CCharString, bool>, ..>::_M_upper_bound(const CCharString&) const -- STLport 4.6 upper-bound walk of the CCharString -> bool flag map (map<CCharString, bool>)'s tree
// Leaf: ?_M_upper_bound@?$_Rb_tree@VCCharString@@U?$pair@$$CBVCCharString@@_N@std@@U?$_Select1st@U?$pair@$$CBVCCharString@@_N@std@@@3@U?$less@VCCharString@@@3@V?$allocator@U?$pair@$$CBVCCharString@@_N@std@@@3@@std@@ABEPAU_Rb_tree_node_base@2@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ACD30, 67 bytes (`ret 4`).
// Identity: STLport 4.6 _tree.h private const _M_upper_bound (ABE) over the CCharString -> bool flag map (map<CCharString, bool>): its callers 0x007ADBC0 equal_range (NF87) and the landed 0x007ADE60 bind
//   exactly this spelling (reloc_bindings). The old row's CCharStringMap_007acd30 / CMappedValue model and public (QBE) spelling were stand-ins
//   (descriptive-by-library: no PDB holds STLport instances).
// NF87: moved off the row-local tree model onto stlport/fable_map.h with NF85's 0x007564F0 opt-in set; explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_BOUNDS_OUTOFLINE
#define FABLE_RETAIL_STLP_TREE_BOUNDS_BODY_PASS22
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
namespace std {
template _Rb_tree_node_base* std::_Rb_tree<CCharString, std::pair<CCharString const, bool >, std::_Select1st<std::pair<CCharString const, bool > >, std::less<CCharString>, std::allocator<std::pair<CCharString const, bool > > >::_M_upper_bound(const CCharString&) const;
}
