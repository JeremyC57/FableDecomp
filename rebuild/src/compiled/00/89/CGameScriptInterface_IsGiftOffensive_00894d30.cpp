// CGameScriptInterface::IsGiftOffensive(const CCharString& defName) const -- true when the named def has a gift sub-def of type 2 (offensive)
// Leaf: ?IsGiftOffensive@CGameScriptInterface@@UBE_NABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894D30, length 159 bytes.
// Identity: CGameScriptInterface vtable 0x01260F0C (engine/CGameScriptInterface.h: IsGiftRomantic / IsGiftFriendly / IsGiftOffensive are
// slots 679 / 680 / 681); twins 0x00894BF0:IsGiftRomantic , 0x00894C90:IsGiftFriendly.
// Callees: CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410 (on DefinitionManager +0x10), CGameDefinitionManager::Get @ 0x0044C6B0
// (static), CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C, CDefClassBase::GetSubDef<CGiftDef> @ 0x006D3EF0, the defs' virtual
// slot 1 when a release drops the count to zero.
// Shape: plain member code over the shared headers (the noinline GetDef / GetSubDef bodies keep the handles' addresses unescaped, so
// the class def stays in esi and both handles share the dead argument word); the gift def's release after `Type == 2` drops its null
// test, as retail.
// Name fix (agentNF10 2026-11-11): moved off the local CCharString / CDefPointerSlot / CDefClass_Retail / CGiftDef_Retail /
// CGameDefinitionManager_Retail / CDefinitionManager stand-ins (and their same-TU stand-in bodies) onto the shared headers (new
// engine/CGiftDef.h; CGameScriptInterface.h under FABLE_RETAIL_NSCRIPT). 0x0042AF3C binds GetDef<CThingBaseDef> (was
// ?GetDef@CGameDefinitionManager_Retail@@QAE_NJPAU?$CDefPointerSlot@UCDefClass_Retail@@@@@Z), 0x006D3EF0
// ??$GetSubDef@VCGiftDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCGiftDef@@@@@Z (FableWin.pdb), the gift type is the PDB member
// CGiftDef::Type (was m_giftType).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_NSCRIPT
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThingBaseDef.h"
#include "engine/CGiftDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::IsGiftOffensive(const CCharString& def_name) const
{
    long index = DefinitionManager->GetDefGlobalIndexFromName(def_name);
    CDefPointer<const CThingBaseDef> def;
    if (CGameDefinitionManager::Get().GetDef(index, &def)) {
        CDefPointer<const CGiftDef> gift_def;
        if (def->GetSubDef(&gift_def))
            return gift_def->Type == 2;
    }
    return false;
}
