// CDefinitionManager::GetDef<CSpecialAbilitiesThunderLightningStormDef>(long, CDefPointer<const CSpecialAbilitiesThunderLightningStormDef>*) const --
// the def of a global index: GetPDefFromGlobalIndex, then SetDynamicCast into the out handle on a hit
// Leaf: ??$GetDef@VCSpecialAbilitiesThunderLightningStormDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSpecialAbilitiesThunderLightningStormDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008E31D0, 108 bytes, `ret 8`. Callee: CDefinitionManager::GetPDefFromGlobalIndex 0x009AD9E0.
// NF72 (round 7, q10862): moved off its local CDefinitionManager / CDefPointer / CDefBase stand-in (CDefinitionManager::GetDef_008e31d0) onto
//   the shared engine/CDefinitionManager.h body FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY with engine/CDefPointer.h
//   _SETDYNAMICCAST_REREAD: the text of the landed sibling instantiation 0x007F3BB0 (GetDef<CLightningDef>, NF39), instantiated for
//   CSpecialAbilitiesThunderLightningStormDef. Evidence: Ego_r.pdb + FableWin.pdb carry
//   ??$GetDef@VCSpecialAbilitiesThunderLightningStormDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSpecialAbilitiesThunderLightningStormDef@@@@@Z
//   (egor_pdb_names.tsv); its retail callers CAIStateGroup_Thunder::Update 0x0096C4F0 (ported, NF67) and
//   CModeThunderLightningStorm::OnActivate 0x008DD6E0 already bind this symbol.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CDEFSTRINGTABLE
#include "fable_string.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CSpecialAbilitiesThunderLightningStormDef.h"

template bool CDefinitionManager::GetDef<CSpecialAbilitiesThunderLightningStormDef>(long index, CDefPointer<const CSpecialAbilitiesThunderLightningStormDef>* out) const;
