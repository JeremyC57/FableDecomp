// CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CIsSeatWithSpaceOwnedBy>(const CIsSeatWithSpaceOwnedBy& filter, std::vector<CThingObject*>& result) const -- append every scan-visible thing of the CThingObject type list passing the filter to the result vector
// Leaf: ??$GetAllLeafThings@VCThingObject@@V?$vector@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@VCIsSeatWithSpaceOwnedBy@@@CThingSearchTools@@QBEXABVCIsSeatWithSpaceOwnedBy@@AAV?$vector@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00910A90, 136 bytes (`ret 8`).
// Identity: Ego_r.pdb 0x00935A50 + FableWin.pdb 0x02B75240 ??$GetAllLeafThings@VCThingObject@@...VCIsSeatWithSpaceOwnedBy@@@CThingSearchTools@@QBEX...@Z
//   (the only CThingObject instantiation with a one-pointer owner filter); its filter's operator() is 0x009109B0 (Ego_r.pdb
//   0x00935940 ??RCIsSeatWithSpaceOwnedBy@@QBE_NABVCThingObject@@@Z); caller CAIStateGroup_MinionSitOnSeatBase::GetSeat 0x009104D0.
// Callees: PeekTypeList(5) 0x0049C780, CIsSeatWithSpaceOwnedBy::operator() 0x009109B0, vector<CThingObject*>::_M_insert_overflow 0x0074F900.
// NF127 (2026-11-20, q4992 / q4993): moved off the local CThing / CThingList / STLport vector models and the CThingSearchToolsMethods /
//   CUnprovenThingFilter_009109b0 stand-ins onto engine/CThingSearchTools.h (the FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY template body,
//   the one GetSeat 0x009104D0 expands from), with the PDB filter class as GetSeat declares it; the instance is emitted by an explicit
//   instantiation. The preamble is GetSeat's.
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY_CASTARG   // NF127 overlay opt-in: the push_back argument as a cast temporary
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE   // the alive test as IsAlive()'s `!Dead` (`mov cl,al ; not cl ; test cl,1` at 0x00910AC1), as the landed GetAllLeafThings COMDAT rows (wfA1)
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGOBJECT_STATGETTYPE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"

class CIsSeatWithSpaceOwnedBy {                                     // Ego_r.pdb: ctor (const CThing*), operator()(const CThingObject&) const
public:
    CIsSeatWithSpaceOwnedBy(const CThing* owner) : Owner(owner) { }
    bool operator()(const CThingObject& seat) const;                // 0x009109B0
private:
    const CThing* Owner;
};

template void CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CIsSeatWithSpaceOwnedBy>(
    const CIsSeatWithSpaceOwnedBy& filter, std::vector<CThingObject*>& result) const;
