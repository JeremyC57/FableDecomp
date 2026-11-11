// CTCHeroSpecialMovement::OnCreate() -- look up the parent thing's def by its def index and take its hero special-movement sub-def; then clear Sprinting and preset the sprint frames from the def's sprint duration
// Leaf: ?OnCreate@CTCHeroSpecialMovement@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0910, length 105 bytes (+7 int3 padding). Vtable 0x0126FE3C slot 4 (Ego_r.pdb /
// FableWin.pdb ?OnCreate@CTCHeroSpecialMovement@@UAEXXZ).
// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CHeroSpecialMovementDef> @ 0x007E0A40 (FableWin.pdb ??$GetSubDef@VCHeroSpecialMovementDef@@@..), the def's
// slot 1 on release. Globals: CMainGameComponent::ConstantFPS 0x01375550 (inline GetConstantFPS()).
// Shape: the def index is an unsigned short (`mov ax,[eax+8Ch] ; test ax,ax`, zero-extended for the call); `(float)fps * duration`
// loads the long first.
// Layout: engine/CTCHeroSpecialMovement.h (Ego_r.pdb, same offsets in retail): Sprinting +0x0c, SprintFramesRemaining +0x10,
// PHeroSpecialMovementDef +0x18; CHeroSpecialMovementDef TotalTimeToSprintSeconds +0x28 (engine/CHeroSpecialMovementDef.h).
// Name fix (agentNF10 2026-11-11): moved off the local CTCBase / CThing / CDefPointer / CDefClassBase / CDefinitionManager /
// CHeroSpecialMovementDef / CTCHeroSpecialMovement stand-ins onto the shared headers (new engine/CTCHeroSpecialMovement.h,
// CHeroSpecialMovementDef.h FABLE_RETAIL_CHEROSPECIALMOVEMENTDEF). 0x0042AF3C binds GetDef<CThingBaseDef> (was
// ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@$$CBVCDefClassBase@@@@@Z), 0x006869D0 the PDB const GetDefinitionManager (was
// ?GetDefinitions@CTCBase@@QAEPAVCDefinitionManager@@XZ), 0x007E0A40 the FableWin.pdb template spelling. The def's sprint duration is
// its PDB member TotalTimeToSprintSeconds (was the invented SprintDurationSeconds).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CHEROSPECIALMOVEMENTDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHeroSpecialMovementDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CTCHeroSpecialMovement.h"

void CTCHeroSpecialMovement::OnCreate()
{
    unsigned short def_index = (unsigned short)ParentThing->GetDefGlobalIndex();
    if (def_index) {
        CDefPointer<const CThingBaseDef> def;
        GetDefinitionManager().GetDef(def_index, &def);
        def->GetSubDef(&PHeroSpecialMovementDef);
    }
    Sprinting = false;
    SprintFramesRemaining = (float)CMainGameComponent::GetConstantFPS() * PHeroSpecialMovementDef->TotalTimeToSprintSeconds;
}
