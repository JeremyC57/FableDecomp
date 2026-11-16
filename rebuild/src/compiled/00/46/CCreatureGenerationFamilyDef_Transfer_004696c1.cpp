// Leaf: ?Transfer@CCreatureGenerationFamilyDef@@UAEXAAVCPersistContext@@@Z
#pragma optimize("s",on)
// Port (round 4, NF40): the local CPersistContext model (stand-in Transfer_00466a47 / Transfer_004696fa members) and the
//   pad-member stand-in class are replaced by engine/CCreatureGenerationFamilyDef.h (Ego_r.pdb members Creatures,
//   DifficultyLevel, GeneratorObject, GenerationTypes; the FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_TRANSFER opt-in declares the
//   override) and engine/CPersistContext.h: TransferVector<long> 0x00466A47, Transfer<long> 0x00431102 x2,
//   TransferVector<ECreatureGeneratorGenerateType> 0x004696FA.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCREATUREGENERATIONFAMILYDEF_TRANSFER
#include "engine/CCreatureGenerationFamilyDef.h"
#include "engine/CPersistContext.h"

void CCreatureGenerationFamilyDef::Transfer(CPersistContext& context)
{
    context.TransferVector(Creatures);
    context.Transfer(DifficultyLevel);
    context.Transfer(GeneratorObject);
    context.TransferVector(GenerationTypes);
}
