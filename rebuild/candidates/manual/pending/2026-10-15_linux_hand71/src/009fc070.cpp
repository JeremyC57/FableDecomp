// CResourceList::GetUnloadableMemory(unsigned long minUnusedFrames) -- walk the list from the oldest resource and total the unloadable memory of every resource unused for at least `minUnusedFrames` frames, stopping at the first more recent one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC070, length 59 bytes.
// Retail bytes:  push ebx ; push ebp ; push esi ; push edi ; mov edi,ecx ; mov esi,[edi+14h] ; lea ebp,[edi+4] ; xor ebx,ebx ;
//                cmp esi,ebp ; je E ; L: mov eax,[edi+2Ch] ; mov edx,[esi+18h] ; mov ecx,[esp+14h] ; sub eax,edx ; cmp eax,ecx ;
//                jb E ; mov edx,[esi] ; mov ecx,esi ; call [edx+0Ch] ; mov esi,[esi+10h] ; add ebx,eax ; cmp esi,ebp ; jne L ;
//                E: pop edi ; pop esi ; pop ebp ; mov eax,ebx ; pop ebx ; ret 4
// Shape: a top-tested `for` over Head.NextResource .. &Head (rotated: one entry test, bottom test); CurrentFrame and the
// argument are re-read every iteration (the virtual call may change them); the age test is unsigned (`jb`).
// Local polymorphic models (the generated engine headers flatten the vptrs into plain words); member names and offsets
// are the PDB ones from engine/CResource.h and engine/CResourceList.h, the +4 word of CResource is IVRefCount of its
// CIVCountedPointeeBase base. CResource vtable 0x012354A4: slot 3 (+0x0C) GetUnloadableMemory (0x009FBEF0, returns
// ResourceSize), slot 9 (+0x24) ResetDebugMemoryInUse (0x009FBF10).
struct CResourceList;

struct CResource {                                   // 0x1C bytes
    virtual ~CResource();                            // slot 0
    virtual void v01();
    virtual void v02();
    virtual unsigned long GetUnloadableMemory();     // slot 3 (+0x0C)
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void ResetDebugMemoryInUse();            // slot 9 (+0x24)
    long           IVRefCount;                       // +0x04 (CIVCountedPointeeBase)
    CResourceList* ResourceList;                     // +0x08
    CResource*     PrevResource;                     // +0x0C
    CResource*     NextResource;                     // +0x10
    unsigned long  ResourceSize;                     // +0x14
    unsigned long  LastUsedFrame;                    // +0x18
};

struct CResourceList {                               // vtable 0x0129C808, 0x3C bytes
    virtual ~CResourceList();
    CResource     Head;                              // +0x04 (sentinel)
    unsigned long ResourceCount;                     // +0x20
    long          AllocatedMemory;                   // +0x24
    long          MaximumMemory;                     // +0x28
    unsigned long CurrentFrame;                      // +0x2C
    unsigned long DebugStatsFrame;                   // +0x30
    unsigned long UnloadDelay;                       // +0x34
    unsigned long UnloadedThisFrame;                 // +0x38
    unsigned long GetUnloadableMemory(unsigned long minUnusedFrames);
};

unsigned long CResourceList::GetUnloadableMemory(unsigned long minUnusedFrames)
{
    unsigned long total = 0;
    for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource) {
        if (CurrentFrame - resource->LastUsedFrame < minUnusedFrames)
            break;
        total += resource->GetUnloadableMemory();
    }
    return total;
}
