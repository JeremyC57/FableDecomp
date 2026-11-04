// CTCTargetingPlayer::GetPBestThingForMeleeAttack() const -- the targeted thing when it is still a melee target for the player (while Z-targeting only the locked target; otherwise a Z-targetable, smashable or enemy thing), else null; a missing or dead target is returned as is
// Grade: DIFFER(ours v retail) -- DIFFER(150v152) (try i): one instruction: the parent thing for CTCEnemy::AreThingsEnemies loads straight into ecx (mov ecx,[ebx+4]); retail loads it into the dead this register and copies (mov ebx,[ebx+4] ; mov edx,esi ; mov ecx,ebx). Tried 16 spellings (inline IsThingAnEnemy, parent reference locals, *ParentThing, bool local, if/else forms).
// Leaf: ?GetPBestThingForMeleeAttack@CTCTargetingPlayer@@UBEPAVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x008031E0, 152 bytes; slot 28 of the CTCTargetingPlayer vtable 0x0124280C (slot list in the landed
//   CTCTargetingPlayer::GetPTargetedThing 0x00802610); Ego_r.pdb ?GetPBestThingForMeleeAttack@CTCTargetingPlayer@@UBEPAVCThing@@XZ.
// Callees: virtual GetPTargetedThing (slot 33, +0x84) and GetPTargetLockedThing (slot 36, +0x90), CThing::PeekPlayer() const 0x004C7A10,
//   LowerBound 0x0040F020 (CThing::PeekTC<const CTCTargeted> through Components.Find), static __fastcall
//   CTCEnemy::AreThingsEnemies(const CThing&, const CThing&) 0x0076D2F0.
// Layout: engine/CTCTargetingBase.h (vtable slots 28-39), engine/CTCTargeted.h retail branch (TargetingFlags +0x0C, IsZTargetable),
//   engine/CPlayer.h retail opt-in (IsZTargeting, +0x20A); TCI_SMASHABLE (78) is word +0x28 bit 0x4000.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_PLAYER
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CPlayer.h"

class CTCEnemy : public CTCBase {                                   // no shared header
public:
    static bool __fastcall AreThingsEnemies(const CThing& a, const CThing& b);    // 0x0076D2F0
};

class CTCTargetingPlayer : public CTCTargetingBase {                // vtable 0x0124280C
public:
    virtual CThing* GetPBestThingForMeleeAttack() const;
};

CThing* CTCTargetingPlayer::GetPBestThingForMeleeAttack() const
{
    CThing* target = GetPTargetedThing();
    if (target && !target->IsDead()) {
        if (GetParentThing().PeekPlayer().IsZTargeting()) {
            if (GetPTargetLockedThing() != target)
                return 0;
        } else {
            const CTCTargeted* targeted;
            if (target->PeekTC(&targeted) && targeted->IsZTargetable())
                return target;
            if (target->HasTC(TCI_SMASHABLE))
                return target;
            if (!CTCEnemy::AreThingsEnemies(GetParentThing(), *target))
                return 0;
        }
    }
    return target;
}
