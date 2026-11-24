// Leaf: ?GetSizeofClass@CTrollBattleDef@@UAEKXZ
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS
#include "fable_string.h"
#include "engine/CTrollBattleDef.h"
#pragma optimize("s",on)
unsigned long CTrollBattleDef::GetSizeofClass()
{
    return 96;
}
// Flatten route (flatten_pilot v4, NF324): CTrollBattleDef_GetSizeofClass -> CTrollBattleDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CTrollBattleDef.h under FABLE_RETAIL_CTROLLBATTLEDEF_MEMBERS (preamble of CTrollBattleDef_Alloc_004e4833.cpp).
