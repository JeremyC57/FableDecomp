// CDefinitionManager::GetDef<CLightningDef>(long, CDefPointer<const CLightningDef>*) const -- the def of a global index: GetPDefFromGlobalIndex,
// then SetDynamicCast into the out handle on a hit
// Leaf: ??$GetDef@VCLightningDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCLightningDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F3BB0.
// NF39 (2026-11-16): moved off its local CDefinitionManager / CDefPointer stand-in (CDefinitionManager::GetDef_007f3bb0) onto the shared
//   engine/CDefinitionManager.h body FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (FableWin.pdb 0x01CCD240 text: `index > 0`,
//   GetPDefFromGlobalIndex, SetDynamicCast on a hit; with engine/CDefPointer.h _SETDYNAMICCAST_REREAD, the re-read proven on the landed
//   GetDef<CConfigOptionsDefaultsDef> 0x00410890), instantiated for CLightningDef: FableWin.pdb
//   ??$GetDef@VCLightningDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCLightningDef@@@@@Z (LABELS.tsv); its landed callers
//   0x007F26B0 / 0x007F2DC0 already bind this symbol. Defines from the landed 0x007F2DC0 (the same opt-in).
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
#include "engine/CLightningDef.h"

template bool CDefinitionManager::GetDef<CLightningDef>(long index, CDefPointer<const CLightningDef>* out) const;
