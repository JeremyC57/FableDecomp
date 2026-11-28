// Flags: 
// CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable() -- require a live, visible reacting-to thing, a nearby matching creature, and an ordered distance strictly greater than 3.0f; retail 0x009863e0, 304 bytes. Uses the generated flattened state-group header and actual CAIStateGroupBase/CAIBrain/CThingAICreature/CThing/CThingSearchTools headers. Donor caveat: OpinionReactionRoot is a flattened generated layout; its +0x04 brain link is viewed through CAIStateGroupBase. ThingReactingTo at +0x6b is a 9-byte CIntelligentPointer unavailable as a typed shared member, so the adapter is field-free. Native query helper: CThingSearchTools::GetPFirstThingInRoughArea @0x987430, filter init @0x986ed0, pointer conversion @0xa01b50. Radius constant 3.0f @0x0122ded4. FCOMP/TEST AH,0x41/JNE returns false for less, equal, or unordered; success is ordered greater-than only.
// Leaf: ?CommentAboutHeroIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Name fix (agentNF36 2026-11-16): the body is now the member itself, ?CommentAboutHeroIsRunnable@CAIStateGroup_OpinionReactionRoot@@AAE_NXZ
// (private; engine/CAIStateGroup_OpinionReactionRoot.h _BEHAVIOURS), was a CAIStateGroup_OpinionReactionRootView stand-in.
// NF137 (2026-11-20, q4994 / q5140): moved onto the retail class (engine/CAIStateGroup_OpinionReactionRoot.h _WIFEUPDATE + _THINGREACTINGTO and
//   the overlay opt-in _COMMENTABOUTHERO, as landed sibling 0x00986210). PeekThingSearchTools 0x008FD000 now binds the PDB
//   ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (Ego_r.pdb + engine/CAIStateGroupBase.h _PEEK), was the
//   CAIStateGroupBaseView pointer stand-in; the search 0x00987430 now binds its self row's PDB template instance
//   GetPFirstThingInRoughArea<CThingFilter_IsAICreatureMatchingTypeAndFactionNearby, const CThingAICreature> (QBE, both PDBs; was the
//   CThingSearchToolsView stand-in), the filter 0x00986ED0 its self row's PDB constructor (engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h;
//   was the CNearbyCreatureFilter stand-in), GetPos 0x004C73D0 the PDB ?GetPos@CThing@@QBEABVC3DVector@@XZ (were the CThingView / CThingAICreatureView
//   GetPosition stand-ins), GetPItem 0x00A01B50 the shared CBaseIntelligentPointer member. The +0x28 bit 0x20000 test is HasTC(TCI_HERO_OPINION_DEED_LOG)
//   (component bitset word 2, bit 17 = interface 81); the +0x91 bit 0 test is IsDead(). The scan box is C2DBoxI(pos.ToC2DCoordI(), 3).
#include <math.h>
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
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_THINGREACTINGTO
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_COMMENTABOUTHERO
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CWORLD
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/C2DCoordI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CWorld.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsAICreatureMatchingTypeAndFactionNearby.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_OpinionReactionRoot.h"


bool CAIStateGroup_OpinionReactionRoot::CommentAboutHeroIsRunnable()
{
    if (ThingReactingTo.Get() != 0 &&
        !ThingReactingTo.Get()->IsDead() &&
        ThingReactingTo.Get()->HasTC(TCI_HERO_OPINION_DEED_LOG)) {
        CThingAICreature& creature = GetCreature();
        C2DBoxI area(creature.GetPos().ToC2DCoordI(), 3);
        const CThingAICreature* nearby;
        if (PeekThingSearchTools().GetPFirstThingInRoughArea(area, CThingFilter_IsAICreatureMatchingTypeAndFactionNearby(creature, 5.0f, true), &nearby)) {
            CThingAICreature& self = GetCreature();
            CThing* thing = ThingReactingTo.Get();
            const C3DVector& creaturePosition = self.GetPos();
            const C3DVector& thingPosition = thing->GetPos();
            float dx = thingPosition.X - creaturePosition.X;
            float dy = thingPosition.Y - creaturePosition.Y;
            float dz = thingPosition.Z - creaturePosition.Z;
            float distance = sqrtf(dx * dx + dy * dy + dz * dz);
            return distance > 3.0f;
        }
    }
    return false;
}
