// CTCStealth::InitialiseSneakSpeed() -- derive the maximum sneaking speed from the creature's crouching walk animation: its scaled movement length over its duration in frames (0.1 when either is not positive)
// Leaf: ?InitialiseSneakSpeed@CTCStealth@@AAEXXZ
// Name: Ego_r.pdb / FableWin.pdb ?InitialiseSneakSpeed@CTCStealth@@AAEXXZ (private; engine/CTCStealth.h declares it under private:).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00747330, 279 bytes (plain `ret`).
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("CROUCH" @ 0x01267B28, "ST_WALK" @ 0x0125A990), CCharString()
//   @ 0x0099E4B0, CCharString::operator= @ 0x0099EFB0, CRC Calc @ 0x004014A0 (`__fastcall`, "" @ 0x0129AAF4 for an empty
//   name), ?GetAnimFromName@CThingCreatureBase@@QBEABVCAnimationEntry@@ABVCCharString@@ABVCAnimFilterBase@@J@Z @ 0x00662B10,
//   ~CCharString @ 0x0099EAE0 (x3), ?GetAnimMovementDistance@CThingCreatureBase@@QBEMJ@Z @ 0x006627A0,
//   ?GetAnimDuration@CThingCreatureBase@@QBEMJ@Z @ 0x006627F0, ?GetFullScale@CTCGraphicAppearance@NGraphicAppearance@@QBEMXZ
//   @ 0x004BBFF0 (all Ego_r.pdb / FableWin.pdb spellings). CMainGameComponent::GetConstantFPS() reads 0x01375550; 0.0f @ 0x0122DEDC.
// Layout: Ego_r.pdb CTCStealth MaxSneakingSpeed +0x18; CThing PTCGraphicAppearance +0x64 (inline GetTCGraphicAppearance);
//   CAnimFilterOfGroup {Group +4, GroupCRC +8} with its inline constructor (vtable 0x01267568, base 0x0122FFCC);
//   CAnimationEntry BankIndex +0.
// Shape: the names and the group filter live in a block that closes after the anim lookup; the scaled length and the
//   duration are float locals compared with `> 0.0f`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMDURATION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMMOVEMENTDISTANCE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCBase.h"
#include "engine/CTCStealth.h"
#include "engine/CAnimFilterOfGroup.h"
#include "engine/CAnimationEntry.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMainGameComponent.h"

void CTCStealth::InitialiseSneakSpeed()
{
    CThingCreatureBase& creature = static_cast<CThingCreatureBase&>(GetParentThing());
    long anim;
    {
        CCharString group("CROUCH");
        CCharString name("ST_WALK");
        CAnimFilterOfGroup filter(group);
        anim = creature.GetAnimFromName(name, filter, 0).BankIndex;
    }
    float length = creature.GetAnimMovementDistance(anim) * creature.GetTCGraphicAppearance()->GetFullScale();
    float duration = creature.GetAnimDuration(anim);
    if (length > 0.0f && duration > 0.0f)
        MaxSneakingSpeed = length / (duration * CMainGameComponent::GetConstantFPS());
    else
        MaxSneakingSpeed = 0.1f;
}
