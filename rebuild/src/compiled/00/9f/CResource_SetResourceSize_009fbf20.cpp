// CResource::SetResourceSize(unsigned long) -- re-size a resource: when it sits in a resource list, move the list's allocated total from
// the old size to the new one; record the new size either way
// Leaf: ?SetResourceSize@CResource@@QAEXK@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FBF20, 46 bytes (`ret 4`).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?SetResourceSize@CResource@@QAEXK@Z; engine/CResource.h (retail, ResourceList +0x08,
//   ResourceSize +0x14) names it at 0x009FBF20 and CResourceList.h AllocatedMemory +0x24 is the word adjusted here.
// Name fix (agentNF5 2026-11-10): was the derived stand-in `CResource_Methods::SetResourceSize`; now the shared retail CResource
//   (FABLE_RETAIL_DEF_CLASS_BASE + FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS + FABLE_RETAIL_CGRAPHICINFOFRAME_FREE declare it);
//   CResourceList stays the default flattened layout (the retail branch keeps AllocatedMemory private).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#include "engine/CResource.h"
#include "engine/CResourceList.h"
void CResource::SetResourceSize(unsigned long size)
{
    CResourceList* list = ResourceList;
    if (list) {
        list->AllocatedMemory -= ResourceSize;
        ResourceList->AllocatedMemory += size;
    }
    ResourceSize = size;
}
