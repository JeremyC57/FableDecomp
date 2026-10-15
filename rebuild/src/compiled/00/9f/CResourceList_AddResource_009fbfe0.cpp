// CResourceList::AddResource(CResource* resource) -- take ownership of a resource: point it at this list, stamp it with the current frame, link it before the head sentinel (the most-recently-used end), count it and add its size to the allocated total
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBFE0, length 49 bytes (+15 int3 padding).
// Retail bytes:  mov eax,[esp+4] ; mov [eax+8],ecx ; mov edx,[ecx+2Ch] ; mov [eax+18h],edx ; mov edx,[ecx+10h] ; mov [eax+0Ch],edx ;
//                lea edx,[ecx+4] ; mov [eax+10h],edx ; mov edx,[eax+0Ch] ; mov [edx+10h],eax ; mov edx,[eax+10h] ; mov [edx+0Ch],eax ;
//                inc dword ptr [ecx+20h] ; mov eax,[eax+14h] ; add [ecx+24h],eax ; ret 4
// engine/CResourceList.h: +4 Head (a CResource sentinel: Head.PrevResource at +0x10), +0x20 ResourceCount, +0x24
// AllocatedMemory, +0x2C CurrentFrame; engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource,
// +0x14 ResourceSize, +0x18 LastUsedFrame. CResourceBank::AddResource (0x009FC380) / AddResourceToLoadingQueue
// (0x009FC3D0) are this body inlined on the bank's two lists.
// Shape: `inc`/`add` on memory = `#pragma optimize("s", on)` (the speed form loads both counters first); the head
// sentinel is reached through an inline accessor at each use, so Head.PrevResource is read straight off the list
// ([ecx+10h]) before `lea edx,[ecx+4]` (a cached `head` pointer local makes the lane form the address first and hold the
// link in esi); the neighbour links are written through the resource's members (re-read after the stores).
#pragma optimize("s", on)
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    CResource* GetHead() { return reinterpret_cast<CResource*>(this->Head); }
    void AddResource(CResource* resource);
};

void CResourceList_Methods::AddResource(CResource* resource)
{
    resource->ResourceList = this;
    resource->LastUsedFrame = this->CurrentFrame;
    resource->PrevResource = GetHead()->PrevResource;
    resource->NextResource = GetHead();
    resource->PrevResource->NextResource = resource;
    resource->NextResource->PrevResource = resource;
    this->ResourceCount++;
    this->AllocatedMemory += resource->ResourceSize;
}