// CResourceList::UnloadNextResource(unsigned long minimumSize) -- try to unload the least-recently-used resource (if it has been idle for UnloadDelay frames and is big enough), then rotate it to the most-recently-used end
// Grade: DIFFER(155v149) (retry1) -- with the landed ListRemove 0x009FBFB0 body defined first (noinline) the call site and everything up to the inlined ListInsert match; residue: retail copies the inserted prev into ecx (`mov ecx,eax ; mov [ecx+10h],esi`, then `mov edx,[esi+10h]`) and increments UnloadedThisFrame in memory (`inc dword ptr [edi+38h]`), ours stores through eax and does load/inc/store | tried: same-TU ListRemove body, PrevResource->NextResource spelling | next: ListInsert as a separate inline taking (prev, next) by a different parameter order, or the PDB ListInsert signature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC170, length 149 bytes (+11 int3 padding).
// engine/CResourceList.h: Head (CResource, +0x04; Head.PrevResource +0x10, Head.NextResource +0x14), ResourceCount
// +0x20, CurrentFrame +0x2C, UnloadDelay +0x34, UnloadedThisFrame +0x38. CResource (PDB): vptr, count word +0x04,
// ResourceList +0x08, PrevResource +0x0C, NextResource +0x10, ResourceSize +0x14, LastUsedFrame +0x18.
// Callee: CResource::ListRemove @ 0x009FBFB0 (private, out of line); CResource virtuals slot 1 (+0x04, the
// self-delete on the last release) and slot 2 (+0x08, Unload, returns bool).
// Shape: returns false for an empty list or when every resource was already tried this frame; the call is
// bracketed by an inline AddRef (`inc` through a register) and Release (`dec [esi+4] ; jne`, the delete path
// returns true at once); the rotate is the inlined private ListInsert(prev, next).
// Model: local model (the generated header holds Head as bytes and the vptr as a plain word).
struct CResourceList;

struct CResource {
    virtual ~CResource();
    virtual void DeleteThis();                       // slot 1 (+0x04)
    virtual bool Unload();                           // slot 2 (+0x08)

    void AddRef() { ++RefCount; }
    void Release() { if (--RefCount == 0) DeleteThis(); }
    void ListRemove();                               // 0x009FBFB0
    void ListInsert(CResource* prev, CResource* next)
    {
        NextResource = next;
        PrevResource = prev;
        prev->NextResource = this;
        NextResource->PrevResource = this;
    }

    long            RefCount;                        // +0x04
    CResourceList*  ResourceList;                    // +0x08
    CResource*      PrevResource;                    // +0x0C
    CResource*      NextResource;                    // +0x10
    unsigned long   ResourceSize;                    // +0x14
    unsigned long   LastUsedFrame;                   // +0x18
};

struct CResourceList {
    virtual ~CResourceList();
    bool UnloadNextResource(unsigned long minimumSize);

    CResource     Head;                              // +0x04
    unsigned long ResourceCount;                     // +0x20
    long          AllocatedMemory;                   // +0x24
    long          MaximumMemory;                     // +0x28
    unsigned long CurrentFrame;                      // +0x2C
    unsigned long DebugStatsFrame;                   // +0x30
    unsigned long UnloadDelay;                       // +0x34
    unsigned long UnloadedThisFrame;                 // +0x38
};

// 0x009FBFB0, the landed CResource::ListRemove body: defined first so VC7.1 knows it touches only eax/edx.
__declspec(noinline) void CResource::ListRemove()
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

bool CResourceList::UnloadNextResource(unsigned long minimumSize)
{
    CResource* resource = Head.NextResource;
    if (resource == &Head || UnloadedThisFrame >= ResourceCount)
        return false;

    bool unloaded = false;
    if (CurrentFrame - resource->LastUsedFrame >= UnloadDelay && resource->ResourceSize >= minimumSize)
    {
        resource->AddRef();
        unloaded = resource->Unload();
        if (--resource->RefCount == 0)
        {
            resource->DeleteThis();
            return true;
        }
    }
    if (resource->ResourceList == this)
    {
        resource->ListRemove();
        resource->ListInsert(Head.PrevResource, &Head);
        if (unloaded)
            resource->LastUsedFrame = CurrentFrame;
        ++UnloadedThisFrame;
    }
    return true;
}
