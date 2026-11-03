#ifndef FABLE_ENGINE_CTEXTURERENDERER_H
#define FABLE_ENGINE_CTEXTURERENDERER_H
// Retail CTextureRenderer (CVideoSys::TexRenderer +0x20; movie row 0x006286F0): the DirectX 9 SDK Texture3D9 sample's
// renderer filter. RTTI (vtable 0x0129D08C) CTextureRenderer : CBaseVideoRenderer (strmbase, engine/CBaseVideoRenderer.h,
// 0x160 bytes). Members: the Ego_r.pdb platform stub's own members (PVideoSys .. CreateOwnTexture, 0x1C bytes) after the
// base, as the landed rows use them: PVideoSys +0x160 (CVideoSys::Init 0x00A3B9D0 stores `this`), Texture +0x164,
// TextureFormat +0x168, Width +0x16C / Height +0x170 (SetMediaType 0x00A3B5F0 writes them; 0x006286F0 reads them),
// Pitch +0x174, CreateOwnTexture +0x178; 0x180 in all (CVideoSys::Init allocates 0x180; 8-aligned by the base's
// REFERENCE_TIME members). Member functions: the landed CTextureRenderer rows (ctor 0x00A3B510, dtor 0x00A3B970,
// CheckMediaType 0x00A3B590, SetMediaType 0x00A3B5F0, DoRenderSample 0x00A3B730) define them with local models; only
// the constructor is declared here.
#ifndef FABLE_RETAIL_CBASEVIDEORENDERER
#define FABLE_RETAIL_CBASEVIDEORENDERER
#endif
#include "rebuild_abi.h"
#include "engine/CBaseVideoRenderer.h"
class CVideoSys; class CTexture;
enum _D3DFORMAT;                                                         // MS forward enum (d3d9types.h)
class CTextureRenderer : public CBaseVideoRenderer {
public:
    CTextureRenderer(IUnknown*,long*,CTexture*);                         // 0x00A3B510 ??0CTextureRenderer@@QAE@PAUIUnknown@@PAJPAVCTexture@@@Z
    CVideoSys* PVideoSys;                                                // +0x160
    CTexture*  Texture;                                                  // +0x164
    _D3DFORMAT TextureFormat;                                            // +0x168
    int        Width;                                                    // +0x16C
    int        Height;                                                   // +0x170
    int        Pitch;                                                    // +0x174
    bool       CreateOwnTexture;                                         // +0x178
};
FABLE_STATIC_ASSERT(sizeof(CTextureRenderer) == 0x180);
#endif // FABLE_ENGINE_CTEXTURERENDERER_H
