// Leaf: ?GetSizeofClass@CTargetingDef@@UAEKXZ
#define FABLE_RETAIL_CTARGETINGDEF_RTTI_DECLS
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CTargetingDef.h"
typedef unsigned long ulong;

unsigned long CTargetingDef::GetSizeofClass()
{
    return 0x90;
}
// Flatten route (flatten_pilot v4, NF324): CTargetingDef_GetSizeofClass -> CTargetingDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CTargetingDef.h opt-in FABLE_RETAIL_CTARGETINGDEF_RTTI_DECLS (preamble of CTCTargetingAI_BulletTimeFrameUpdate_004ddaa4.cpp).
