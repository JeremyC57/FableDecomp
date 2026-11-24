// Leaf: ?GetSizeofClass@CCombatDialogueDef@@UAEKXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCombatDialogueDef.h"
typedef unsigned long ulong;

unsigned long CCombatDialogueDef::GetSizeofClass()
{
    return 0x8c;
}
// Flatten route (flatten_pilot v4, NF324): CCombatDialogueDef_GetSizeofClass -> CCombatDialogueDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CCombatDialogueDef.h under its default branch (preamble of CCreatureActionGenericLeaderCommand_Initialise_008bf800.cpp).
