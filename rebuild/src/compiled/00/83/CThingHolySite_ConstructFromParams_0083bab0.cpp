// CThingHolySite::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, bool playerStart) -- base CThingGameObject construction with an empty script name; on success fetches the holy-site def by index and records PlayerStart
// Leaf: ?ConstructFromParams@CThingHolySite@@IAE_NJABVC3DVector@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BAB0, length 131 bytes (`ret 10h`, + int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromParams@CThingHolySite@@IAE_NJABVC3DVector@@J_N@Z (protected; Create pushes the bool,
//   player, position and def index).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0 ("" 0x0122D70E), CThingGameObject::ConstructFromParams 0x008388D0
//   (eight arguments), CCharString::~CCharString 0x0099EAE0, CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingHolySiteDef> 0x0083BD10.
// Layout: CThing DefGlobalIndex +0x8C; CThingHolySite Def (CDefPointer<const CThingHolySiteDef>) +0xD0 (Ego_r +0xE8), PlayerStart +0xD4.
// Shape: the empty name is a named CCharString local in its own block whose call result goes to a bool local (bl) tested after the
//   dtor; base arguments flags = 1, creators 0, final flag true; `playerStart != 0` gives `test al,al ; setne al`.
// Name fix (agentNF9 2026-11-11): moved off the local CThingGameObject / CThingHolySite / CDefinitionManager models onto the shared
//   headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0083BD10
//   ??$GetDef@VCThingHolySiteDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingHolySiteDef@@@@@Z (was
//   ?GetDef@CDefinitionManager@@QAE_NJAAUCDefPointerHolySiteDef@@@Z), 0x008388D0 the PDB
//   ?ConstructFromParams@CThingGameObject@@IAE_NJABVC3DVector@@JKABVCCharString@@PAVCThingPhysical@@2_N@Z; the row's own symbol is the
//   PDB's protected `const C3DVector&` form (was ?ConstructFromParams@CThingHolySite@@QAE_NJPBVC3DVector@@J_N@Z).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingHolySite.h"

bool CThingHolySite::ConstructFromParams(long defIndex, const C3DVector& position, long playerNumber, bool playerStart)
{
    bool ok;
    {
        CCharString name("");
        ok = CThingGameObject::ConstructFromParams(defIndex, position, playerNumber, 1, name, 0, 0, true);
    }
    if (ok) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &Def);
        PlayerStart = playerStart != 0;
        return true;
    }
    return false;
}
