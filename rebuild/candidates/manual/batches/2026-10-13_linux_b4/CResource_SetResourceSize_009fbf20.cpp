// CResource::SetResourceSize(unsigned long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBF20, oracle length 46 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+8] ; test eax,eax ; je N ; mov edx,[ecx+14h] ; push esi ; mov esi,[eax+24h] ; sub esi,edx ; mov edx,[esp+8] ; mov [eax+24h],esi ; mov eax,[ecx+8] ; add [eax+24h],edx ; mov [ecx+14h],edx ; pop esi ; ret 4 ; N: mov eax,[esp+4] ; mov [ecx+14h],eax ; ret 4
// engine/CResource.h (PDB): +0x08 ResourceList, +0x14 ResourceSize; engine/CResourceList.h:
// +0x24 AllocatedMemory. Re-sizing a resource that sits in a list moves the list's allocated
// total from the old size to the new one; the resource then records the new size either way.
#include "engine/CResource.h"
#include "engine/CResourceList.h"

struct CResource_Methods : CResource {
    void SetResourceSize(unsigned long size);
};

void CResource_Methods::SetResourceSize(unsigned long size)
{
    CResourceList* list = this->ResourceList;
    if (list) {
        list->AllocatedMemory -= this->ResourceSize;
        this->ResourceList->AllocatedMemory += size;
    }
    this->ResourceSize = size;
}
