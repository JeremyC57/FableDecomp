// CDefinitionManager::GetNoInstantiatedDefsOfType(const CCharString&) const -- how many defs of the named class are instantiated: the
// class record's GlobalIndices size, 0 when the class is unknown
// Leaf: ?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AD3B0.
// NF39 (2026-11-16): moved off its local CDefinitionManager / CDefClassInfo stand-in (GetNumberOfDefsOfType / GetClassInfo) onto the shared
//   engine/CDefinitionManager.h (_NOINSTANTIATED names 0x009AD3B0; overlay opt-in FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES for
//   the const GetPDefClassInfo 0x009AD2E0): Ego_r.pdb + FableWin.pdb ?GetNoInstantiatedDefsOfType@CDefinitionManager@@QBEJABVCCharString@@@Z
//   (Ego_r donor order, see 0x009AD390). Include chain from the landed 0x009ADC90.
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
#define FABLE_RETAIL_CDEFINITIONMANAGER_NOINSTANTIATED
#define FABLE_RETAIL_CDEFINITIONMANAGER_CLASSINFO_QUERIES
#include "fable_string.h"
#include "engine/CDefString.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"

long CDefinitionManager::GetNoInstantiatedDefsOfType(const CCharString& type_name) const
{
    const CDefClassInfo* info = GetPDefClassInfo(type_name);
    if (info)
        return info->GlobalIndices.size();
    return 0;
}
