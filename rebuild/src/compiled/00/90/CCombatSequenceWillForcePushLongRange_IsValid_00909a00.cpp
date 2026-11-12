// CCombatSequence_WillForcePushLongRange::IsValid() const -- valid unless the creature's will-power indicator (TC 0xCE) is not
// powered up. For a powered-up one the flat direction from the game camera to the creature is normalised and dotted with the
// render camera's flat view direction; the test on that dot (`< 0`) returns true, and so does falling out of it.
// Leaf: ?IsValid@CCombatSequence_WillForcePushLongRange@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00909A00, length 336 bytes, plain `ret` (virtual, const).
// Callees: CVectorMap lower bound 0x004365B0 (the expanded PeekTC), CThing::GetPos 0x004C73D0, the game camera's virtuals
// PeekCurrentPos (slot 11, +0x2C) and GetRenderCamera(float) (slot 1, +0x04) reached through CMainGameComponent::PMainGameComponent
// 0x013B86A0 -> PWorld +0x24 -> PCurrentGameCamera +0x34. Globals: GFastOneOverSqrtTable 0x013CE558, 3.0f 0x0122DED4, 0.5f
// 0x0122F59C, 0.0f 0x0122DEDC.
// Shape: `if (PeekTC) { if (IsPoweredUp()) { ...; if (dot < 0) return true; } else return false; } return true;`. The inner
// `return true` and the common one are separate return statements, so the compare survives VC7.1's optimiser and only the
// branch disappears when the two identical return blocks are merged afterwards: retail keeps the whole dot product and
// `fcomp ; fstp st(0) ; fnstsw ax` with no test. (`if (c) return true; return true;` side by side is merged earlier and drops the
// dot; an empty `if` drops the normalise too.) The view direction is copied out of the GetRenderCamera temporary (read through
// the returned pointer: `fld [eax+18h] ; add eax,18h ; fld [eax+4]`), then flattened; its Y stays on the FPU stack until the
// compare. The creature is held in a reference local (`mov edi,[ecx+1Ch]` once).
#include <stddef.h>
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCOMBATSEQUENCE_WILLFORCEPUSHLONGRANGE
#include "engine/C3DVector.h"
#include "engine/CVectorMap.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
class CTCAICreatureWillPowerIndicator {                    // no shared header: Ego_r.pdb PoweredUp +0x0C
public:
    bool IsPoweredUp() const { return PoweredUp; }         // FableWin.pdb ?IsPoweredUp@CTCAICreatureWillPowerIndicator@@QBE_NXZ (inline)
private:
    unsigned char _pad_0x00[0xc];
    bool PoweredUp;                                        // +0x0C
};
class CCombatAction;
class CCombatSequenceBase {                                // no shared header: Ego_r.pdb members, retail 0x28 bytes
public:
    virtual ~CCombatSequenceBase();
    virtual bool IsValid() const;
    bool                Running;                           // +0x04
    long                NoAvailableActions;                // +0x08
    long                CurrentActionIndex;                // +0x0C
    unsigned char       CombatActions_[0xc];               // +0x10 std::vector<CCombatAction>
    CThingCreatureBase* Creature;                          // +0x1C
    CThingCreatureBase* Target;                            // +0x20
    bool                CreateCalled;                      // +0x24
};
#include "engine/CCombatSequence_WillForcePushLongRange.h"
bool CCombatSequence_WillForcePushLongRange::IsValid() const
{
    const CThingCreatureBase& creature = *Creature;
    const CTCAICreatureWillPowerIndicator* indicator;
    if (creature.PeekTC(TCI_AICREATURE_WILL_POWER_INDICATOR, &indicator)) {
        if (indicator->IsPoweredUp()) {
            C3DVector to_creature = creature.GetPos() - CMainGameComponent::Get()->PeekWorld().PeekGameCamera()->PeekCurrentPos();
            to_creature.Z = 0.0f;
            to_creature.Normalise();
            C3DVector view = CMainGameComponent::Get()->PeekWorld().PeekGameCamera()->GetRenderCamera(1.0f).PeekViewDirection();
            view.Z = 0.0f;
            if (view.Dot(to_creature) < 0.0f)
                return true;
        } else
            return false;
    }
    return true;
}