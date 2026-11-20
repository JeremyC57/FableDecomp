// CTCBoss::IncrementStrikesSinceHitHero() -- count one more strike since the hero was last hit, while the boss is counting them
// Leaf: ?IncrementStrikesSinceHitHero@CTCBoss@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007AED30, oracle length 11 bytes.
// Retail bytes:  mov al, byte ptr [ecx + 0x20] ; test al, al ; je 0x7aed3a ; inc dword ptr [ecx + 0x14] ; ret
// Naming (NF191, 2026-11-20, q12153): Ego_r.pdb 0x007B0680 + FableWin.pdb 0x026D1330 ?IncrementStrikesSinceHitHero@CTCBoss@@QAEXXZ
//   (Ego_d.pdb procnames too); BCountStrikes +0x20 gates StrikesSinceHit +0x14 (engine/CTCBoss.h members), between IncrementBossPhase
//   0x007AED20 (Ego_r 0x007B0670) and GetStrikesSinceHitHero 0x007AED40 in Ego_r order. Was the shape-generated stand-in
//   CTCBoss::IncrementBossPhase on a row-local layout (the same leaf the landed 0x007AED20 owns); now on the shared engine/CTCBoss.h
//   (opt-in FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CTCBoss.h"

void CTCBoss::IncrementStrikesSinceHitHero()
{
    if (BCountStrikes)
        ++StrikesSinceHit;
}
