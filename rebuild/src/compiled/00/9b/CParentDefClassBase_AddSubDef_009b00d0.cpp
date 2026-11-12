// CParentDefClassBase::AddSubDef(const CCharString& name, long def_index, long original_parent_def_index) -- file the sub-def
// under the CRC of its name in the SubDefInfoMap (CVectorMap Insert: inline STLport push_back, Dirty set) and re-sort the map
// (CVectorMap Update: std::sort when dirty).
// Leaf: ?AddSubDef@CParentDefClassBase@@UAEXABVCCharString@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009B00D0, 155 bytes, `ret 0Ch`; CParentDefClassBase vtable 0x01230CBC slot 12.
// Identity: Ego_r.pdb / FableWin.pdb ?AddSubDef@CParentDefClassBase@@UAEXABVCCharString@@JJ@Z; engine/CParentDefClassBase.h slot 12.
// Callees: CCRC::Calc 0x004014A0 (via the inline CCharString::GetCRC), vector<pair<unsigned long,CDefClassBase::CSubDefInfo> >
//   ::_M_insert_overflow 0x009B60D0, std::sort<pair<unsigned long,CSubDefInfo>*,CKeyPairCompareLess<..> > 0x009B84C0 (__fastcall).
// Layout: CParentDefClassBase SubDefInfoMap +0x29 (CVectorMap: CArray 0xc, Compare +0xc, Dirty +0xd).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CPARENTDEFCLASSBASE
#define FABLE_PDCB_TYPED_MEMBERS
#define FABLE_RETAIL_CDEFCLASSBASE_SUBDEFINFO_CTOR
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#include "engine/CDefString.h"
#include "engine/CVectorMap.h"
#include "engine/CParentDefClassBase.h"

void CParentDefClassBase::AddSubDef(const CCharString& name, long def_index, long original_parent_def_index)
{
    SubDefInfoMap.Insert(name.GetCRC(), CSubDefInfo(def_index, original_parent_def_index));
    SubDefInfoMap.Update();
}