// CTCInventoryQuests::IsComponentSerialisable() const -- returns false
// Leaf: ?IsComponentSerialisable@CTCInventoryQuests@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00624DB0, thiscall, `ret`.
// Identity: slot 23 of CTCInventoryQuests's vtable 0x012585B4 (defined here: the slot holds this body in 1 vtable(s), all of CTCInventoryQuests or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 35 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCInventoryQuests@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCInventoryQuests.h (opt-ins as in the landed row CTCInventoryQuests_OnCreate_0061a6a0.cpp).
#define FABLE_RETAIL_CTCINVENTORYQUESTS_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_ONCREATE
#define FABLE_RETAIL_CTCINVENTORYQUESTS_MEMBERS
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_REGISTER
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CPLAYERGUI
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CCreatureDef.h"
#include "engine/CTCInventoryQuests.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#include "engine/CPlayerGui.h"
bool CTCInventoryQuests::IsComponentSerialisable() const
{
    return false;
}
