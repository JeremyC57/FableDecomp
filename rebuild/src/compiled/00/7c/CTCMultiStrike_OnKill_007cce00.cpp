// CTCMultiStrike::OnKill() -- the kill path: FinalCleanup
// Leaf: ?OnKill@CTCMultiStrike@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CCE00, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 14 of CTCMultiStrike's vtable 0x0126EE0C (the slot holds this body in 1 vtable(s), owner CTCMultiStrike); the Xbox 360 Test PDB virtual order (third source) aligned on 14 TLC-named slots of that vtable puts OnKill(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnKill@CTCMultiStrike@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCMultiStrike::vfunc_14 (rebuild/src/compiled/00/7c/gapscan_sub007cce00_007cce00.cpp).
// Opt-ins as in the landed row CTCMultiStrike_CTCMultiStrike_007cd390.cpp.
#define FABLE_RETAIL_CTCMULTISTRIKE_VSLOTS2
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCSPECIALABILITIES_FULL
#define FABLE_RETAIL_CTCMULTISTRIKE_MEMBERS
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/C3DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CSpecialAbilitiesMultiStrikeDef.h"
#include "engine/CTCMultiStrike.h"
void CTCMultiStrike::OnKill()
{
    FinalCleanup();
}
