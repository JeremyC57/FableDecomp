// CTCTargetingPlayer::GetPBestThingForMeleeAttack() const -- the targeted thing, if it is still a valid melee target for the
// Grade: DIFFER(150v152) -- only the AreThingsEnemies argument setup: retail `mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx` (the parent loaded into the dying this register, then copied to ecx), ours `mov ecx,[ebx+4] ; mov edx,esi`. Tried (Q28): parent ref locals (block / inner), *ParentThing, bool result local, inline two-argument wrapper. Tried (Q31): parent ref after the targetable test / pointer local after the smashable test / ref at the top for PeekPlayer only (all forwarded straight into ecx); one ref for both uses (loaded once into ebx); ref at the top of the else block (loaded before the TC lookup); p_thing = 0 single exit (160); one && condition; smashable || enemies; bool is_valid_target; CThing& thing = *p_thing; static_cast second arg; const_cast non-const GetParentThing at either use; default GetTC; /G5 /G7 /GB, optimize("s"); toys (ref/bool locals, pointer-param wrapper, T::IsEnemyOf member, ternary) all load into ecx. Not the PDB IsThingAnEnemy (its retail copy 0x00802380 passes (thing, parent)).
// player: while Z-targeting it must be the locked target; otherwise it must be Z-targetable (CTCTargeted flag 4), smashable, or
// an enemy of the player's thing. A missing or dead targeted thing is returned as it is.
// Leaf: ?GetPBestThingForMeleeAttack@CTCTargetingPlayer@@UBEPAVCThing@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008031E0, 152 bytes; CTCTargetingPlayer vtable 0x0124280C slot 28 (its only reference).
// Identity: Ego_r.pdb / FableWin.pdb ?GetPBestThingForMeleeAttack@CTCTargetingPlayer@@UBEPAVCThing@@XZ; slot 28 is that virtual
//   in the CTCTargetingBase declaration order (engine/CTCTargetingBase.h; the landed 0x00802610 maps slots 28..39).
// Callees: virtual slot 33 GetPTargetedThing (+0x84), CThing::PeekPlayer 0x004C7A10, virtual slot 36 GetPTargetLockedThing
//   (+0x90), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (the expanded GetTC<CTCTargeted>), CTCEnemy::AreThingsEnemies
//   0x0076D2F0 (__fastcall).
// Layout: CThing Dead +0x91 bit 0, UsingComponents +0x20 (TCI_TARGETED 9, TCI_SMASHABLE 78), Components +0x44; CPlayer ZTargeting
//   +0x20A; CTCTargeted TargetingFlags +0x0C.
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_VSLOTS3
#define FABLE_RETAIL_CTCTARGETINGPLAYER_MELEE
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTCENEMY_QUERIES
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CPlayer.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCEnemy.h"

CThing* CTCTargetingPlayer::GetPBestThingForMeleeAttack() const
{
    CThing* p_thing = GetPTargetedThing();
    if (p_thing && !p_thing->IsDead()) {
        if (GetParentThing().PeekPlayer().IsZTargeting()) {
            if (GetPTargetLockedThing() != p_thing)
                return 0;
        } else {
            CTCTargeted* targeted;
            if (p_thing->GetTC(&targeted) && targeted->IsZTargetable())
                return p_thing;
            if (p_thing->HasTC(TCI_SMASHABLE))
                return p_thing;
            if (!CTCEnemy::AreThingsEnemies(GetParentThing(), *p_thing))
                return 0;
        }
    }
    return p_thing;
}
