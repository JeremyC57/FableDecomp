// CTCKraken::FrameUpdate() -- per frame (not in the editor): count down and kill the beam weapon effect, drop dead tentacles, follow the boss phase, re-find the centre marker and the hero target, run the phase / mode updates, start the death phase when the Kraken's health is gone, and keep the Kraken at its centre marker plus the def's height offset
// Grade: DIFFER(704v704, 22 hunks) -- control flow and calls match; frame 0x18 v 0x14: the GetTC<CTCTargeted> out-pointer keeps its own 4-byte slot (retail none)
// Leaf: ?FrameUpdate@CTCKraken@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007FC9D0, 704 bytes + jump tables 0x007FCC6C / 0x007FCC7C, plain `ret`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_COMPONENT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REMOVE_DECL
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#define FABLE_RETAIL_CTCBOSS_EFFECTS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingPhysical.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CTCBase.h"
#include "engine/CTCKraken.h"
#include "engine/CTCBoss.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CMainGameComponent.h"

void CTCKraken::UpdateUnderwater()
{
}

void CTCKraken::UpdateTentacles()
{
}

void CTCKraken::UpdateTentaclesAndHead()
{
}

void CTCKraken::FrameUpdate()
{
    if (PeekComponent().IsEditorActive())
        return;
    if (KillBeamCounter > 0 && --KillBeamCounter == 0 && PBeamWeaponEffect.Get() && !PBeamWeaponEffect->IsDead()) {
        PBeamWeaponEffect->Kill(true);
        PBeamWeaponEffect = 0;
    }
    Tentacles.remove(0);
    if (Phase != PTCBoss->GetBossPhase())
        SetPhase((EKrakenBossPhase)PTCBoss->GetBossPhase());
    if (!PCentreMarker.Get() || PCentreMarker->IsDead())
        GetCentreMarker();
    if (!PTarget.Get() || PTarget->IsDead())
        PTarget = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    switch (Phase) {
    case 1:
    case 2:
        switch (Mode) {
        case 1:
            UpdateTentaclesOnly();
            break;
        case 2:
            AttackingTentacleIterator = Tentacles.end();
            break;
        case 3:
            DoHeadTracking();
            break;
        case 5:
            DoHeadTracking();
        case 4:
            if (DelayBeforeBeam == 0)
                UpdateBeamExplosion();
            else
                --DelayBeforeBeam;
            break;
        }
    case 0:
        UpdateUnderwater();
    default:
        if (Phase == 1) {
            if (PTCBoss->IsReadyForPhaseChange())
                ReturnHeadToWater = true;
        } else if (static_cast<CThingPhysical&>(GetParentThing()).GetHealth() < 1.0f) {
            CTCTargeted* targeted;
            if (GetParentThing().GetTC(&targeted))
                targeted->SetTargetable(false);
            CreateHeadEffect("ON_DIE");
            SetPhase((EKrakenBossPhase)3);
        }
        break;
    case 3:
        break;
    }
    C3DVector pos = PCentreMarker->GetPos();
    pos.Z += GetKrakenDef().HeightOffset;
    GetParentThing().GetPhysicsTC()->SetPosition(pos);
}
