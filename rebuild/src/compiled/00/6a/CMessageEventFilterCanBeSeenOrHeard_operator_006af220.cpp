// CMessageEventFilter_CanBeSeenOrHeard::operator()(const CMessageEvent& event) const -- message filter: can the filter's creature see or
//   hear the event (sight test first)
// Leaf: ??RCMessageEventFilter_CanBeSeenOrHeard@@QBE_NABVCMessageEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AF220, 49 bytes, `ret 4`: Creature in esi, the event in edi; IsMessageEventWithinSightRange
//   0x00833230, then IsMessageEventWithinHearingRange 0x008332E0; `mov eax,1` / `xor eax,eax`.
// Name (NF118, PRIORITY q5078): Ego_r.pdb 0x006C9910 / FableWin.pdb 0x02111FF0 ??RCMessageEventFilter_CanBeSeenOrHeard@@QBE_NABVCMessageEvent@@@Z,
//   first of the Ego_r donor run CanBeSeenOrHeard / CanBeSeen 0x006AF260 / CanBeHeard 0x006AF270 (landed) / MessageIDGreaterThan
//   0x006AF280; the landed callers 0x00746260 / 0x00910CF0 bind this spelling. Was the descriptive stand-in
//   CDistanceFilter_006af220::operator()(CThing*) on a local checker struct.
// Header: engine/CMessageEventFilters.h (the YELLFORHELP_UPDATE class: unary_function<CMessageEvent, bool>, Creature +0x00) and
//   engine/CThingAICreature.h FABLE_RETAIL_CTHINGAICREATURE_MESSAGERANGE (the two range tests), as the sibling row 0x006AF270.
// Inferred: the named local for Creature (retail loads the creature once into esi; the member read twice reloads it after the call).
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
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_YELLFORHELP_UPDATE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEventFilters.h"
bool CMessageEventFilter_CanBeSeenOrHeard::operator()(const CMessageEvent& event) const
{
    const CThingAICreature& creature = Creature;
    return creature.IsMessageEventWithinSightRange(event) || creature.IsMessageEventWithinHearingRange(event);
}
