// CTCQuestCompletionUI::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCQuestCompletionUI@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9750, thiscall, `ret`.
// Identity: slot 23 of CTCQuestCompletionUI's vtable 0x0125589C (defined here: the slot holds this body in 1 vtable(s), all of CTCQuestCompletionUI or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCQuestCompletionUI@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCQuestCompletionUI.h (opt-ins as in the landed row CTCQuestCompletionUI_DrawPreparePrimitivesForRendering_005e3f00.cpp).
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_VSLOTS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCQUESTCOMPLETIONUI_DRAW
#define FABLE_RETAIL_CUSERPROFILEMANAGER_UIBACKGROUND
#include "engine/ETCInterfaceType.h"
#include "engine/CTCQuestCompletionUI.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/NUISystem_CUIMiscThingsDef.h"
#include "engine/CDefPointer.h"
#include "engine/NUISystem_CManager.h"
#include "engine/CUserProfileManager.h"
#include "engine/GFGetTime.h"
bool CTCQuestCompletionUI::IsComponentSerialisable() const
{
    return true;
}
