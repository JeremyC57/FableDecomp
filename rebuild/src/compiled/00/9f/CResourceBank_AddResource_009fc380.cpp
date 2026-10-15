// CResourceBank::AddResource(CResource* resource) -- hand a resource to the bank's resource list (CResourceList::AddResource inlined on the list at +4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC380, length 52 bytes (+12 int3 padding).
// Retail bytes:  lea eax,[ecx+4] ; mov ecx,[esp+4] ; mov [ecx+8],eax ; mov edx,[eax+2Ch] ; mov [ecx+18h],edx ; mov edx,[eax+10h] ;
//                mov [ecx+0Ch],edx ; lea edx,[eax+4] ; mov [ecx+10h],edx ; mov edx,[ecx+0Ch] ; mov [edx+10h],ecx ; mov edx,[ecx+10h] ;
//                mov [edx+0Ch],ecx ; inc dword ptr [eax+20h] ; mov ecx,[ecx+14h] ; add [eax+24h],ecx ; ret 4
// PDB CResourceBank (see the landed CResourceBank::FrameEnd 0x009FC430): +0x04 CResourceList ResourceList, +0x40
// CResourceList LoadingQueue. engine/CResourceList.h: +4 Head (CResource sentinel), +0x20 ResourceCount, +0x24
// AllocatedMemory, +0x2C CurrentFrame; engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource,
// +0x14 ResourceSize, +0x18 LastUsedFrame. The list's AddResource (out of line at 0x009FBFE0) points the resource at
// the list, stamps the current frame, links it before the head sentinel (MRU end), counts it and adds its size.
// Shape: `inc`/`add` on memory = `#pragma optimize("s", on)`, under which the list's AddResource must be __forceinline
// (else it stays out of line); the head sentinel is reached through an inline accessor at each use, so Head.PrevResource
// is read straight off the list ([list+10h]) before the head address is formed (a cached `head` local makes the lane
// form the address first and hold the link in esi); the links are written through the resource's members.
#pragma optimize("s", on)
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    CResource* GetHead() { return reinterpret_cast<CResource*>(this->Head); }
    __forceinline void AddResource(CResource* resource)
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
};

struct CResourceBank_Retail {
    void*                 __vftable;        // +0x00
    CResourceList_Methods ResourceList;     // +0x04
    CResourceList_Methods LoadingQueue;     // +0x40
    void AddResource(CResource* resource);
};

void CResourceBank_Retail::AddResource(CResource* resource)
{
    this->ResourceList.AddResource(resource);
}