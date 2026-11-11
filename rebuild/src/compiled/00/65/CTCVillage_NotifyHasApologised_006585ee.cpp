// CTCVillage::NotifyHasApologised() -- remember the frame the hero last apologised to the village
// Leaf: ?NotifyHasApologised@CTCVillage@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006585EE, 21 bytes, plain `ret` (packed run: the previous function ends `ret 8` at 0x006585EB).
// Re-identified (agentNF14 2026-11-11; the catalog called it CTCHeroExperience::AddSuccessfulBlockParry, landed as a free __fastcall
// stub): the body stores CWorld::GetFrame() through `this` +0xd8, which is CTCVillage::LastFrameApologised in the retail CTCVillage
// layout (engine/CTCVillage.h FABLE_RETAIL_CTCVILLAGE: HeroEnemyOfGuardsUntil +0xd4, LastFrameApologised +0xd8), and it sits in the
// Ego_r.pdb donor order NotifyHasApologised, CreateDummyVillagersIfNecessary (empty: the lone `ret` at 0x00658603),
// GetIsHeroEnemyOfGuards (0x00658604, which reads +0xd4).
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused: the static frame read goes through the world reference),
// CWorld::GetFrame() @ 0x0049D870 (static __fastcall).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_APOLOGISE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCVillage.h"
#include "engine/CWorld.h"

void CTCVillage::NotifyHasApologised()
{
    LastFrameApologised = GetWorld().GetFrame();
}
