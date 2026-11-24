#ifndef FABLE_ENGINE_CDISPLAYMANAGERINIT_H
#define FABLE_ENGINE_CDISPLAYMANAGERINIT_H
#include "rebuild_abi.h"
// New header (B24C4, row 0x00402510 GFMain): CDisplayManagerInit, Ego_r.pdb layout (0x3C bytes, every member name and offset);
// embedded in the native CSystemManagerInit at +0x5C. Its constructor is out of line in retail (0x004016F0, landed; called by
// CSystemManagerInit() 0x00403B10). Declaration only.
#include "fable_string.h"
#include "engine/Win32Types.h"
#include "engine/ESurfaceMultisampleType.h"
class CDisplayManagerInit {
public:
    long Width;                                 // +0x00
    long Height;                                // +0x04
    long Depth;                                 // +0x08
    long RefreshRate;                           // +0x0c
    bool UseRenderManager;                      // +0x10
    bool UseShaderRenderManager;                // +0x11
    HWND WindowHandle;                          // +0x14
    unsigned char ZBufferDepth;                 // +0x18
    unsigned char NoBackBuffers;                // +0x19
    ESurfaceMultisampleType MultisampleType;    // +0x1c
    long PushBufferSize;                        // +0x20
    long KickOffSize;                           // +0x24
    bool PresentImmediate;                      // +0x28
    bool PresentImmediatelyIfMissedVSync;       // +0x29
    long PresentationIntevalVSyncCount;         // +0x2c (sic, PDB spelling)
    bool SkipConfigDetection;                   // +0x30
    unsigned long RenderManagerInitFlags;       // +0x34
    CWideString ShaderResourcePath;             // +0x38
    CDisplayManagerInit();                      // 0x004016F0
};
FABLE_STATIC_ASSERT(sizeof(CDisplayManagerInit) == 0x3c);
#endif
