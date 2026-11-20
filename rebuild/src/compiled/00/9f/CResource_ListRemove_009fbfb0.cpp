// CResource::ListRemove()
// Leaf: ?ListRemove@CResource@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FBFB0, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov eax,[ecx+10h] ; test eax,eax ; je R ; mov edx,[ecx+0Ch] ; cmp [edx+10h],ecx ; jne C ; cmp [eax+0Ch],ecx ; jne C ; mov [edx+10h],eax ; mov eax,[ecx+10h] ; mov edx,[ecx+0Ch] ; mov [eax+0Ch],edx ; C: mov dword ptr [ecx+0Ch],0 ; mov dword ptr [ecx+10h],0 ; R: ret
// engine/CResource.h (PDB): +0x0C PrevResource, +0x10 NextResource (intrusive doubly linked list
// with a sentinel, so a linked resource always has a next). Unlinks this resource: the neighbours
// are re-joined only if they still point back at this resource (a consistency guard), then both
// links are cleared.
// Name (NF143, q4942): the private member ?ListRemove@CResource@@AAEXXZ (Ego_r.pdb 0x00B6BE60 + FableWin.pdb 0x031772B0; donor order
//   ListInsert, ListRemove = retail 0x009FBF90, 0x009FBFB0), declared by engine/CResource.h (FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE);
//   was the stand-in CResource_Methods::ListRemove.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#include "engine/CResource.h"

void CResource::ListRemove()
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
