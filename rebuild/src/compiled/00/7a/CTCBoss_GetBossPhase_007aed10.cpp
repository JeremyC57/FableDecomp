// CTCBoss::GetBossPhase() const -- the boss's current phase (currentBossPhase, +0x18)
// Leaf: ?GetBossPhase@CTCBoss@@QBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007AED10, `mov eax,[ecx+18h] ; ret`.
// Name-fix (agentNF55, round 7; was a free `__fastcall DrawGetEnvironment(const unsigned char*)` reading +24, a propagated manifest
//   label): Ego_r.pdb 0x007B0660 / FableWin.pdb ?GetBossPhase@CTCBoss@@QBEJXZ, the first of the PDB run GetBossPhase /
//   IncrementBossPhase / IncrementStrikesSinceHitHero / GetStrikesSinceHitHero / ResetStrikesSinceHitHero (Ego_r 0x007B0660 ..
//   0x007B06A0) that retail lays out at 0x007AED10 .. 0x007AED50 (ResetStrikesSinceHitHero 0x007AED50 landed so); declared by the
//   shared engine/CTCBoss.h (currentBossPhase +0x18 after GenericCounter / StrikesSinceHit).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/CTCBoss.h"

long CTCBoss::GetBossPhase() const
{
    return currentBossPhase;
}
