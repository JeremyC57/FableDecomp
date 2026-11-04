// CResourceBank::RemoveResource(CResource& resource) -- take a resource out of the bank's resource list
// Leaf: ?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FC3C0, 8 bytes: `add ecx,4 ; jmp 0x009FC020` (ResourceList.RemoveResource(resource), tail call).
// Identity (two sources): (1) Ego_r.pdb / FableWin.pdb ?RemoveResource@CResourceBank@@QAEXAAVCResource@@@Z, in Ego_r donor order between
//   AddResource and AddToLoadingQueue exactly as retail lays the bank out (AddResource 0x009FC380, this row, AddResourceToLoadingQueue
//   0x009FC3D0, ProcessLoadingQueue 0x009FC410, FrameEnd 0x009FC430), forwarding to the member ResourceList (+0x04, engine/CResourceBank.h)
//   through the bound ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z 0x009FC020; (2) its caller ~CEngineSurfaceCompositionManager
//   0x00B2D231 unhooks each cached surface from its own CResourceBank base. The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ?RemoveResource@CResourceList@@QAEXAAVCResource@@@Z 0x009FC020.
// Header: engine/CResourceBank.h, engine/CResourceList.h (FABLE_RETAIL_CRESOURCELIST, _REMOVERESOURCE new).
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCELIST_REMOVERESOURCE
#include "engine/CResourceList.h"
#include "engine/CResourceBank.h"
void CResourceBank::RemoveResource(CResource& resource)
{
    ResourceList.RemoveResource(resource);
}