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
// ---- harness ----
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static CResourceList g_list;
static CResource* Head() { return reinterpret_cast<CResource*>(g_list.Head); }
static void Append(CResource* r)        // CResourceList::AddResource's links: before the head sentinel
{
    r->ResourceList = &g_list;
    r->PrevResource = Head()->PrevResource;
    r->NextResource = Head();
    r->PrevResource->NextResource = r;
    r->NextResource->PrevResource = r;
}
static bool Order(CResource** want, int n)
{
    CResource* p = Head()->NextResource;
    for (int i = 0; i < n; ++i, p = p->NextResource) {
        if (p != want[i] || p->NextResource->PrevResource != p) return false;
    }
    return p == Head() && Head()->PrevResource == want[n - 1];
}
int main()
{
    memset(&g_list, 0, sizeof g_list);
    Head()->PrevResource = Head(); Head()->NextResource = Head();
    CResource_Methods a, b, c;
    memset(&a, 0, sizeof a); memset(&b, 0, sizeof b); memset(&c, 0, sizeof c);
    Append(&a); Append(&b); Append(&c);
    a.SetAsMRU();                               // oldest moves to the most-recently-used end
    CResource* w1[3] = { &b, &c, &a };
    CHECK(Order(w1, 3));
    c.SetAsMRU();
    CResource* w2[3] = { &b, &a, &c };
    CHECK(Order(w2, 3));
    c.SetAsMRU();                               // already MRU: unchanged
    CHECK(Order(w2, 3));
    CResource_Methods lone; memset(&lone, 0, sizeof lone);
    lone.SetAsMRU();                            // not in a list: untouched
    CHECK(lone.PrevResource == 0 && lone.NextResource == 0);
    CHECK(Order(w2, 3));
    if (fails == 0) printf("009fc4a0_TEST PASS\n");
    return fails;
}