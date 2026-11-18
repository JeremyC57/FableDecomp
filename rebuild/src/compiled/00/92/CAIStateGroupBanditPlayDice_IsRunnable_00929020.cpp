// CAIStateGroup_BanditPlayDice::IsRunnable() -- choose a free marker and check whether the bandit can join the dice game.
// Leaf: ?IsRunnable@CAIStateGroup_BanditPlayDice@@MAE_NXZ
// Retail: 0x00929020, 507 bytes, protected virtual thiscall.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCEXCLUSIVEUSE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_RUNNABLE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingMarker.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCExclusiveUse.h"
#include "engine/CWorld.h"
#include "engine/CAIStateGroup_BanditPlayDice.h"

bool CAIStateGroup_BanditPlayDice::IsRunnable()
{
    if (CWorld::GetFrame() < FrameToInitiate)
        return false;
    DiceGame = GetDiceGame();
    if (!DiceGame)
        return false;
    bool someone_already_playing = false;
    CThing* best = 0;
    float best_dist = 0.0f;
    long marker_type = GetMarkerType();
    CTCVillageMember* village_member;
    if (!PBrain->Creature->GetTC(TCI_VILLAGE_MEMBER, &village_member))
        return false;
    const std::list<CIntelligentPointer<CThingMarker> >& markers = village_member->GetPVillage()->VillageMarkers;
    for (std::list<CIntelligentPointer<CThingMarker> >::const_iterator i = markers.begin(); i != markers.end(); ++i) {
        if (i->Get() && !(*i)->IsDead() && (*i)->GetDefGlobalIndex() == marker_type) {
            CTCExclusiveUse* exclusive_use;
            (*i)->GetTC(&exclusive_use);
            if (exclusive_use->GetUser()) {
                someone_already_playing = true;
            } else {
                CThing* creature = PBrain->Creature;
                float dist = ((*i)->GetPos() - creature->GetPos()).GetSquaredMagnitude();
                if (!best || dist < best_dist) {
                    best = i->Get();
                    best_dist = dist;
                    DiceGamePosition = exclusive_use;
                }
            }
        }
    }
    if (best && (someone_already_playing || CWorld::GetFrame() > FrameToJoin))
        return true;
    return false;
}