// CTCInventoryItem::GetHairCardDef(long defIndex) -- static: fetch the thing def for defIndex from the game definition manager and return its CHairCardDef sub-definition as a counted pointer (null when either lookup fails)
// Leaf: ?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z
// Retail: Fable.exe @ 0x005D9A10, 137 bytes (+7 int3). Ego_r.pdb `?GetHairCardDef@CTCInventoryItem@@SI?AV?$CDefPointer@$$CBVCHairCardDef@@@@J@Z`
// (static __fastcall: the hidden CDefPointer result in ecx, defIndex in edx; `ret`). Twin: GetTattooCardDef 0x005D9AA0.
// Instance twin: GetHairCardDef() const 0x005D9430 (the owner thing's def instead of a manager lookup).
// Callees: CGameDefinitionManager::Get() @ 0x0044C6B0 (static __fastcall), CDefinitionManager::GetDef<CThingBaseDef> @ 0x0042AF3C,
// CDefClassBase::GetSubDef<CHairCardDef> @ 0x005DA180, the defs' virtual slot 1 on the last release.
// Shape: speed TU; GetDef's arguments are pushed before the manager is fetched; the thing def is read into esi after GetDef; `return`
// of the sub-def handle folds the result's add-ref with the local's release (`inc edx ; mov eax,edx ; dec eax`), the miss paths store
// a null result, the thing def is released last. The headers' noinline GetDef / GetSubDef bodies let VC7.1 see that the callees keep
// no pointer to the handles (reusing the one frame slot). Twin: GetTattooCardDef 0x005D9AA0.
// Name fix (agentNF10 2026-11-11): moved off the local CDefPointeeBase / CDefPointer / CDefClassBase / CThingDef / CDefinitionManager /
// CGameDefinitionManager stand-ins (and their same-TU stand-in bodies) onto the shared headers (engine/CTCInventoryItem.h
// FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS, CHairCardDef.h FABLE_RETAIL_CHAIRCARDDEF / _TYPENAME). The thing def handle is CDefPointer<const CThingBaseDef>, so
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
#define FABLE_RETAIL_CHAIRCARDDEF
#define FABLE_RETAIL_CHAIRCARDDEF_TYPENAME
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCINVENTORYITEM_CARDDEFS
#include "fable_string.h"
#include "engine/CThingBaseDef.h"
#include "engine/CHairCardDef.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCInventoryItem.h"

CDefPointer<const CHairCardDef> __fastcall CTCInventoryItem::GetHairCardDef(long def_index)
{
    CDefPointer<const CThingBaseDef> def;
    if (CGameDefinitionManager::Get().GetDef(def_index, &def)) {
        CDefPointer<const CHairCardDef> card_def;
        if (def->GetSubDef(&card_def))
            return card_def;
    }
    return CDefPointer<const CHairCardDef>();
}
