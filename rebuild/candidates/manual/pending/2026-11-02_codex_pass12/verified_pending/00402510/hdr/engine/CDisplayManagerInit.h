#ifdef FABLE_RETAIL_GFMAIN
#ifndef FABLE_ENGINE_GFMAIN_CDISPLAYMANAGERINIT
#define FABLE_ENGINE_GFMAIN_CDISPLAYMANAGERINIT
// Retail opt-in (Codex pass12): original PDB declarations, native GFMain field offsets.
#include "engine/CWideString.h"
#include "engine/ESurfaceMultisampleType.h"
class CDisplayManagerInit{public:
 long Width,Height,Depth,RefreshRate;bool UseRenderManager,UseShaderRenderManager;HWND WindowHandle;
 unsigned char ZBufferDepth,NoBackBuffers;ESurfaceMultisampleType MultisampleType;
 long PushBufferSize,KickOffSize;bool PresentImmediate,PresentImmediatelyIfMissedVSync;
 long PresentationIntevalVSyncCount;bool SkipConfigDetection;unsigned long RenderManagerInitFlags;CWideString ShaderResourcePath;
 CDisplayManagerInit();
};
FABLE_STATIC_ASSERT(sizeof(CDisplayManagerInit)==60);
#endif

#else

#endif
