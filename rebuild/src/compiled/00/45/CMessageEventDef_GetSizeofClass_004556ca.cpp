// Leaf: ?GetSizeofClass@CMessageEventDef@@UAEKXZ
#define FABLE_RETAIL_CMESSAGEEVENTDEF_RTTI_DECLS
#include "fable_compiler.h"
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CMessageEventDef.h"
#pragma optimize("s",on)
unsigned long CMessageEventDef::GetSizeofClass()
{
    return 0x50;
}
// Flatten route (flatten_pilot v4, NF324): CMessageEventDef_GetSizeofClass -> CMessageEventDef::GetSizeofClass, the RTTI-placed PDB member; class model: engine/CMessageEventDef.h opt-in FABLE_RETAIL_CMESSAGEEVENTDEF_RTTI_DECLS (preamble of CThingAICreature_OnDie_00832e60.cpp).
