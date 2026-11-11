// CThingCreatureBase::PreloadBasicAnimations() -- start loading every walk and run animation of the creature's appearance animation set, and every animation whose name starts with "TURNING_"
// Leaf: ?PreloadBasicAnimations@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663610.
// Identity: Ego_r.pdb ?PreloadBasicAnimations@CThingCreatureBase@@QAEXXZ (Ego_r 0x0053E560, the last of the appearance-animation run
//   0x0053D8E0.. = retail 0x00662A00..0x00663610).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CThing::PeekWorld 0x004C7AB0, CCharString(const char*, long) 0x0099EBF0,
//   ~CCharString 0x0099EAE0, CAnimationSet::GetNoAnims 0x005DC290, CAnimationSet::GetAnim 0x005DC340,
//   CAnimationSet::GetAnimsWithNameStartingWith 0x005DCB00, CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::BeginLoading
//   0x00A26730, free 0x00BFEA14 (the vector's storage).
// Strings: 0x0125A990 "ST_WALK", 0x0125A998 "ST_RUN", 0x0125AB08 "TURNING_". CAnimFilterNoGroup vtable 0x0122FFDC.
// Name fix (agentNF9 2026-11-11): moved off the local CThingCreatureBase / CWorld / CMeshBank / CAnimationSet / CAnimFilter models (a
//   same-TU stand-in GetSubDef body) onto the shared headers; the callees bind their PDB spellings: 0x004C7AB0
//   ?PeekWorld@CThing@@IBEABVCWorld@@XZ (was ?GetWorld@CThing@@..), 0x00A242C0 ?PeekAnimation@CMeshDataBank@@QBEPBVC3DAnimationInfo@@K@Z
//   (was ?GetAnimation@CMeshBank@@QAEPAVC3DAnimationInfo@@J_N@Z), 0x00A26730 ?BeginLoading@C3DAnimationInfo@@QBEX_N@Z, 0x005DC290
//   ?GetNoAnims@CAnimationSet@@QBEJABVCCharString@@ABVCAnimFilterBase@@@Z (was ?GetNumberOfAnimations@..), 0x005DC340
//   ?GetAnim@CAnimationSet@@QBEABVCAnimationEntry@@ABVCCharString@@JABVCAnimFilterBase@@@Z (was ?GetAnimation@..), 0x0042B0A2 the PDB
//   GetSubDef<CAppearanceDef>.
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
#define FABLE_RETAIL_CANIMATIONSET_QUERIES
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

void CThingCreatureBase::PreloadBasicAnimations()
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance)) {
        const CMeshDataBank* bank = PeekWorld().PeekMeshBank();
        const CAnimationSet& set = appearance->Animation;
        CCharString walk("ST_WALK", -1);
        long count = set.GetNoAnims(walk, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->PeekAnimation(set.GetAnim(walk, i, CAnimFilterNoGroup()).BankIndex)->BeginLoading(false);
        CCharString run("ST_RUN", -1);
        count = set.GetNoAnims(run, CAnimFilterNoGroup());
        for (long i = 0; i < count; ++i)
            bank->PeekAnimation(set.GetAnim(run, i, CAnimFilterNoGroup()).BankIndex)->BeginLoading(false);
        CCharString turning("TURNING_", -1);
        std::vector<const CAnimationEntry*> anims;
        set.GetAnimsWithNameStartingWith(turning, anims, CAnimFilterNoGroup());
        for (unsigned long i = 0; i < anims.size(); ++i)
            bank->PeekAnimation(anims[i]->BankIndex)->BeginLoading(false);
    }
}
