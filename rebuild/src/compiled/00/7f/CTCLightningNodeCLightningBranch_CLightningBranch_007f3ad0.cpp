// CTCLightningNode::CLightningBranch::CLightningBranch(CThing& target) -- two intelligent pointers (vtable 0x01238C6C): PTarget (+0) bound to
// the target through SetPItem (0x00A01B90), PEffect (+8) empty, and Timer (+0x10) cleared
// Leaf: ??0CLightningBranch@CTCLightningNode@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3AD0, `ret 4`. Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, SetPItem @ 0x00A01B90.
// NF39 (2026-11-16): moved off its global CLightningBranch stand-in (a void* target over a local intelligent-pointer model) onto the
//   nested class of the shared engine/CTCLightningNode.h (FABLE_RETAIL_CTCLIGHTNINGNODE_FRAMEUPDATE; overlay opt-in
//   _BRANCH_CTOR_DECL keeps the in-class body as a declaration so this row defines the out-of-line copy): Ego_r.pdb 0x007ED080 /
//   FableWin.pdb 0x027AE5F0 ??0CLightningBranch@CTCLightningNode@@QAE@AAVCThing@@@Z; FrameUpdate 0x007F2DC0 pushes the thing and calls it
//   on the 0x14-byte new block. Member names are the header's (descriptive, no PDB layout record).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_CREATE
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_UPDATEPARAMETERS
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_CTHINGFILTER_ENEMYCREATURE_CTOR
#define FABLE_RETAIL_C2DBOXI_CENTRE_RADIUS_OUTOFLINE
#define FABLE_RETAIL_CHITPARAMETERS_CTOR
#define FABLE_RETAIL_CHITPARAMETERS_ADDDAMAGE
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT_FORCEINLINE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCLIGHTNINGNODE_FRAMEUPDATE
#define FABLE_RETAIL_CTCLIGHTNINGNODE_BRANCH_CTOR_DECL
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCDParticleEmitter.h"
#include "engine/NParticleEngine_CParticleUpdateParameters.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingSearchTools.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Bind3.h"
#include "engine/CThingFilter_In3DRange.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CHitParameters.h"
#include "engine/CTCElectrocutedEffect.h"
#include "engine/CLightningDef.h"
#include "engine/CTCLightningNode.h"

CTCLightningNode::CLightningBranch::CLightningBranch(CThing& target)
    : PTarget(&target), Timer(0)
{
}
