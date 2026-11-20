// CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CThingFilter_HasTC>(const CThingFilter_HasTC& filter, std::vector<CThingObject*>& result) const -- append every scan-visible thing of type list 5 that the filter accepts
// Leaf: ??$GetAllLeafThings@VCThingObject@@V?$vector@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@VCThingFilter_HasTC@@@CThingSearchTools@@QBEXABVCThingFilter_HasTC@@AAV?$vector@PAVCThingObject@@V?$allocator@PAVCThingObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0077EC20, 136 bytes (`ret 8`).
// Identity: the template instance CTCHeroCentre::FrameUpdate 0x0077D5B0 calls (its TU emits this COMDAT, RELOCATION_MATCH against this
//   address: hand3c3 b10) under the CThingSearchTools spelling of the landed sibling instances (0x00910A90 = Ego_r.pdb 0x00935A50 /
//   FableWin.pdb 0x02B75240, same template, another filter). Callees: PeekTypeList(5) 0x0049C780, CThingFilter_HasTC::operator()
//   0x006A80A0, vector<CThingObject*>::_M_insert_overflow 0x0074F900.
// Re-port (landing hand3c3 b10 follow-up, 2026-11-20; advisor decision, owner delegated): the row defined the instance on the local
//   CThingSearchToolsMethods stand-in (non-const), so b10's binding of the real spelling gave the address two names. Now the
//   0x00910A90 form: engine/CThingSearchTools.h's FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY body (with b10's define pair _CASTARG /
//   CTHING_SCANINCLUSION_ISALIVE, B's recipe) emitted by an explicit instantiation; the preamble is 0x00910A90's.
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
#define FABLE_RETAIL_CTHINGFILTER_HASTC_OUTOFLINE   // the filter's operator() is called out of line (0x006A80A0), as in 0x0077D5B0
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingObject.h"
#include "engine/CThingSearchTools.h"
#include "engine/CWorld.h"
#include "engine/CThingFilter_HasTC.h"

template void CThingSearchTools::GetAllLeafThings<CThingObject, std::vector<CThingObject*>, CThingFilter_HasTC>(
    const CThingFilter_HasTC& filter, std::vector<CThingObject*>& result) const;
