// CTCTargetingPlayer::GetPBestThingForMeleeAttack() const -- the targeted thing, if it is still a valid melee target for the
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
// Shape: retail loads the parent into the register that held `this` and copies it to ecx for AreThingsEnemies
//   (`mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx`), which VC7.1 only does for a VARIABLE with a second use: every single-use
//   spelling loads straight into ecx (`mov ecx,[ebx+4]`, 150 bytes). No second use survives in the bytes, so it is one that the
//   compiler deletes after register allocation: a compare of the target with the parent whose two outcomes are the same statement
//   (`return p_thing`; the branch goes when the two arms are merged, and the compare with it). This is the only construct found that
//   reproduces the bytes (an empty-bodied `if`, the compare on the `return 0` side, a null test, a dead store and ~70 other
//   spellings do not: NOTES.md of chunk F3); Ego_r's build of this function is 34 bytes longer, so the arm most likely held code
//   that the TLC source lost. The exact wording of the dead test is therefore an inference; its presence is what the bytes show.
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
            const CThing& parent = GetParentThing();
            if (!CTCEnemy::AreThingsEnemies(parent, *p_thing))
                return 0;
            if (p_thing == &parent)
                return p_thing;
        }
    }
    return p_thing;
}