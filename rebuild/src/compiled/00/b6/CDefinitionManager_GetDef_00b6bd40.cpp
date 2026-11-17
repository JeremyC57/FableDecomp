// CDefinitionManager::GetDef<CEngineThemeDef>(const CCharString& class_name, long index, CDefPointer<const CEngineThemeDef>* out) const -- fetch the def at `index` of the class through GetPDefFromClassIndex; on a hit reassign the out pointer (old def released, new one add-ref'd), release the temporary and return true; false for index <= 0 or a null def
// Leaf: ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B6BD40, length 113 bytes (`ret 0Ch`).
// Callees: CDefinitionManager::GetPDefFromClassIndex(const CCharString&, long) const @ 0x009ADE10 (CDefPointer by value
// through the hidden pointer), the def's virtual slot 1 (destroy at a zero count).
// Shape: the GetDef family body compiled for speed (the 113-byte form of 0x006B25E0): the hidden result is homed in the dead
// `index` argument slot and re-read from it at every use; the hit assigns through SetDynamicCast; the temporary's destructor
// releases through the ref-count base's two inline accessors.
// Name (round 7, NF53): the instance is GetDef<CEngineThemeDef> by class name and index: its only retail caller
//   CEngineLandscapeRenderer::BuildThemes 0x00B68899 passes CCharString("ENGINE_THEME") (0x01235F48), the index and an out handle on
//   the engine definition manager 0x01436E28, and FableWin.pdb 0x02900540 lists
//   ??$GetDef@VCEngineThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCEngineThemeDef@@@@@Z next to the
//   CEnvironmentThemeDaySetDef instance (0x02900600). The catalog label GetDef<CSoundThemeDef> belongs to 0x006B25E0. The callee
//   0x009ADE10 is GetPDefFromClassIndex (Ego_r.pdb 0x00B05EA0 / FableWin.pdb 0x03061E20). Was the bare leaf `GetDef` over a local
//   CDefinitionManager / CDefPointer / CDefBase model keyed by `const unsigned long&`; now the header's by-class-index body
//   (FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY with FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD).
//   CEngineThemeDef has no shared header; its base is CParentDefClassBase (RTTI, as the landed 0x004629FB models it).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CParentDefClassBase.h"

class CEngineThemeDef : public CParentDefClassBase { };

template bool CDefinitionManager::GetDef<CEngineThemeDef>(const CCharString&, long, CDefPointer<const CEngineThemeDef>*) const;
