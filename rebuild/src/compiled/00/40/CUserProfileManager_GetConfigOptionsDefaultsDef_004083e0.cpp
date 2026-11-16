// CUserProfileManager::GetConfigOptionsDefaultsDef() const -- obtain configuration-option defaults from the active runtime definition manager (the game's when the main game component exists, else the front end's startup manager).
// Leaf: ?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ
// Identity (FINDINGS chunk 472; sweep2): Ego_r.pdb name set `?GetConfigOptionsDefaultsDef@CUserProfileManager@@QBE?AV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@XZ`;
//   the catalog/file name "GetIconsDef" (NUISystem::CManager::GetIconsDef) is propagated.
// Retail: Fable.exe (TLC, VC7.1) @ 0x004083E0, 194 bytes. Runtime singletons: ?PMainGameComponent@CMainGameComponent@@1PAV1@A 0x013B86A0
//   and ?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A 0x013B871C (read through their inline Get()); getters
//   CGameDefinitionManager::Get 0x0044C6B0 / CStartupDefinitionManager::Get 0x0043368D; CDefinitionManager::GetGlobalIndexFromClassIndex
//   0x009AD390; CDefinitionManager::GetDef<CConfigOptionsDefaultsDef>(long, CDefPointer<const CConfigOptionsDefaultsDef>*) const 0x00410890
//   (FableWin.pdb 0x01897320 ??$GetDef@VCConfigOptionsDefaultsDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCConfigOptionsDefaultsDef@@@@@Z).
// NF41 (2026-11-16): was a renaming-macro row with local CDefinitionManager / CDefPointer / CCharString models, a non-const
//   GetDef(long, CDefPointer*) and two made-up `void*` globals for the singletons; now the include chain of the landed sibling
//   0x004084B0 (whose file holds this same body and grades RELOCATION_MATCH here) with the shared engine/CNewFrontendGameComponent.h,
//   (in place of fable_game.h, whose CGameComponent / CMainGameComponent models clash with the engine headers), the PDB static
//   spellings and the const GetDef template.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CUSERPROFILEMANAGER
#define FABLE_RETAIL_CSTARTUPDEFINITIONMANAGER
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "fable_user_profile.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CStartupDefinitionManager.h"
#include "engine/CNewFrontendGameComponent.h"
class CConfigOptionsDefaultsDef;

template<class T> bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* result) const   // 0x00410890 for T = CConfigOptionsDefaultsDef
{
    if (index <= 0) return false;
    CDefPointer<const CDefClassBase> definition = GetPDefFromGlobalIndex(index);
    if (!definition.IsNull()) { result->SetDynamicCast(definition); return true; }
    return false;
}

CDefPointer<const CConfigOptionsDefaultsDef> CUserProfileManager::GetConfigOptionsDefaultsDef() const
{
    CDefPointer<const CConfigOptionsDefaultsDef> result;
    if (CMainGameComponent::Get())
    {
        long index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CGameDefinitionManager::Get().CDefinitionManager::GetDef(index, &result);
    }
    else if (CNewFrontendGameComponent::Get())
    {
        long index = CStartupDefinitionManager::Get().GetGlobalIndexFromClassIndex("CONFIG_OPTIONS_DEFAULTS_DEF", 1);
        CStartupDefinitionManager::Get().GetDef(index, &result);
    }
    return result;
}
