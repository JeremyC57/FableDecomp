// CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude(float magnitude) const -- classify the stick magnitude into a movement band (0 stationary, 1 slow, 2 walking, 3 running, 4 jogging) from the player movement def's joystick bands, honouring the hero-run option, sneaking and the special-movement run flag; stationary whenever the hero cannot run at all
// Leaf: ?GetMovementBandTypeFromJoytickMagnitude@CThingPlayerCreature@@IBE?AW4EMovementBandType@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006AB430, 408 bytes, thiscall, `ret 4`.
// Name: Ego_r.pdb / FableWin.pdb ?GetMovementBandTypeFromJoytickMagnitude@CThingPlayerCreature@@IBE?AW4EMovementBandType@@M@Z
//   (protected const; "Joytick" is the PDB spelling).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound const 0x004365B0 (the expanded PeekTC lookups of TCI_STEALTH 54 and
//   TCI_HERO_SPECIAL_MOVEMENT 223), CThing::GetPos 0x004C73D0, the virtual GetMaxRunningSpeed(const C3DVector&) const (slot 81, +0x144).
// Data: g_bHeroRunOption 0x013756EA (landed spelling, as CTCHeroSpecialMovement::FrameUpdate 0x007E0750); 0.01f 0x01230A5C, 0.97f 0x0125D1DC.
// Shape: the band values are written as numbers (no EMovementBandType enumerator survives in the name sets); the component flags
//   are read through the out-parameter PeekTC form with the inline CTCStealth::IsSneaking / CTCHeroSpecialMovement::IsRunning (the
//   component pointer is reused in eax and its flag loaded into cl, 0x006AB464 / 0x006AB504, as the landed GetCurrentMaxMovementSpeed
//   0x006AB5D0); the non-option thresholds are an else-if chain ending in `else return 3`, so VC7.1 keeps the FIRST copy of each
//   merged return in place (the fabs / run-option exits fall through, 0x006AB495 / 0x006AB4C9) as retail (playbook: duplicate returns).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTBAND
#define FABLE_RETAIL_CTCSTEALTH_SNEAKTOGGLE
#define FABLE_RETAIL_CTCHEROSPECIALMOVEMENT_ISRUNNING
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerMovementDef.h"
#include "engine/CTCStealth.h"
#include "engine/CTCHeroSpecialMovement.h"

extern bool g_bHeroRunOption;                           // 0x013756EA

EMovementBandType CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude(float magnitude) const
{
    const CTCStealth* stealth;
    bool sneaking = PeekTC(TCI_STEALTH, &stealth) && stealth->IsSneaking();
    if (fabs(GetMaxRunningSpeed(GetPos())) < 0.01f)
        return (EMovementBandType)0;
    if (g_bHeroRunOption) {
        if (magnitude < PPlayerMovementDef->JoystickMovementStationaryBand)
            return (EMovementBandType)0;
        if (magnitude < PPlayerMovementDef->JoystickMovementWalkingBand)
            return (EMovementBandType)2;
        const CTCHeroSpecialMovement* special;
        if (PeekTC(TCI_HERO_SPECIAL_MOVEMENT, &special) && special->IsRunning())
            return (EMovementBandType)3;
        return (EMovementBandType)4;
    }
    const CPlayerMovementDef* def = PPlayerMovementDef.Get();
    if (magnitude < def->JoystickMovementSlowBand && !sneaking)
        return (EMovementBandType)1;
    else if (magnitude < def->JoystickMovementWalkingBand)
        return (EMovementBandType)2;
    else if (magnitude < 0.97f && sneaking)
        return (EMovementBandType)2;
    else if (magnitude < def->JoystickMovementJoggingBand)
        return (EMovementBandType)4;
    else if (magnitude < def->JoystickMovementStationaryBand)
        return (EMovementBandType)0;
    else if (magnitude < def->JoystickMovementSlowBand)
        return (EMovementBandType)1;
    else if (magnitude < def->JoystickMovementWalkingBand)
        return (EMovementBandType)2;
    else if (magnitude < def->JoystickMovementJoggingBand)
        return (EMovementBandType)4;
    else
        return (EMovementBandType)3;
}