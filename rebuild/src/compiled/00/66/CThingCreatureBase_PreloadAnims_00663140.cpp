// CThingCreatureBase::PreloadAnims() -- fetch the creature's appearance sub-def and touch (preload) every animation of its animation set whose entry flags have bit 2 set
// Leaf: ?PreloadAnims@CThingCreatureBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663140, 117 bytes.
// Identity: Ego_r.pdb ?PreloadAnims@CThingCreatureBase@@QAEXXZ (Ego_r 0x0053E070; donor order PreloadAnims / PreloadAnimation =
//   retail 0x00663140 / 0x006631C0).
// Callees: CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, CThing::PeekWorld 0x004C7AB0, CAnimationEntry::GetFlags 0x005DC210,
//   CMeshDataBank::PeekAnimation 0x00A242C0, C3DAnimationInfo::TouchAnimation 0x00A26A50.
// Layout: CThing PBaseDef +0x70; CWorld PMainMeshBank +0x3C; CAppearanceDef Animation +0x34 (its CVectorMap's array first/last at
//   +0x34/+0x38) of 20-byte pairs {unsigned long key; CAnimationEntry}; CAnimationEntry BankIndex +0x00.
// Name fix (agentNF9 2026-11-11): moved off the local CThingCreatureBase / CWorld / CMeshDataBank / CAnimationEntry / CAppearanceDef
//   models (a same-TU stand-in GetSubDef body, a CDefHolder) onto the shared headers; 0x004C7AB0 binds ?PeekWorld@CThing@@IBEABVCWorld@@XZ
//   (was ?GetWorld@CThing@@..), 0x005DC210 ?GetFlags@CAnimationEntry@@QBEJXZ, 0x0042B0A2 the PDB GetSubDef<CAppearanceDef> (was
//   ?GetSubDef@CThingBaseDef@@..).
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
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMATIONENTRY_GETFLAGS
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

void CThingCreatureBase::PreloadAnims()
{
    CDefPointer<const CAppearanceDef> appearance;
    if (PBaseDef->GetSubDef(&appearance)) {
        const CMeshDataBank* bank = PeekWorld().PeekMeshBank();
        for (CAnimationSet::TAnimMap::const_iterator it = appearance->Animation.Anims.begin(); it != appearance->Animation.Anims.end(); ++it) {
            if (it->second.GetFlags() & 4)
                bank->PeekAnimation(it->second.BankIndex)->TouchAnimation();
        }
    }
}
