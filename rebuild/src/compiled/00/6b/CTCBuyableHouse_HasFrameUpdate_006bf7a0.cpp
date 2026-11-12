// CTCBuyableHouse::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBuyableHouse@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BF7A0, thiscall, `ret`.
// Identity: slot 8 of CTCBuyableHouse's vtable 0x0125D6A4 (defined here: the slot holds this body in 1 vtable(s), all of CTCBuyableHouse or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 16 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBuyableHouse@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBuyableHouse.h (opt-ins as in the landed row CGameScriptInterface_OpenChest_00896390.cpp).
#define FABLE_RETAIL_CTCBUYABLEHOUSE_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_CTCHERO_TUTORIAL
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_OPENCHEST
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CTCHero.h"
#include "engine/CTCChest.h"
#include "engine/CTCOwnedEntity.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CCreatureAction_OpenChest.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
bool CTCBuyableHouse::HasFrameUpdate() const
{
    return true;
}
