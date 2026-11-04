// CMessageEventFilter_CanBeHeard::operator()(const CMessageEvent& event) const -- message filter: can the filter's creature hear the event
// Leaf: ??RCMessageEventFilter_CanBeHeard@@QBE_NABVCMessageEvent@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF270, 7 bytes: `mov ecx,[ecx] ; jmp 0x008332E0` (Creature->IsMessageEventWithinHearingRange(event)).
// Identity (two sources): Ego_r.pdb ??RCMessageEventFilter_CanBeHeard@@QBE_NABVCMessageEvent@@@Z in the donor run CanBeSeenOrHeard
//   0x006C9910 / CanBeSeen 0x006C9950 / CanBeHeard 0x006C9960 / MessageIDGreaterThan 0x006C9970, which retail keeps at 0x006AF220 (calls
//   the sight test 0x00833230 then the hearing test 0x008332E0) / 0x006AF260 (`jmp 0x00833230`) / 0x006AF270 (`jmp 0x008332E0`) /
//   0x006AF280 (the landed MessageIDGreaterThan +0x40 compare); Ego_r's CMessageEventFilter_CanBeHeard holds only CThingAICreature*
//   Creature at +0; the callers are message searches (FindMessage 0x0090ED25, FindMostRecentMessage<CFilter_Bind3<..>> 0x0090EDF3).
//   The catalog's "_Cons_val<..CParticleMorphs..>" was a propagated label.
// Callees: ?IsMessageEventWithinHearingRange@CThingAICreature@@QBE_NABVCMessageEvent@@@Z 0x008332E0 (bound by landed rows under an
//   invented name; NAME_FIXES.tsv).
// Header: engine/CMessageEventFilter_CanBeHeard.h (new), engine/CThingAICreature.h (FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEventFilter_CanBeHeard.h"
bool CMessageEventFilter_CanBeHeard::operator()(const CMessageEvent& event) const
{
    return Creature->IsMessageEventWithinHearingRange(event);
}