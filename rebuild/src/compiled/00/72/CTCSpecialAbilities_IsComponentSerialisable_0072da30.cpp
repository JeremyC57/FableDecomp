// CTCSpecialAbilities::IsComponentSerialisable() const -- returns true
// Leaf: ?IsComponentSerialisable@CTCSpecialAbilities@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072DA30, thiscall, `ret`.
// Identity: slot 23 of CTCSpecialAbilities's vtable 0x0126683C (defined here: the slot holds this body in 1 vtable(s), all of CTCSpecialAbilities or derived); the Xbox 360 Test PDB virtual order (third source) aligned on 17 TLC-named slots of that vtable puts IsComponentSerialisable(void) at this slot; Ego_r/FableWin name set: ?IsComponentSerialisable@CTCSpecialAbilities@@UBE_NXZ; `ret` matches the signature.
// Header: engine/CTCSpecialAbilities.h (opt-ins as in the landed row CWorld_EAUseProjectileWeapon_0062bef0.cpp).
#define FABLE_RETAIL_CTCSPECIALABILITIES_VSLOTS
#define FABLE_RETAIL_CGAMEEVENT_DATAPACKAGE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_EVENTACTIONS
#define FABLE_RETAIL_CTCCARRYING
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#define FABLE_RETAIL_CTCHERORANGEDATTACKS_PROJECTILEMODE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMERASOUND
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayer.h"
#include "engine/CTCBase.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameEvent.h"
#include "engine/EPlayerMode.h"
#include "engine/CWorld.h"
#define FABLE_FASTCALL __fastcall
#define FABLE_FASTCALL
bool CTCSpecialAbilities::IsComponentSerialisable() const
{
    return true;
}
