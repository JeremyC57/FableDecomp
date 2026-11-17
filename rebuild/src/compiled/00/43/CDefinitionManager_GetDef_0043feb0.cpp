// CDefinitionManager::GetDef(const CCharString& type_name, long index, CDefPointer<const CGlobalDef>* out) const -- the def of a type by global index
// Leaf: ??$GetDef@VCGlobalDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCGlobalDef@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0043FEB0, 113 bytes, thiscall, `ret 0Ch`.
// Identity (proven): its only caller CGameDefinitionManager::InitAndCompile 0x0044C72B passes CCharString("GLOBAL"), index 1 and the manager's +0xD0 pointer; Ego_r.pdb ??$GetDef@VCGlobalDef@@@CDefinitionManager@@QBE_NABVCCharString@@JPAV?$CDefPointer@$$CBVCGlobalDef@@@@@Z.
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
//   the 113-byte form of 0x006B25E0) instantiated for CGlobalDef, as the landed 0x00451354.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_PASS22_GETDEF_INDEX_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD
#include "fable_string.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGlobalDef.h"

template bool CDefinitionManager::GetDef<CGlobalDef>(const CCharString&, long, CDefPointer<const CGlobalDef>*) const;
