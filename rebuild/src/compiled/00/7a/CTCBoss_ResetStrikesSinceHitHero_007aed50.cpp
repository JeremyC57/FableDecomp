// CTCBoss::ResetStrikesSinceHitHero() -- NF37 (2026-11-16) port off the address-suffixed stand-in onto the shared engine/CTCBoss.h.
// Leaf: ?ResetStrikesSinceHitHero@CTCBoss@@QAEXXZ
// Evidence: Ego_r.pdb / FableWin.pdb ?ResetStrikesSinceHitHero@CTCBoss@@QAEXXZ (donor 0x007B06A0 after GetStrikesSinceHitHero 0x007B0690 = retail 0x007AED50 after 0x007AED40); the header STRIKES opt-in names 0x007AED50; stores 0 to StrikesSinceHit +0x14.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#define FABLE_RETAIL_CTCBOSS_STRIKES
#include "engine/CTCBoss.h"

void CTCBoss::ResetStrikesSinceHitHero()
{
    StrikesSinceHit = 0;
}
