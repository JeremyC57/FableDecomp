// CThingCreatureBase::PreloadAnimation(const CCharString& name) -- fetch the creature's appearance sub-def and touch (preload) every variant of one named animation in its animation set (no group filter)
// Leaf: ?PreloadAnimation@CThingCreatureBase@@QAEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006631C0, 160 bytes (`ret 4`).
// Identity: Ego_r.pdb ?PreloadAnimation@CThingCreatureBase@@QAEXABVCCharString@@@Z, donor order PreloadAnims / PreloadAnimation = Ego_r
//   0x0053E070 / 0x0053E100 = retail 0x00663140 / 0x006631C0. The name is passed through by address to the CCharString-keyed set queries,
//   which is why the landed `long animation` form had the same bytes.
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CThing::PeekWorld 0x004C7AB0, CAnimationSet::GetNoAnims 0x005DC290,
//   CAnimationSet::GetAnim 0x005DC340, CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::TouchAnimation 0x00A26A50; two
//   CAnimFilterNoGroup temporaries (vtable 0x0122FFDC).
// Layout: CThing PBaseDef +0x70; CWorld PMainMeshBank +0x3C (the inline PeekMeshBank); CAppearanceDef Animation +0x34; CAnimationEntry
//   BankIndex +0x00.
// Name fix (agentNF9 2026-11-11): moved off the local CThingCreatureBase / CWorld / CMeshBank / CAnimationSet / CAnimFilter models (a
//   same-TU stand-in GetSubDef body) onto the shared headers; the callees now bind their PDB spellings: 0x004C7AB0
//   ?PeekWorld@CThing@@IBEABVCWorld@@XZ (was ?GetWorld@CThingCreatureBase@@QAEPAVCWorld@@XZ), 0x00A242C0
//   ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z (was ?GetAnimation@CMeshBank@@...), 0x005DC290
//   ?GetNoAnims@CAnimationSet@@QBEJABVCCharString@@ABVCAnimFilterBase@@@Z (was ?GetNumberOfAnimations@..), 0x005DC340
//   ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z (was ?GetAnimation@..), 0x0042B0A2 the PDB
//   GetSubDef<CAppearanceDef> (was ?GetSubDef@CThingBaseDef@@..); the row's own symbol is the PDB's (was ..QAEXJ@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CANIMATIONSET_GETNOANIMS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CAPPEARANCEDEF_COMPLETE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKANIMATION
#define FABLE_RETAIL_C3DANIMATIONINFO_LOADING
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CAnimFilterNoGroup.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DAnimationInfo.h"

void CThingCreatureBase::PreloadAnimation(const CCharString& name)
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance)) {
        const CMeshDataBank* bank = PeekWorld().PeekMeshBank();
        const CAnimationSet& set = appearance->Animation;
        long count = set.GetNoAnims(name, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->PeekAnimation(set.GetAnim(name, i, CAnimFilterNoGroup()).BankIndex)->TouchAnimation();
    }
}
