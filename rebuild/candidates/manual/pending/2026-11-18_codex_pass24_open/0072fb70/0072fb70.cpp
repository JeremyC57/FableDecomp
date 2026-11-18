// CTCDummyVillager::FrameUpdate() -- schedule a missing villager and pick a concealed respawn location.
// Grade: DIFFER(1319v1278) -- The shared village and spawn APIs retain the native second marker pass, including frustum and line-of-sight gating. Definition cleanup, vector allocation and marker traversal scheduling still differ.
// Leaf: ?FrameUpdate@CTCDummyVillager@@UAEXXZ
// Retail: 0x0072FB70, 1278 bytes, thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CCREATUREDEF_TRANSFER
#define FABLE_RETAIL_CCREATUREDEF_RESPAWN_PASS24
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMETIMEMANAGER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_GFRANDOM_OUTOFLINE
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_RANDOM_SHUFFLE_2ARG
#define FABLE_RETAIL_CTHINGFILTER_ISNOTBEINGEXCLUSIVELYUSED_PASS24
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingMarker.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CWorld.h"
#include "engine/CGameTimeManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CCreatureDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/GFRandom.h"
#include "engine/ThingAlive.h"
#include "engine/CThingFilter_IsNotBeingExclusivelyUsed.h"
#include "engine/CRightHandedSet.h"
#include "engine/CTCPhysicsBase.h"

void CTCDummyVillager::FrameUpdate()
{
    if (IS_THING_ALIVE(Creature) || !CanBeRecreated())
        return;
    if (DayToBeRecreated == 0x7fffffff && FrameToBeRecreated == 0x7fffffff) {
        CDefPointer<const CThingBaseDef> pdef;
        GetDefinitionManager().GetDef(DefType, &pdef);
        CDefPointer<const CCreatureDef> creature_def;
        if (pdef->GetSubDef(&creature_def)) {
            if (creature_def->RespawnWaitInDaysMin == 0 && creature_def->RespawnWaitInDaysMax == 0) {
                DayToBeRecreated = GetWorld().PeekGameTimeManager().GetDayCount();
                FrameToBeRecreated = GFRandom(creature_def->RespawnWaitInFramesMax - creature_def->RespawnWaitInFramesMin,
                    GetPWorldSeed()) + GetWorld().GetFrame() + creature_def->RespawnWaitInFramesMin;
            } else {
                DayToBeRecreated = GFRandom(creature_def->RespawnWaitInDaysMax - creature_def->RespawnWaitInDaysMin,
                    GetPWorldSeed()) + creature_def->RespawnWaitInDaysMin + GetWorld().PeekGameTimeManager().GetDayCount();
                FrameToBeRecreated = 0x7fffffff;
            }
        }
    }
    bool recreate;
    if (FrameToBeRecreated != 0x7fffffff)
        recreate = GetWorld().GetFrame() >= FrameToBeRecreated;
    else
        recreate = GetWorld().PeekGameTimeManager().GetDayCount() >= DayToBeRecreated;
    if (!recreate)
        return;

    CTCVillageMember* village_member;
    if (!GetParentThing().GetTC(&village_member) || !village_member->GetPVillage())
        return;
    C3DVector spawn_pos;
    bool found_spawn_pos = false;
    CThingPlayerCreature* phero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    long def_index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("GAZE_OUT_OF_BUILDING_MARKER");
    const std::list<CIntelligentPointer<CThingMarker> >& village_markers = village_member->GetPVillage()->VillageMarkers;
    std::vector<CThingMarker*> shuffled_markers;
    for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i = village_markers.begin(); i != village_markers.end(); ++i)
        shuffled_markers.push_back(i->Get());
    std::random_shuffle(shuffled_markers.begin(), shuffled_markers.end());
    for (unsigned pass = 0; pass != 2 && !found_spawn_pos; ++pass) {
        const bool test_line_of_sight = pass != 0;
        for (std::vector<CThingMarker*>::iterator i = shuffled_markers.begin(); i != shuffled_markers.end(); ++i) {
            if (!IS_THING_ALIVE(*i) || (*i)->GetDefGlobalIndex() != static_cast<unsigned long>(def_index) ||
                !CThingFilter_IsNotBeingExclusivelyUsed()(**i))
                continue;
            if (GetWorld().GetGameCamera()->IsPosInViewingFrustrum((*i)->GetPos())) {
                if (!test_line_of_sight || GetWorld().GetGameCamera()->IsLineOfSightToPos((*i)->GetPos()))
                    continue;
            }
            if (found_spawn_pos && !((spawn_pos - phero->GetPos()).GetMagnitude() > ((*i)->GetPos() - phero->GetPos()).GetMagnitude()))
                continue;
            found_spawn_pos = true;
            spawn_pos = (*i)->GetPos();
            if (test_line_of_sight)
                break;
        }
    }
    if (!found_spawn_pos) {
        if (CTCVillage* ptc_village = village_member->GetPVillage())
            spawn_pos = ptc_village->GetParentThing().GetPos();
    }
    if (CTCVillage* ptc_village = village_member->GetPVillage()) {
        CreateVillager(*ptc_village, spawn_pos, true);
        if (IS_THING_ALIVE(Creature)) {
            C3DVector facing_dir = phero->GetPos() - Creature->GetPos();
            facing_dir.AccurateNormalise();
            Creature->GetPhysicsTC()->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), facing_dir));
        }
    }
}
