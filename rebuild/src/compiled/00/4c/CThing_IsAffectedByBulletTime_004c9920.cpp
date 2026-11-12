// CThing::IsAffectedByBulletTime() const -- whether bullet time slows this thing.
// Leaf: ?IsAffectedByBulletTime@CThing@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) 0x004C9920, 598 bytes, __fastcall, plain `ret`, /Oy (no frame pointer).
// In a pause mode (CThingManager +0x54 NonScriptedEntitesPauseMode / +0x55 AllEntitesPauseMode) script supervision and the
// cutscene flags decide first, then type 2, doors, bobbing, region exits (never affected), a fading thing (affected unless fading),
// a carried thing (its carrier decides) and a particle emitter (affected unless AlwaysUpdate); otherwise script control decides.
// Outside pause modes: region exits never; while the main player's creature is alive and has TCI_BULLET_TIME (and this thing has
// no IsAffectedByBulletTimeOverride) never; type 2 never; a carried thing / an attached particle emitter defer to their carrier /
// attached thing. The deferrals are self tail calls that VC7.1 turns into the jump back to the top (0x004C9930).
// Callees: CPlayerManager::GetMainPlayer() const 0x00449960, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//          CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0, CTCCarryable::GetPCarriedByThing const 0x00705170,
//          CThing::IsUnderScriptControl const 0x004C8C90, CTCFadeOutAndIn::IsFading const 0x0049D310.
// Inline in retail (FableWin.pdb copies placed by Thing.cpp): CThingManager::PeekPlayerManager, CPlayer::GetPPlayerCharacter,
//          CTCDParticleEmitter::IsAlwaysUpdating / PeekPThingAttachedTo, CThing::PeekPCurrentThingManager (0x013B8A1C), PeekTC, HasTC.
// CTCFadeOutAndIn has no shared header: the minimal model the landed IsFading row (0x0049D310) uses, over engine/CTCBase.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CTHINGMANAGER_ALLPAUSE
#define FABLE_RETAIL_CTHINGMANAGER_PEEKPLAYERMANAGER
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_ATTACHED_INLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CVectorMap.h"

class CTCFadeOutAndIn : public CTCBase {
public:
    static ETCInterfaceType FABLE_FASTCALL StatGetInterfaceType() { return TCI_FADE_OUT_AND_IN; }
    bool IsFading() const;                                       // 0x0049D310
};

bool CThing::IsAffectedByBulletTime() const
{
    const CThingManager* manager = PeekPCurrentThingManager();
    if (manager->IsNonScriptedEntitesPauseMode() || manager->IsAllEntitesPauseMode()) {
        if (manager->IsNonScriptedEntitesPauseMode()) {
            if (NotPausedDuringCutscenes && !IsUnderScriptControl())
                return false;
            if (PausedDuringCutscenes && IsUnderScriptControl())
                return true;
        }
        if (GetType() == 2)
            return false;
        if (HasTC(TCI_DOOR))
            return false;
        if (HasTC(TCI_BOB))
            return false;
        if (HasTC(TCI_REGION_EXIT))
            return false;
        const CTCFadeOutAndIn* fade;
        if (PeekTC(TCI_FADE_OUT_AND_IN, &fade))
            return !fade->IsFading();
        const CTCCarryable* carryable;
        if (PeekTC(TCI_CARRYABLE, &carryable)) {
            const CThing* carrier = carryable->GetPCarriedByThing();
            if (carrier)
                return carrier->IsAffectedByBulletTime();
        }
        const CTCDParticleEmitter* emitter;
        if (PeekTC(TCI_PARTICLE_EMITTER, &emitter))
            return !emitter->IsAlwaysUpdating();
        if (PeekPCurrentThingManager()->IsNonScriptedEntitesPauseMode())
            return !IsUnderScriptControl();
        return true;
    }
    if (HasTC(TCI_REGION_EXIT))
        return false;
    const CThingPlayerCreature* player = manager->PeekPlayerManager().GetMainPlayer().GetPPlayerCharacter();
    if (player && !player->IsDead() && !IsAffectedByBulletTimeOverride && player->HasTC(TCI_BULLET_TIME))
        return false;
    if (GetType() == 2)
        return false;
    const CTCCarryable* carryable;
    if (PeekTC(TCI_CARRYABLE, &carryable)) {
        const CThing* carrier = carryable->GetPCarriedByThing();
        if (carrier)
            return carrier->IsAffectedByBulletTime();
    }
    const CTCDParticleEmitter* emitter;
    if (PeekTC(TCI_PARTICLE_EMITTER, &emitter)) {
        if (emitter->PeekPThingAttachedTo() && emitter->PeekPThingAttachedTo() != this)
            return emitter->PeekPThingAttachedTo()->IsAffectedByBulletTime();
    }
    return true;
}