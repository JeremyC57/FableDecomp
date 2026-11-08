#ifndef FABLE_ENGINE_NTARGETING_H
#define FABLE_ENGINE_NTARGETING_H
// New header (chunk 732, row 0x006AABE0 CThingPlayerCreature::SetActionForMelee): the NTargeting free function the row calls,
// Ego_r.pdb ?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z (0x007E4530; FableWin 0x02538AF0),
// retail 0x007EA2F0 (`lea ecx,<result> ; mov edx,<world>` then the thing and the float pushed). A declaration only.
#include "engine/C3DVector.h"
class CWorld;
class CThing;
namespace NTargeting {
#ifdef FABLE_RETAIL_NTARGETING_MELEE
    // Retail opt-in (chunk 867, row 0x0064BFBA NPlayerGui::CDrawHighlightingTargetingCursor::Draw): FableWin.pdb
    // ?IsThingVulnerableToMeleeStrikesFromThing@NTargeting@@YI_NABVCThing@@0@Z, out of line at 0x007F59F0 (`mov ecx,<thing> ;
    // mov edx,<hero> ; call`). Declaration only.
    bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(const CThing& thing, const CThing& attacker);   // 0x007F59F0
#endif
    C3DVector __fastcall GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float scale);   // 0x007EA2F0
}
#endif
