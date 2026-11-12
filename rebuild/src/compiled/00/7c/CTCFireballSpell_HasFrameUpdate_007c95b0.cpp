// CTCFireballSpell::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCFireballSpell@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C95B0, thiscall, `ret`.
// Identity: slot 8 of CTCFireballSpell's vtable 0x0126E964 (defined here: the slot holds this body in 1 vtable(s), all of CTCFireballSpell or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCFireballSpell@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCFireballSpell.h (opt-ins as in the landed row CTCInventory_OpenPCInventory_0058f649.cpp).
#define FABLE_RETAIL_CTCFIREBALLSPELL_VSLOTS
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_VTABLE
#define FABLE_RETAIL_CTCINVENTORYBASE_CAPTUREBACKBUFFER
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_OPTIONSMENU
#define FABLE_RETAIL_CTCINVENTORY_OPENPC
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCOUNTEDPOINTER_CALLED_CTOR
#define FABLE_RETAIL_CDELETION
#define FABLE_RETAIL_CDELETION_BODIES
#define FABLE_RETAIL_CTCFIREBALLSPELL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_GET3DENGINE
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_SCOPED_PTR
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCInventory.h"
#include "engine/CTCFireballSpell.h"
#include "engine/CASoundManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/NUISystem_CAction.h"
#include "engine/NUISystem_EActionType.h"
#include "engine/CCountedPointer.h"
bool CTCFireballSpell::HasFrameUpdate() const
{
    return true;
}
