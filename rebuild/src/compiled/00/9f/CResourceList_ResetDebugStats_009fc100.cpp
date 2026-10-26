// CResourceList::ResetDebugStats() -- reset the debug memory statistic of every resource in the list, then record the current frame as the debug-stats frame
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC100, length 50 bytes.
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,ecx ; mov esi,[edi+14h] ; lea ebx,[edi+4] ; cmp esi,ebx ; je E ; nop ;
//                L: mov eax,[esi] ; mov ecx,esi ; call [eax+24h] ; mov esi,[esi+10h] ; cmp esi,ebx ; jne L ;
//                mov ecx,[edi+2Ch] ; mov [edi+30h],ecx ; pop edi ; pop esi ; pop ebx ; ret ;
//                E: mov edx,[edi+2Ch] ; mov [edi+30h],edx ; pop edi ; pop esi ; pop ebx ; ret
// Shape: a top-tested `for` over Head.NextResource .. &Head calling virtual slot 9 (+0x24) on each resource (the loop
// head is aligned with a nop: speed TU); the trailing store is tail-duplicated into both exits.
// Local polymorphic models (the generated engine headers flatten the vptrs into plain words); member names and offsets
// are the PDB ones from engine/CResource.h and engine/CResourceList.h, the +4 word of CResource is IVRefCount of its
// CIVCountedPointeeBase base. CResource vtable 0x012354A4: slot 3 (+0x0C) GetUnloadableMemory (0x009FBEF0, returns
// ResourceSize), slot 9 (+0x24) ResetDebugMemoryInUse (0x009FBF10).
class CResourceList;

class CResource { public:                                   // 0x1C bytes
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

class CResourceList { public:                               // vtable 0x0129C808, 0x3C bytes
    virtual ~CResourceList();
    CResource     Head;                              // +0x04 (sentinel)
    unsigned long ResourceCount;                     // +0x20
    long          AllocatedMemory;                   // +0x24
    long          MaximumMemory;                     // +0x28
    unsigned long CurrentFrame;                      // +0x2C
    unsigned long DebugStatsFrame;                   // +0x30
    unsigned long UnloadDelay;                       // +0x34
    unsigned long UnloadedThisFrame;                 // +0x38
    void ResetDebugStats();
};

void CResourceList::ResetDebugStats()
{
    for (CResource* resource = Head.NextResource; resource != &Head; resource = resource->NextResource)
        resource->ResetDebugMemoryInUse();
    DebugStatsFrame = CurrentFrame;
}