// std::_Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::operator=(const _Rb_tree&) -- STLport 4.6 _tree.c tree assignment: unless self-assigned, clear 0x004DBB20, zero the count, then
//   reset the header to an empty tree or _M_copy 0x004E58EB the source root under the header, walk leftmost / rightmost, copy the count
// Leaf: ??4?$_Rb_tree@VCDefString@@U?$pair@$$CBVCDefString@@J@std@@U?$_Select1st@U?$pair@$$CBVCDefString@@J@std@@@3@V?$CCompareDefString@J@@V?$allocator@U?$pair@$$CBVCDefString@@J@std@@@3@@std@@QAEAAV01@ABV01@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E5878, 115 bytes, size TU (`and dword ptr [..],0`), thiscall `ret 4`.
// NF64 (round 7, q10710): an explicit instantiation of stlport/fable_map.h operator= (overlay opt-in FABLE_RETAIL_STLP_TREE_ASSIGN_BODY:
//   STLport's _tree.c text, _M_copy declared only) for the tree whose _M_copy 0x004E58EB NF61 re-authored; the value-type declarations
//   are that row's. It now binds _M_copy and clear under their STLport spellings. Was the stand-in `CRetailTree_004e58eb::operator=` (bound
//   ?Copy@CRetailTree_004e58eb / ?Clear@CRetailTree_004e58eb). Descriptive-by-library (retail is STLport 4.6; both PDBs are Dinkumware builds; FableWin.pdb
//   lists the class-level ??4CHeroAbilityStatWeight at 0x0191AD74, 31 bytes before the next symbol, as its own function).
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_TREE_ASSIGN_BODY
#include "stlport/fable_map.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_COMPARE_FRIEND
#include "engine/CDefString.h"
template<class T> class CCompareDefString {   // Ego_r.pdb / FableWin.pdb ??R?$CCompareDefString@J@@QBE_NABVCDefString@@0@Z; no shared header
public:
    bool operator()(const CDefString& a, const CDefString& b) const { return a.TablePos < b.TablePos; }
};
namespace std {
template _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >& _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >::operator=(const _Rb_tree<CDefString, pair<CDefString const, long>, _Select1st<pair<CDefString const, long> >, CCompareDefString<long>, allocator<pair<CDefString const, long> > >&);
}
