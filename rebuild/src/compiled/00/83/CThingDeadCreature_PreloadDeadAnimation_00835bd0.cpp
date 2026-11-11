// CThingDeadCreature::PreloadDeadAnimation() -- run the dead-creature animation preloader over every entry of the appearance definition's animation set, resolving the matching entries through the world's main mesh bank
// Leaf: ?PreloadDeadAnimation@CThingDeadCreature@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835BD0, 75 bytes (+ int3 padding), plain `ret`. Ego_r.pdb / FableWin.pdb
//   ?PreloadDeadAnimation@CThingDeadCreature@@IAEXXZ (protected; one caller, OnCreate 0x00835FD0).
// Callees (PDB spellings): CThing::PeekWorld() const 0x004C7AB0 (the mesh bank through the inline CWorld::PeekMeshBank, +0x3C),
//   CDefClassBase::GetSubDef<CAppearanceDef> 0x0042B0A2, std::for_each<const pair<unsigned long,CAnimationEntry>*,
//   CDeadCreatureAnimPreloader> 0x008365F0 (landed; /Gr __fastcall: hidden result in ecx, first in edx, last and the by-value
//   functor on the stack), the def's virtual release.
// Layout: CThing PBaseDef +0x70; CWorld PMainMeshBank +0x3C; CAppearanceDef Animation +0x34 (Anims start +0x34 / finish +0x38).
// Shape: the bank is read first; the functor is built from the bank (Ego_r ??0CDeadCreatureAnimPreloader@@QAE@ABVCMeshDataBank@@@Z,
//   inline) and its returned copy lands in the dead out-handle slot.
// Name fix (agentNF11 2026-11-11): moved off the local CThing / CDefPointer / CAppearanceDef / CAnimationSet stand-ins (and the same-TU
// stand-in GetSubDef and for_each bodies) onto the shared headers (new engine/CDeadCreatureAnimPreloader.h). 0x0042B0A2 binds
// GetSubDef<CAppearanceDef> (was ?GetSubDef@CDefClassBase@@QAE_NAAV?$CDefPointer@VCAppearanceDef@@@@@Z), 0x008365F0 the landed
// for_each spelling (was ?ForEachAnimation@@YI?AUCPreloadDeadAnimation@@PAUSAnimationSetEntry@@0U1@@Z), 0x004C7AB0 PeekWorld; the
// row's own symbol is the PDB protected IAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGDEADCREATURE_ANIMATION
#define FABLE_RETAIL_CANIMATIONSET_ANIMS
#define FABLE_RETAIL_CANIMFILTER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CAnimationEntry.h"
#include "engine/CAppearanceDef.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CDeadCreatureAnimPreloader.h"

void CThingDeadCreature::PreloadDeadAnimation()
{
    const CMeshDataBank& mesh_bank = *PeekWorld().PeekMeshBank();
    CDefPointer<const CAppearanceDef> appearance;
    PBaseDef->GetSubDef(&appearance);
    const CAnimationSet& animations = appearance->Animation;
    std::for_each(animations.Anims.begin(), animations.Anims.end(), CDeadCreatureAnimPreloader(mesh_bank));
}
