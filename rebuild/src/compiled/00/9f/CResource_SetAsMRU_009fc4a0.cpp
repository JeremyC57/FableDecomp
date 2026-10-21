// CResource::SetAsMRU() -- move a listed resource to the most-recently-used end: unlink it, then relink it right before its list's head sentinel
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC4A0, length 80 bytes.
// Retail bytes:  mov eax,[ecx+8] ; test eax,eax ; je R ; mov eax,[ecx+10h] ; test eax,eax ; je L ; (unlink as ListRemove) ;
//                L: mov eax,[ecx+8] ; mov edx,[eax+10h] ; add eax,4 ; mov [ecx+10h],eax ; mov [ecx+0Ch],edx ; mov eax,edx ;
//                mov [eax+10h],ecx ; mov edx,[ecx+10h] ; mov [edx+0Ch],ecx ; R: ret
// engine/CResource.h: +8 ResourceList, +0xC PrevResource, +0x10 NextResource; engine/CResourceList.h: +4 Head (the
// CResource sentinel, Head.PrevResource at +0x10). The list is kept oldest first from Head.NextResource
// (CResourceList::AddResource 0x009FBFE0 appends new resources before the head), so relinking before the head marks the
// resource most recently used. The twin 0x009FC450 relinks after the head (the LRU end).
// Shape: the landed ListRemove (0x009FBFB0) inlined, then the four links written out in CResourceList::AddResource's
// (0x009FBFE0) order: PrevResource = head->PrevResource, NextResource = head, then PrevResource->NextResource and
// NextResource->PrevResource through the members. The store-forwarded PrevResource gives retail's `mov eax,edx` copy.
// Tried: ListInsert(prev, next) and a next-first ListInsertBefore helper (both 78 bytes), a prev local (78).
#include "engine/CResource.h"
#include "engine/CResourceList.h"

// The unlink is the landed CResource::ListRemove (0x009FBFB0) body, inlined here.
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
    void SetAsMRU();
};

void CResource_Methods::SetAsMRU()
{
    if (this->ResourceList) {
        ListRemove();
        CResource* head = reinterpret_cast<CResource*>(this->ResourceList->Head);
        this->PrevResource = head->PrevResource;
        this->NextResource = head;
        this->PrevResource->NextResource = this;
        this->NextResource->PrevResource = this;
    }
}