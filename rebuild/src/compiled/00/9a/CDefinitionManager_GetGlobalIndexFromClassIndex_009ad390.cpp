// CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString&, long) const -- the global def index of the class's class_index-th
// instance: GetPDefClassInfo(class_name) 0x009AD2E0, then its GlobalIndices[class_index] (`mov ecx,[eax+8] ; mov eax,[ecx+edx*4] ; ret 8`)
// Leaf: ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD390.
// NF39 (2026-11-16): moved off its free __fastcall stand-in (COptimisedPrimitive ActualToUniqueIndex / sub_9ad2e0) onto the shared
//   engine/CDefinitionManager.h (overlay opt-in FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES over _INSTANTIATEDEF's CDefClassInfo):
//   (the header already declares the public member); Ego_r.pdb + FableWin.pdb ?GetGlobalIndexFromClassIndex@CDefinitionManager@@QBEJABVCCharString@@J@Z, Ego_r donor order
//   GetPDefClassInfo (const) 0x00B05200 / GetGlobalIndexFromClassIndex 0x00B05260 / GetNoInstantiatedDefsOfType 0x00B052D0 = retail
//   0x009AD2E0 / 0x009AD390 / 0x009AD3B0; manifest label GetGlobalIndexFromClassIndex. Include chain from the landed 0x009ADC90.
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
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"

long CDefinitionManager::GetGlobalIndexFromClassIndex(const CCharString& class_name, long class_index) const
{
    const CDefClassInfo* info = GetPDefClassInfo(class_name);
    return info->GlobalIndices.begin()[class_index];
}
