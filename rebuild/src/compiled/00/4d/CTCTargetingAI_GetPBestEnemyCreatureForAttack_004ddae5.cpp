// CTCTargetingAI::GetPBestEnemyCreatureForAttack() const -- the best enemy creature to attack, as last chosen (PBestEnemyCreatureForAttack +0x38)
// Leaf: ?GetPBestEnemyCreatureForAttack@CTCTargetingAI@@UBEPAVCThingCreatureBase@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDAE5, 8 bytes: `add ecx,38h ; jmp CBaseIntelligentPointer::GetPItem 0x00A01B50`.
// Name-fix 2026-11-12 (was the stand-in CTCTargetingAI::vfunc_29 over a local CTCTargetingAI): CTCTargetingAI vtable 0x012411FC
//   slot 29 = 0x004DDAE5 (rebuild/manifest/rtti-vtables.tsv); slots 28 / 30 are the landed GetPBestThingForMeleeAttack 0x004DDA83 /
//   GetPBestThingForProjectileTargetingLock 0x004DDA86, and engine/CTCTargetingBase.h (PDB order) puts GetPBestEnemyCreatureForAttack
//   between them; Ego_r.pdb + FableWin.pdb ?GetPBestEnemyCreatureForAttack@CTCTargetingAI@@UBEPAVCThingCreatureBase@@XZ (public
//   virtual const), Ego_r.pdb member PBestEnemyCreatureForAttack +0x38. Class from engine/CTCTargetingAI.h
//   (FABLE_RETAIL_CTCTARGETINGAI_ATTACK), with the include set of the landed 0x007F67C0.
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTCTARGETINGAI_ATTACK
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingAI.h"
#include "engine/CWorldMap.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"

CThingCreatureBase* CTCTargetingAI::GetPBestEnemyCreatureForAttack() const
{
    return PBestEnemyCreatureForAttack.Get();
}
