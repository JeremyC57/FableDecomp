// CResourceList::CResourceList() -- default constructor: own vtable, the head sentinel resource constructed inline and linked to itself, counters and frames zeroed, no memory limit (0x7FFFFFFF), unload delay 1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC570, length 77 bytes (+3 int3 padding).
// Leaf: ??0CResourceList
// Retail bytes:  mov eax,ecx ; xor edx,edx ; lea ecx,[eax+4] ; mov dword ptr [eax],offset CResourceList::`vftable' ; mov dword ptr [ecx+4],1 ;
//                mov dword ptr [ecx],offset CResource::`vftable' ; mov [ecx+8],edx ; ... ; mov [ecx+18h],edx ; mov [eax+10h],ecx ;
//                mov [eax+14h],ecx ; mov [eax+20h],edx ; mov [eax+2Ch],edx ; mov [eax+30h],edx ; mov [eax+24h],edx ;
//                mov dword ptr [eax+28h],7FFFFFFFh ; mov dword ptr [eax+34h],1 ; mov [eax+38h],edx ; ret
// Vtables (rtti-vtables.tsv): 0x0129C808 = CResourceList, 0x012354A4 = CResource. The head member is a CResource whose
// default constructor (out of line at 0x009FBEC0: reference count 1 from the counted-pointee base, own vtable, five
// zeros) is inlined here; local polymorphic models with the PDB member names (engine/CResource.h, engine/CResourceList.h).
// Shape: the list's vtable store precedes the member construction; the remaining stores are body assignments in source
// order (0x20, 0x2C, 0x30, 0x24, 0x28, 0x34, 0x38 -- not declaration order), sharing the zero register.
struct CResourceList;

// engine/CIVCountedPointeeBase.h names the count IVRefCount (+4); its generated header flattens the vptr.
struct CIVCountedPointeeBase {                       // vtable 0x012354D0
    virtual ~CIVCountedPointeeBase();
    long IVRefCount;                                   // +0x04
    CIVCountedPointeeBase() : IVRefCount(1) { }
};

struct CResource : CIVCountedPointeeBase {           // vtable 0x012354A4, 0x1C bytes
    CResourceList* ResourceList;                     // +0x08
    CResource*     PrevResource;                     // +0x0C
    CResource*     NextResource;                     // +0x10
    unsigned long  ResourceSize;                     // +0x14
    unsigned long  LastUsedFrame;                    // +0x18
    CResource() : ResourceList(0), PrevResource(0), NextResource(0), ResourceSize(0), LastUsedFrame(0) { }
    virtual ~CResource();
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
    CResourceList();
};

CResourceList::CResourceList()
{
    Head.PrevResource = &Head;
    Head.NextResource = &Head;
    ResourceCount = 0;
    CurrentFrame = 0;
    DebugStatsFrame = 0;
    AllocatedMemory = 0;
    MaximumMemory = 0x7fffffff;
    UnloadDelay = 1;
    UnloadedThisFrame = 0;
}
