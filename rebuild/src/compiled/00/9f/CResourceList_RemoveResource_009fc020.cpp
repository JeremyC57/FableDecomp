// CResourceList::RemoveResource(CResource* resource) -- release a resource from this list: detach it from the list, unlink it from its neighbours, uncount it and take its size off the allocated total
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC020, length 80 bytes.
// Retail bytes:  mov eax,[esp+4] ; mov edx,[eax+10h] ; test edx,edx ; push esi ; mov dword ptr [eax+8],0 ; je S ; mov esi,[eax+0Ch] ;
//                cmp [esi+10h],eax ; jne Z ; cmp [edx+0Ch],eax ; jne Z ; mov [esi+10h],edx ; mov edx,[eax+10h] ; mov esi,[eax+0Ch] ;
//                mov [edx+0Ch],esi ; Z: mov dword ptr [eax+0Ch],0 ; mov dword ptr [eax+10h],0 ; S: mov esi,[ecx+20h] ;
//                mov edx,[ecx+24h] ; dec esi ; mov [ecx+20h],esi ; mov eax,[eax+14h] ; sub edx,eax ; mov [ecx+24h],edx ; pop esi ; ret 4
// Manifest label "CResourceBank::RemoveResource" names the wrong class: `this` carries ResourceCount (+0x20) and
// AllocatedMemory (+0x24), the CResourceList members (engine/CResourceList.h), and the body mirrors
// CResourceList::AddResource (0x009FBFE0). engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource,
// +0x14 ResourceSize.
// Shape: the landed CResource::ListRemove (0x009FBFB0) inlined after the ResourceList clear (the clear is scheduled
// between the NextResource test and its branch); the two counters are updated in the speed form (both loaded first).
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResource_Methods : CResource {
    void ListRemove()
    {
        if (this->NextResource) {
            if (this->PrevResource->NextResource == this && this->NextResource->PrevResource == this) {
                this->PrevResource->NextResource = this->NextResource;
                this->NextResource->PrevResource = this->PrevResource;
            }
            this->PrevResource = 0;
            this->NextResource = 0;
        }
    }
};

struct CResourceList_Methods : CResourceList {
    void RemoveResource(CResource* resource);
};

void CResourceList_Methods::RemoveResource(CResource* resource)
{
    resource->ResourceList = 0;
    static_cast<CResource_Methods*>(resource)->ListRemove();
    this->ResourceCount--;
    this->AllocatedMemory -= resource->ResourceSize;
}