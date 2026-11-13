#ifndef FABLE_ENGINE_CVOLUMETEXTURE_H
#define FABLE_ENGINE_CVOLUMETEXTURE_H
// NEW (chunk 2008, row 0x00B54890 CEngineWeatherRenderer::RenderRain). Ego_r.pdb CVolumeTexture (class, 16 bytes): a vptr (??_ECVolumeTexture@@UAEPAXI@Z,
// virtual destructor), PD3DVolumeTexture +0x04, ByteLength +0x08, PData +0x0C. RenderRain reads PD3DVolumeTexture through the inline
// AttachVolumeTextureToStage (0x00B54926 `mov eax,[eax+4]`). Members only; the methods (Ego_r.pdb ?PreLoad@ / ?Unlock@ / ...) are not declared.
#include "rebuild_abi.h"
struct IDirect3DVolumeTexture9;
class CVolumeTexture {
public:
    virtual ~CVolumeTexture();
    IDirect3DVolumeTexture9* PD3DVolumeTexture;                          // +0x04
    long                     ByteLength;                                 // +0x08
    void*                    PData;                                      // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CVolumeTexture) == 0x10);
#endif
