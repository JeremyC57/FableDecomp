// CResource::GetUnusedFrames() const -- frames since the resource was last used: its list's current frame minus LastUsedFrame, or
// 0xFFFFFFFF when it is in no resource list
// Leaf: ?GetUnusedFrames@CResource@@QBEKXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FBF70, 18 bytes: `mov eax,[ecx+8] ; test ; je ; mov eax,[eax+2Ch] ; sub eax,[ecx+18h] ; ret ;
//   or eax,-1 ; ret`.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetUnusedFrames@CResource@@QBEKXZ; engine/CResource.h (retail) names it at 0x009FBF70
//   (ResourceList +0x08, LastUsedFrame +0x18) and CResourceList.h CurrentFrame +0x2C / GetCurrentFrame() is the word read.
// Name fix (agentNF5 2026-11-10): was the free stand-in `fn_009fbf70(UF*)` (?fn_009fbf70@@YIHPAUUF@@@Z) over padded local structs.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICINFOFRAME_FREE
#define FABLE_RETAIL_CRESOURCELIST
#include "engine/CResource.h"
#include "engine/CResourceList.h"
unsigned long CResource::GetUnusedFrames() const
{
    if (ResourceList)
        return ResourceList->GetCurrentFrame() - LastUsedFrame;
    return 0xffffffff;
}
