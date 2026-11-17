// CDefinitionManager::GetPDefFromClassIndex(const CCharString&, long) const -- map the class name to its class info, take the global index at `index` of that class's def list, load the def and return a counted pointer to it
// Leaf: ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009ADE10, length 62 bytes.
// Callees: CDefinitionManager::GetPDefClassInfo(const CCharString&) const @ 0x009AD2E0, CDefinitionManager::LoadDef(unsigned long) @ 0x009AD6E0.
// The CDefPointer<CDefClassBase> result is built through the hidden pointer (`ret 0Ch`: result + name + index).
// PDB CDefinitionManager: InstantiatedDefs at +188, retail +0xa4; CInstantiatedDefInfo: PDef at +0xc; the class info
// record keeps its def-index array pointer at +8. Shape: the copy constructor's increment is the single
// `inc dword ptr [ecx+4]` of an inline AddRef helper, scheduled after the pops.
// Name (round 7, NF53): Ego_r.pdb 0x00B05EA0 / FableWin.pdb 0x03061E20
//   ?GetPDefFromClassIndex@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@J@Z, the only CDefinitionManager
//   lookup taking (const CCharString&, long) (GetPDefFromGlobalIndex takes the global index alone, retail 0x009AD9E0 / 0x009ADA10); the
//   landed GetDef rows (0x00451354, 0x006B25E0 ...) call 0x009ADE10 so. Was GetPDefFromGlobalIndex(const unsigned long&, long) over a
//   local CDefinitionManager / CDefPointer model; now engine/CDefinitionManager.h (the class-info records of _INSTANTIATEDEF, the const
//   GetPDefClassInfo of _CLASSINFO_QUERIES, LoadDef of _INSTANTIATESUBDEF), as the landed 0x009AD390.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_CTOR_SETSTRING
#define FABLE_RETAIL_CSYMBOLMAP
#define FABLE_RETAIL_CSYMBOLMAP_ADDSYMBOLDIRTY
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATEDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES
#define FABLE_RETAIL_CDEFINITIONMANAGER_INSTANTIATESUBDEF
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"

CDefPointer<const CDefClassBase> CDefinitionManager::GetPDefFromClassIndex(const CCharString& class_name, long class_index) const
{
    const CDefClassInfo* info = GetPDefClassInfo(class_name);
    unsigned long global_index = info->GlobalIndices.begin()[class_index];
    LoadDef(global_index);
    return InstantiatedDefs[global_index]->PDef;
}
