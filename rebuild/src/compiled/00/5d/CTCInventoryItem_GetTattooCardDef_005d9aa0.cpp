// CTCInventoryItem::GetTattooCardDef(long defIndex) -- static: fetch the thing def for defIndex from the game definition manager and return its CTattooDef sub-definition as a counted pointer (null when either lookup fails)
// Leaf: ?GetTattooCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCTattooDef@@@@J@Z
// Retail: Fable.exe @ 0x005D9AA0, 137 bytes (+7 int3). Ego_r.pdb `?GetTattooCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCTattooDef@@@@J@Z`
// (static __fastcall: the hidden CDefPointer result in ecx, defIndex in edx; `ret`). Twin: GetHairCardDef 0x005D9A10.
// Instance twin: GetTattooCardDef() const 0x005D93B0 (the owner thing's def instead of a manager lookup).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall), CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CTattooDef> @ 0x005B1EFC, the defs' virtual slot 1 on the last release.
// Shape: speed TU; GetDef's arguments are pushed before the manager is fetched; the thing def is read into esi after GetDef; `return`
// of the sub-def handle folds the result's add-ref with the local's release (`inc edx ; mov eax,edx ; dec eax`), the miss paths store
// a null result, the thing def is released last. The headers' noinline GetDef / GetSubDef bodies let VC7.1 see that the callees keep
// no pointer to the handles (reusing the one frame slot). Twin: GetHairCardDef 0x005D9A10.
// Name fix (agentNF10 2026-11-11): moved off the local CDefPointeeBase / CDefPointer / CDefClassBase / CThingDef / CDefinitionManager /
// CGameDefinitionManager stand-ins (and their same-TU stand-in bodies) onto the shared headers (engine/CTCInventoryItem.h
// FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS, CTattooDef.h FABLE_RETAIL_CTATTOODEF / _TYPENAME). The thing def handle is CDefPointer<const CThingBaseDef>, so
// 0x0042AF3C binds ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z (was GetDef<CThingDef>).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
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
#define FABLE_RETAIL_CTATTOODEF
#define FABLE_RETAIL_CTATTOODEF_TYPENAME
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTattooDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"

CDefPointer<const CTattooDef> __fastcall CTCInventoryItem::GetTattooCardDef(long def_index)
{
    CDefPointer<const CThingBaseDef> def;
    if (CGameDefinitionManager::Get().GetDef(def_index, &def)) {
        CDefPointer<const CTattooDef> card_def;
        if (def->GetSubDef(&card_def))
            return card_def;
    }
    return CDefPointer<const CTattooDef>();
}
