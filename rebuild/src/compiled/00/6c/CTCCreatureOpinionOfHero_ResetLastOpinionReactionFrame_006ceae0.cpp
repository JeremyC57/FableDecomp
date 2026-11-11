// CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame() -- restart the opinion-reaction timer at the current world frame
// Leaf: ?ResetLastOpinionReactionFrame@CTCCreatureOpinionOfHero@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006CEAE0, 18 bytes, plain `ret`.
// Re-identified (agentNF14 2026-11-11; the catalog called it CTCHeroExperience::AddSuccessfulBlockParry, landed as a free __fastcall
// stub over a local pad struct): FableWin.pdb ?ResetLastOpinionReactionFrame@CTCCreatureOpinionOfHero@@QAEXXZ, already declared at this
// address by engine/CTCCreatureOpinionOfHero.h (FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET, chunk 795); its one caller 0x0092FFF8
// (in CAIStateGroup greet code) calls it on the TCI_CREATURE_OPINION_OF_HERO (79) component right after setting its interacted /
// greeted flags +0x340 / +0x341, and the body stores the frame to LastOpinionReactionFrame +0x0C (Ego_r.pdb layout, the only setter
// of that member in the FableWin name set).
// Callees: CTCBase::GetWorld() const @ 0x00686A70 (result unused), CWorld::GetFrame() @ 0x0049D870 (static __fastcall).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#include "engine/ETCInterfaceType.h"
enum EOpinionAttitudeType { };                              // Ego_r.pdb enum (enumerators not used here; engine/COpinionDeedReactionDef.h spelling)
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CWorld.h"

void CTCCreatureOpinionOfHero::ResetLastOpinionReactionFrame()
{
    LastOpinionReactionFrame = GetWorld().GetFrame();
}
