// Flags: 
// CResourceList::AddResource(CResource&)
// Leaf: ?AddResource@CResourceList@@QAEXAAVCResource@@@Z
// Ego_r 00B6BE90 and FableWin 031777D0. The list takes a reference.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_TEXTUREPOOL_RESIZE_PASS30
#include "engine/CResourceList.h"

void CResourceList::AddResource(CResource& resource)
{
    resource.ResourceList = this;
    resource.LastUsedFrame = CurrentFrame;
    resource.PrevResource = Head.PrevResource;
    resource.NextResource = &Head;
    resource.PrevResource->NextResource = &resource;
    resource.NextResource->PrevResource = &resource;
    ResourceCount++;
    AllocatedMemory += resource.ResourceSize;
}
