// CResource::ListRemove()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBFB0, oracle length 47 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[ecx+10h] ; test eax,eax ; je R ; mov edx,[ecx+0Ch] ; cmp [edx+10h],ecx ; jne C ; cmp [eax+0Ch],ecx ; jne C ; mov [edx+10h],eax ; mov eax,[ecx+10h] ; mov edx,[ecx+0Ch] ; mov [eax+0Ch],edx ; C: mov dword ptr [ecx+0Ch],0 ; mov dword ptr [ecx+10h],0 ; R: ret
// engine/CResource.h (PDB): +0x0C PrevResource, +0x10 NextResource (intrusive doubly linked list
// with a sentinel, so a linked resource always has a next). Unlinks this resource: the neighbours
// are re-joined only if they still point back at this resource (a consistency guard), then both
// links are cleared.
#include "engine/CResource.h"

struct CResource_Methods : CResource {
    void ListRemove();
};

void CResource_Methods::ListRemove()
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
