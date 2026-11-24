// Leaf: ?SetDefaultValue@CAnimComponentCombatComboChain@@UAEXXZ
#define FABLE_RETAIL_ANIMCOMPONENT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CAnimComponentCombatComboChain.h"
#include "engine/CAnimComponentCombatComboChain.h"  // retyped onto the PDB layout; byte parity re-verified
void CAnimComponentCombatComboChain::SetDefaultValue()
{
    CAnimComponentCombatComboChain* component = this; component->ComboID = -999; component->ComboStage = -777; }
// Flatten route (flatten_pilot v4, NF324): CAnimComponentCombatComboChain_SetDefaultValue -> CAnimComponentCombatComboChain::SetDefaultValue, the RTTI-placed PDB member; class model: engine/CAnimComponentCombatComboChain.h under FABLE_RETAIL_ANIMCOMPONENT (preamble of CAnimComponentCombatComboChain_TransferBinaryIn_005dbb60.cpp).
