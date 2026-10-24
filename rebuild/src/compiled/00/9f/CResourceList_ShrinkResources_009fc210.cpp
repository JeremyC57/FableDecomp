// CResourceList::ShrinkResources(unsigned long) -- while AllocatedMemory + size exceeds MaximumMemory, unload the next resource (UnloadNextResource(0) @ 0x009FC170), giving up after ResourceCount tries or when nothing more unloads
// Leaf: ?ShrinkResources@CResourceList_Methods@@QAEXK@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FC210, 64 bytes, thiscall, `ret 4`.
// Name: Ego_d.pdb ?ShrinkResources@CResourceList@@QAEXK@Z (the only void(unsigned long) CResourceList member besides
//   IncrementFrameCounter); its caller CResourceList::MakeFreeSpace @ 0x009FC4F0 (landed, where it is "EvictForSpace") is
//   ?MakeFreeSpace@CResourceList@@QAE_NK@Z. The callee 0x009FC170 is ?UnloadNextResource@CResourceList@@QAE_NK@Z.
//   The chunk label "CResourceList_MakeFreeSpace" names the caller.
// Layout: engine/CResourceList.h (Ego_r.pdb): ResourceCount +0x20, AllocatedMemory +0x24, MaximumMemory +0x28.
// Shape: the unsigned compare (long + unsigned long) tested before the loop and again at its foot; `count++ >= ResourceCount`
//   keeps the old count in ecx (`mov ecx,edi ; inc edi`).
#include "engine/CResourceList.h"

struct CResourceList_Methods : CResourceList {
    bool UnloadNextResource(unsigned long frames);  // 0x009FC170
    void ShrinkResources(unsigned long size);
};

void CResourceList_Methods::ShrinkResources(unsigned long size)
{
    unsigned long count = 0;
    while (AllocatedMemory + size > MaximumMemory) {
        if (count++ >= ResourceCount)
            break;
        if (!UnloadNextResource(0))
            break;
    }
}