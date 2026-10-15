// CResource::SetAsLRU() -- move a listed resource to the least-recently-used end: unlink it, then relink it right after its list's head sentinel
// Leaf: SetAsLRU
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC450, length 78 bytes (+2 int3 padding).
// Retail bytes:  mov eax,[ecx+8] ; test eax,eax ; je R ; mov eax,[ecx+10h] ; test eax,eax ; je L ; mov edx,[ecx+0Ch] ;
//                cmp [edx+10h],ecx ; jne Z ; cmp [eax+0Ch],ecx ; jne Z ; mov [edx+10h],eax ; mov eax,[ecx+10h] ; mov edx,[ecx+0Ch] ;
//                mov [eax+0Ch],edx ; Z: mov dword ptr [ecx+0Ch],0 ; mov dword ptr [ecx+10h],0 ; L: mov eax,[ecx+8] ;
//                mov edx,[eax+14h] ; add eax,4 ; mov [ecx+0Ch],eax ; mov [ecx+10h],edx ; mov [eax+10h],ecx ; mov eax,[ecx+10h] ;
//                mov [eax+0Ch],ecx ; R: ret
// engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource; engine/CResourceList.h: +4 Head (the
// CResource sentinel, so Head.NextResource is +0x14). The list is kept oldest first from Head.NextResource
// (CResourceList::AddResource 0x009FBFE0 appends before the head, GetUnloadableMemory 0x009FC070 walks from
// Head.NextResource while resources are old enough), so linking right after the head makes this resource the first
// one to unload: the least recently used. Manifest label "SetAsMRU" is shared with the twin 0x009FC4A0, which links
// before the head (the MRU end); this body is named for what it does.
// Shape: the landed ListRemove (0x009FBFB0) and ListInsert (0x009FBF90) bodies inlined; ResourceList is re-read after
// the unlink.
#include "engine/CResource.h"
#include "engine/CResourceList.h"

// The list links are the landed CResource::ListRemove (0x009FBFB0) and ::ListInsert (0x009FBF90) bodies, inlined here.
struct CResource_Methods : CResource {
    void ListInsert(CResource* prev, CResource* next)
    {
        this->PrevResource = prev;
        this->NextResource = next;
        prev->NextResource = this;
        this->NextResource->PrevResource = this;
    }
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
    void SetAsLRU();
};

void CResource_Methods::SetAsLRU()
{
    if (this->ResourceList) {
        ListRemove();
        CResource* head = reinterpret_cast<CResource*>(this->ResourceList->Head);
        ListInsert(head, head->NextResource);
    }
}