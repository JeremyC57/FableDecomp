// CTCVillage::GetIsHeroEnemyOfGuards() -- is the world frame still before the frame the guards' enmity runs until
// Leaf: ?GetIsHeroEnemyOfGuards@CTCVillage@@QAE_NXZ
// Flags: /Os
// Retail: Fable.exe (TLC, VC7.1) @ 0x00658604, 28 bytes, plain `ret` (packed after the empty CreateDummyVillagersIfNecessary `ret`
// at 0x00658603). Ego_r.pdb ?GetIsHeroEnemyOfGuards@CTCVillage@@QAE_NXZ (the catalog name; landed as a free __fastcall stub over a
// local pad struct): `GetWorld().GetFrame() < HeroEnemyOfGuardsUntil` (+0xd4 in the retail CTCVillage layout, engine/CTCVillage.h).
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused), CWorld::GetFrame() @ 0x0049D870 (static __fastcall).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_APOLOGISE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"

bool CTCVillage::GetIsHeroEnemyOfGuards()
{
    return GetWorld().GetFrame() < HeroEnemyOfGuardsUntil;
}
