// CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const CSoundThemeDef>* out) const -- the def of a type by global index
// Leaf: ??$GetDef@VCSoundThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCSoundThemeDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B25E0, 113 bytes, thiscall, `ret 0Ch`.
// Identity (proven): its callers (0x006B16B0, 0x006B1A20, 0x006B1D30, CAtmosProcess 0x006B1F30, 0x006BDC60, CTCSoundAtmosphereVillage 0x007F53B0) pass CCharString("SOUND_THEME"); Ego_r.pdb ??$GetDef@VCSoundThemeDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCSoundThemeDef@@@@@Z.
// Callees: CDefinitionManager::GetPDefFromClassIndex(const CCharString&, long) const 0x009ADE10 (CDefPointer by hidden
//   pointer), the def's virtual slot 1 (destroy on the last release).
// Shape (family M026, representative 0x0043FEB0): `if (index <= 0) return false; CDefPointer<const CDefClassBase> def =
//   GetPDefFromClassIndex(type_name, index); if (def.Object) { *out = def; return true; } return false;` -- the temporary
//   is homed in the dead `index` slot; operator= releases with `dec [ecx+4] ; jne`, the temporary's destructor through the
//   DecRef/GetRefCount accessors (`dec esi ; mov edx,esi ; test edx,edx`).
// Name (round 7, NF53): the callee at 0x009ADE10 is ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z
//   (Ego_r.pdb 0x00B05EA0 / FableWin.pdb 0x03061E20: the only (const CCharString&, long) lookup; GetPDefFromGlobalIndex takes the index
//   alone). Was a local CDefinitionManager / CDefPointer / CDefClassBase / CCharString model that called it GetPDefFromGlobalIndex; now
//   the header's by-class-index body (FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY with FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD,
//   the 113-byte form of 0x006B25E0) instantiated for CSoundThemeDef, as the landed 0x00451354.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#define FABLE_RETAIL_CSOUNDTHEMEDEF_PASS22
#include "engine/CSoundThemeDef.h"

template bool CDefinitionManager::GetDef<CSoundThemeDef>(const CCharString&, long, CDefPointer<const CSoundThemeDef>*) const;
