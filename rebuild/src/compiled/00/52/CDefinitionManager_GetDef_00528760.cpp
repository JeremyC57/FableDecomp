// CDefinitionManager::GetDef<CThingBaseDef>(const CCharString& name, CDefPointer<const CThingBaseDef>* out) const -- look the def up by its
//   instantiation name; on a hit assign it to *out and return true, else false
// Leaf: ??$GetDef@VCThingBaseDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCThingBaseDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00528760, thiscall, `ret 8`: GetPDefFromInstantiationName 0x009ADA40 into a temporary, the inline
//   CDefPointer assignment (release the old def, add-ref the new one), the temporary released.
// Identity (two sources): Ego_r.pdb / FableWin.pdb name the by-name template ??$GetDef@V..@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBV..@@@@@Z
//   (engine/CDefinitionManager.h); the landed callers
//   CThingManager::LoadSingleThing 0x0051FD80 and CTCObjectAugmentations::OnSerialise 0x00767200 bind this CThingBaseDef instance, and
//   CTCDRumble::Create 0x007F1B70 looks up "THING_RUMBLE" through it into a CThingBaseDef handle.
// Was: the non-template stand-in CDefinitionManager::GetDef_00528760 over local CDefBase / CDefPointer models
//   (rebuild/src/compiled/00/52/CDefinitionManager_GetDef00528760_00528760.cpp).
// Form: an explicit instantiation of the header's by-name template text FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE (`def.Get()`
//   test, SetDynamicCast on a hit) with FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD (retail re-reads the source pointee after the
//   Release, `mov eax,[esp+8]` at 0x00528790 / 0x005287A3), the recipe of the landed GetDef<CConfigOptionsDefaultsDef> 0x00410890. The
//   _NAME_BODY text (`!def.IsNull()` / `*out = def`) caches the pointee in esi and keeps the temporary's null test (DIFFER 112v104).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGBASEDEF
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"
template bool CDefinitionManager::GetDef<CThingBaseDef>(const CCharString&, CDefPointer<const CThingBaseDef>*) const;
