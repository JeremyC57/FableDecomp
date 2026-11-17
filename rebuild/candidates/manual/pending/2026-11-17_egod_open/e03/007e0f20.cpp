// CTCAICreatureWillPowerIndicator::FrameUpdate() -- while the creature is in melee and not yet powered up: when the timer is within a second and no emitter is alive, attach a POWER_UP_INDICATOR particle emitter to the creature's weapon; then count the timer down and power up when it runs out
// Grade: DIFFER(531v537) -- shared-header port with Ego_d locals; residue: retail loads the component-presence word into ecx before `test ecx,10000h` (0x007E0F2A, ours `test dword [eax+24h]`) and packs the GetTC key temporary in fx_index's frame word (0x007E0F4A [esp+18h]); the rest is frame-word renumbering
// Leaf: ?FrameUpdate@CTCAICreatureWillPowerIndicator@@UAEXXZ
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTCDRIVER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_ISINMELEE
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCCombat.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CMainGameComponent.h"

class CTCAICreatureWillPowerIndicator : public CTCBase {
public:
    virtual void FrameUpdate();
private:
    bool PoweredUp;                                                      // +0x0c
    long Timer;                                                          // +0x10
    long ResetCount;                                                     // +0x14
    bool ShowEffect;                                                     // +0x18
    CIntelligentPointer<CThing> PEmitter;                                // +0x19
};

void CTCAICreatureWillPowerIndicator::FrameUpdate()
{
    CTCCombat* ptc_combat = 0;
    if (!GetParentThing().GetTC(&ptc_combat) || !ptc_combat->IsInMelee() || PoweredUp)
        return;
    if ((float)Timer <= (float)CMainGameComponent::GetConstantFPS() && !IS_THING_ALIVE(PEmitter) && ShowEffect) {
        CDefPointer<const CSpecialEffectsDef> pspecial_fx;
        long fx_index = 0;
        if (GetParentThing().PeekBaseDef().GetSubDef(&pspecial_fx) && pspecial_fx->GetEffectFromString("POWER_UP_INDICATOR", fx_index)) {
            CTCCarrying* ptc_carrying;
            C3DVector weapon_pos = GetParentThing().GetPos();
            if (GetParentThing().GetTC(&ptc_carrying)) {
                CThing* pweapon = ptc_carrying->GetThingInPrimarySlot();
                if (IS_THING_ALIVE(pweapon)) {
                    CThing* pemitter = CTCDParticleEmitter::Create(fx_index, weapon_pos, false);
                    if (pemitter) {
                        PEmitter = CIntelligentPointer<CThing>(pemitter);
                        CTCDParticleEmitter* ptcd_particle_emitter = 0;
                        pemitter->GetTCDriver(&ptcd_particle_emitter);
                        if (pweapon)
                            ptcd_particle_emitter->AttachToThing(*pweapon, 0xc, "weapon_pos_a", 0, 0.0f);
                    }
                }
            }
        }
    }
    if (Timer <= 0)
        PoweredUp = true;
    else
        --Timer;
}
