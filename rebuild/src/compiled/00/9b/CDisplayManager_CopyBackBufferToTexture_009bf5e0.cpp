// CDisplayManager::CopyBackBufferToTexture()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BF5E0, oracle length 18 bytes.
// Retail bytes:  push esi ; lea eax,[ecx+2Ch] ; lea esi,[ecx+4Ch] ; push eax ; push esi ; call CDisplayManager::BlitRenderTargetSurfaceToTexture ; mov eax,esi ; pop esi ; ret
// Blits the back-buffer surface (+0x2C) into the back-buffer copy texture (+0x4C) and returns that texture.
// Port (NF98, round 8, q11274): the local CDisplayManager_Retail stand-in replaced by the shared engine/CDisplayManager.h (Ego_r.pdb
//   members BackBufferSurface +0x2C / BackBufferCopyTexture +0x4C); own leaf now the Ego_r.pdb 0x00B24A90 / FableWin.pdb 0x0301CEA0
//   spelling (the header's FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN declaration) and the callee 0x009BF350 is bound as
//   ?BlitRenderTargetSurfaceToTexture@CDisplayManager@@QAEXAAVCTexture@@ABVCSurface@@@Z (FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE).
// Leaf: ?CopyBackBufferToTexture@CDisplayManager@@QAEAAVCTexture@@XZ
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_BLITRENDERTARGETSURFACETOTEXTURE
#define FABLE_RETAIL_CDISPLAYMANAGER_BACKSCREEN
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/CPixelFormat.h"
#include "engine/CDisplayManager.h"

CTexture& CDisplayManager::CopyBackBufferToTexture()
{
    BlitRenderTargetSurfaceToTexture(BackBufferCopyTexture, BackBufferSurface);
    return BackBufferCopyTexture;
}
