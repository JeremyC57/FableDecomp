// 00934f20 CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const -- of two creature groups pick the one whose nearest member is closer to the position (null / group-less sides lose)
// Leaf: ??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934F20, 281 bytes, `ret 8`. Identity: Ego_r.pdb / FableWin.pdb
//   ??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z (the comparator of
//   CThingSearchTools::GetPBestThing<CThingFilter_HasTC, CThingCompare_NearestGroupByMember, CThing>).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (CThing::GetTC<CTCCreatureGroup>, type 0xA3
//   TCI_CREATURE_GROUP), CTCCreatureGroup::GetNearestMember(const C3DVector&) const 0x008128F0 (Ego_r/FableWin
//   ?GetNearestMember@CTCCreatureGroup@@QBEPBVCThingAICreature@@ABVC3DVector@@@Z), CThing::GetPos() const 0x004C73D0.
// Layout: PDB CThingCompare_NearestGroupByMember { C3DVector Pos; } (12 bytes).
// Shape: null tests return the other side; the group component of `a` is fetched (end/hit pick) before `b`'s HasTC test;
//   retail asks a's group for its nearest member twice (b's pointer is fetched but unused); the closer member
//   wins by squared distance (first offset spilled to a frame local, the second on the x87 stack).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"

#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CThingAICreature.h"
#include "engine/CTCCreatureGroup.h"

#include "engine/CThingCompare_NearestGroupByMember.h"
template CThing* CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const;