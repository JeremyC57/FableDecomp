// CTCBetting::HasFrameUpdate() const -- returns true
// Leaf: ?HasFrameUpdate@CTCBetting@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E5910, thiscall, `ret`.
// Identity: slot 8 of CTCBetting's vtable 0x01270624 (defined here: the slot holds this body in 1 vtable(s), all of CTCBetting or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts HasFrameUpdate(void) at this slot; Ego_r/FableWin name set: ?HasFrameUpdate@CTCBetting@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCBetting.h (opt-ins as in the landed row CTCBetting_GetPlayersMoney_007e5570.cpp).
#define FABLE_RETAIL_CTCBETTING_VSLOTS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_BRIBE
#define FABLE_RETAIL_CTCHEROSTATS_GETMONEY
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKPLAYER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CBETTINGDEF
#define FABLE_RETAIL_CTCBETTING
#define FABLE_RETAIL_CTCAISCRATCHPAD_BETTING
#include "fable_string.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "fable_game.h"
#include "engine/CTCBetting.h"
#include "engine/CBettingDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/CDefClassBase_GetSubDef.h"
bool CTCBetting::HasFrameUpdate() const
{
    return true;
}
