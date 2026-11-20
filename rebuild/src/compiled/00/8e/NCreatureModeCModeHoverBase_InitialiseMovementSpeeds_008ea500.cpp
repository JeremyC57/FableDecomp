// NCreatureMode::CModeHoverBase::InitialiseMovementSpeeds() -- measure the mode's FORWARD_SLOW/_MEDIUM/_FAST animations (prefixed
//   by the mode's animation prefix) and scale each hover state's speed by the creature def's slow/medium/fast hover speed over the
//   matching animation speed (1 when either is below 1e-4); a state's tier comes from its animation-name suffix (_SLOW, _MEDIUM,
//   _FAST, else 0) and the scaled speed is written to both MovementSpeed and Area.RadHigh
// Leaf: ?InitialiseMovementSpeeds@CModeHoverBase@NCreatureMode@@AAEXXZ
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EA500, 923 bytes (`ret`).
// Callees: CModeBase::GetWorld 0x0073A680 (world +0x3c passed through), own virtual slot 12 (+0x30, CCharString by value),
//   operator+(const CCharString&, const char*) 0x0099F600 (__fastcall), NCreatureMode::GetMovementSpeedFromAnimation(const CCharString&, ...) 0x00861C90
//   (__fastcall name, creature, the world mesh bank), CThingCreatureBase::GetDefHover{Slow,Medium,Fast}Speed 0x006620E0 / 0x006620F0 /
//   0x00662100 (def +0xe0 -> +0xa4/+0xa8/+0xac; Ego_r + FableWin; manifest "CTCPhysicsBase::GetAirResistance" is a propagated label), CCharString 0x0099EBF0 / ~ 0x0099EAE0.
// Data: "FORWARD_SLOW" 0x01287CFC, "FORWARD_MEDIUM" 0x01287CEC, "FORWARD_FAST" 0x01287CDC, "_SLOW" 0x01287CD4, "_MEDIUM" 0x01287CCC,
//   "_FAST" 0x01287CC4, epsilon 1e-4f 0x0129BA3C, the empty string 0x0129AAF4.
// Layout: Ego_r.pdb CModeHoverBase: HoverStates vector +0x1c (12-byte retail vector), CHoverState 0x18 (AnimName +0, Area +4
//   {ThetaLow, ThetaHigh, RadHigh}, MovementType +0x10, MovementSpeed +0x14).
// Shape: the inline CCharString::EndsWith (fable_string.h FABLE_RETAIL_CCHARSTRING_ENDSWITH);
//   the speeds are written as one chained assignment (fst / fstp).
// Naming (NF128, 2026-11-20; q6453 / q6455): the local CModeBase / CThingCreatureBase / CWorld / CCharString stand-ins replaced by the shared
//   headers: CModeHoverBase (no shared header) is the landed sibling 0x008EACB0's class over the shared nested CTCCreatureModeManager::CModeBase
//   (VIRTUALS: GetWorld), engine/CThingCreatureBase.h (overlay opt-in FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS: Ego_r + FableWin
//   GetDefHover{Slow,Medium,Fast}Speed), engine/CWorld.h (FLAT_GETMESHBANK), fable_string.h (ENDSWITH); 0x00861C90 takes its Ego_r / FableWin
//   NCreatureMode::GetMovementSpeedFromAnimation spelling. Own leaf QAE -> AAE (private in both PDBs). RELOCATION_MATCH in one form.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_ENDSWITH
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETDEFHOVERSPEEDS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_CWORLD_FLAT_GETMESHBANK
#include <string.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CIntelligentPointer.h"

CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600
extern const float DELTA_NOTIONAL_ZERO;                                                // 0x0129BA3C (1e-4f)
class CMeshDataBank;
enum EAnimUpdateMode;

namespace NCreatureMode {
float __fastcall GetMovementSpeedFromAnimation(const CCharString& anim_name, const CThingCreatureBase& creature, const CMeshDataBank& bank);   // 0x00861C90

class CModeHoverBase : public CTCCreatureModeManager::CModeBase {
public:
    class CRadialArea {
    public:
        float ThetaLow;                                                                // +0x00
        float ThetaHigh;                                                               // +0x04
        float RadHigh;                                                                 // +0x08
    };
    class CHoverState {
    public:
        CCharString   AnimName;                                                        // +0x00
        CRadialArea   Area;                                                            // +0x04
        EMovementType MovementType;                                                    // +0x10
        float         MovementSpeed;                                                   // +0x14
    };
    virtual void OnActivate();
protected:
    virtual CCharString GetAnimPrefix() const = 0;                                     // slot 12 (+0x30)
    virtual EAnimUpdateMode GetAnimUpdateType() const = 0;                             // slot 13 (+0x34)
    virtual CCharString GetEffectName() const;                                         // slot 14 (+0x38)
    virtual CCharString GetEffectHelperPointName() const;                              // slot 15 (+0x3c)
private:
    void InitialiseMovementSpeeds();                                                   // 0x008EA500
    std::vector<CHoverState> HoverStates;                                              // +0x1c
    CCharString CurrentName;                                                           // +0x28
    const CHoverState* PCurrentState;                                                  // +0x2c
    long CurrentMovementType;                                                          // +0x30
    std::list<void*> AnimQueue;                                                        // +0x34 list<CCountedPointer<CHoverAnim> >
    CIntelligentPointer<CThing> PParticleEffect;                                       // +0x38
};

void CModeHoverBase::InitialiseMovementSpeeds()
{
    CThingCreatureBase& creature = (CThingCreatureBase&)ParentThing;
    const CMeshDataBank& bank = *GetWorld().GetMeshBank();
    CCharString prefix = GetAnimPrefix();
    float slowAnimSpeed = GetMovementSpeedFromAnimation(prefix + "FORWARD_SLOW", creature, bank);
    float mediumAnimSpeed = GetMovementSpeedFromAnimation(prefix + "FORWARD_MEDIUM", creature, bank);
    float fastAnimSpeed = GetMovementSpeedFromAnimation(prefix + "FORWARD_FAST", creature, bank);
    float slowSpeed = creature.GetDefHoverSlowSpeed();
    float mediumSpeed = creature.GetDefHoverMediumSpeed();
    float fastSpeed = creature.GetDefHoverFastSpeed();
    float slowScale, mediumScale, fastScale;
    if (slowAnimSpeed < DELTA_NOTIONAL_ZERO || slowSpeed < DELTA_NOTIONAL_ZERO)
        slowScale = 1.0f;
    else
        slowScale = slowSpeed / slowAnimSpeed;
    if (mediumAnimSpeed < DELTA_NOTIONAL_ZERO || mediumSpeed < DELTA_NOTIONAL_ZERO)
        mediumScale = 1.0f;
    else
        mediumScale = mediumSpeed / mediumAnimSpeed;
    if (fastAnimSpeed < DELTA_NOTIONAL_ZERO || fastSpeed < DELTA_NOTIONAL_ZERO)
        fastScale = 1.0f;
    else
        fastScale = fastSpeed / fastAnimSpeed;
    for (CHoverState* state = HoverStates.begin(); state != HoverStates.end(); ++state) {
        float scale = 0.0f;
        if (state->AnimName.EndsWith("_SLOW"))
            scale = slowScale;
        else if (state->AnimName.EndsWith("_MEDIUM"))
            scale = mediumScale;
        else if (state->AnimName.EndsWith("_FAST"))
            scale = fastScale;
        state->Area.RadHigh = state->MovementSpeed = GetMovementSpeedFromAnimation(state->AnimName, creature, bank) * scale;
    }
}
}
