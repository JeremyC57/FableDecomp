// Grade: DIFFER(805v811) -- first-pass model: ours caches the two texture addresses in ebx/ebp and initialises the depth format with a -1 default ctor; retail recomputes the lea per use and seeds -1 only for the colour/chosen formats; the second technique schedules differently.
// CEngineShadowRenderer::InitialiseTextures() -- (re)create the shadow-buffer textures for the user's buffer size: prefer the colour-buffer technique (A8R8G8B8-class colour texture + best opaque render target), fall back to a depth-texture technique on cards with enough texture memory (first supported depth format of the list), and wire the render target; returns false only when shadows were requested but nothing could be created
// Leaf: InitialiseTextures
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B4F8E0, length 816 bytes (body 0x32b + int3 padding), plain `ret`.
// Callees: OnReleaseDefaultPoolResources 0x00B4B830, CPixelFormat::Initialise 0x009E3830, CDisplayManager (0x01436E1C)
//   IsValidTextureFormat 0x009BE8F0 / GetBestOpaqueTextureFormat 0x009BE610 / IsValidRenderTargetFormat 0x009BE930,
//   Engine_CalculateTextureMemoryFromQuality 0x009BEE00, CTexture::Initialise 0x009FA280 / GetSurface 0x009F9E00 / Uninitialise
//   0x009F9F70 / operator= 0x009FA1C0, CSurface::operator= 0x009F2E70 / Uninitialise 0x009F2E20 (vtable 0x0122F84C) /
//   InitialiseAsDepthBuffer 0x009F32A0, CRenderTarget::Uninitialise 0x00A10E30 / AttachTarget 0x00A10E60 / AttachDepth 0x00A10E90,
//   BuildDepthPackingLookupTexture 0x00B4F420.
// Layout: engine/CEngineShadowRenderer.h (TexturesAllocated +0x2af, RenderTarget +0x2c4, ShadowBufferTexture +0x2e8,
//   ShadowBufferDepthSurface +0x2f8, ShadowBufferColourSurface +0x308, ShadowBufferColourTexture +0x318, RenderingTechnique +0x334,
//   UserVideoOptionShadowBufferSize +0x338).
#include "engine/CEngineShadowRenderer.h"

struct C2DExtentsI {
    long Width, Height;
    C2DExtentsI(long w, long h) : Width(w), Height(h) {}
};
struct CPixelFormat {
    long Format;
    CPixelFormat() : Format(-1) {}
    void Initialise(long format);                  // 0x009E3830
};
struct CSurface {                                  // vtable 0x0122F84C
    unsigned char _body[0xc];
    virtual void Slot0();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                           // 0x009F2E20
    CSurface& operator=(const CSurface& other);    // 0x009F2E70
    bool InitialiseAsDepthBuffer(const C2DExtentsI* size, const CPixelFormat* format, long multisample, bool lockable);   // 0x009F32A0
};
struct CTexture {
    void* _body[2];
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
    CSurface GetSurface(long level);               // 0x009F9E00
    void Uninitialise();                           // 0x009F9F70
    CTexture& operator=(const CTexture& other);    // 0x009FA1C0
};
struct CRenderTarget {
    void Uninitialise();                           // 0x00A10E30
    void AttachTarget(CSurface* surface);          // 0x00A10E60
    void AttachDepth(CSurface* surface);           // 0x00A10E90
};
struct CDisplayManager {
    bool IsValidTextureFormat(const CPixelFormat* format);                                        // 0x009BE8F0
    bool GetBestOpaqueTextureFormat(unsigned long bits, CPixelFormat* format, bool renderTarget);   // 0x009BE610
    bool IsValidRenderTargetFormat(const CPixelFormat* format, const CPixelFormat* depth);        // 0x009BE930
};
extern CDisplayManager* g_pDisplayManager;         // 0x01436E1C
unsigned long Engine_CalculateTextureMemoryFromQuality();   // 0x009BEE00

struct CEngineShadowRenderer_Methods : CEngineShadowRenderer {
    void OnReleaseDefaultPoolResources();          // 0x00B4B830
    void BuildDepthPackingLookupTexture();         // 0x00B4F420
    bool InitialiseTextures();
};

bool CEngineShadowRenderer_Methods::InitialiseTextures()
{
    CTexture& shadowTexture = *(CTexture*)ShadowBufferTexture;
    CTexture& colourTexture = *(CTexture*)ShadowBufferColourTexture;
    OnReleaseDefaultPoolResources();
    *(long*)RenderingTechnique = 0;
    if (UserVideoOptionShadowBufferSize > 0) {
        CPixelFormat depthFormat;
        depthFormat.Initialise(0x50);
        if (g_pDisplayManager->IsValidTextureFormat(&depthFormat)) {
            CPixelFormat colourFormat;
            g_pDisplayManager->GetBestOpaqueTextureFormat(0x10, &colourFormat, true);
            if (g_pDisplayManager->IsValidRenderTargetFormat(&colourFormat, &depthFormat)) {
                long size = UserVideoOptionShadowBufferSize;
                if (shadowTexture.Initialise(&C2DExtentsI(size, size), 1, &depthFormat, 2, 0, false, true)) {
                    if (colourTexture.Initialise(&C2DExtentsI(size, size), 1, &colourFormat, 1, 0, false, true)) {
                        *(long*)RenderingTechnique = 1;
                        *(CSurface*)ShadowBufferDepthSurface = shadowTexture.GetSurface(0);
                        *(CSurface*)ShadowBufferColourSurface = colourTexture.GetSurface(0);
                    } else {
                        shadowTexture.Uninitialise();
                    }
                }
            }
        }
        if (*(long*)RenderingTechnique == 0 && Engine_CalculateTextureMemoryFromQuality() >= 0x578) {
            long formats[8];
            formats[0] = 0x15;
            formats[1] = 0x20;
            formats[2] = 0;
            formats[3] = 0;
            formats[4] = 0;
            formats[5] = 0;
            formats[6] = 0;
            formats[7] = 0;
            CPixelFormat chosen;
            for (long i = 0; i < 8; ++i) {
                CPixelFormat candidate;
                candidate.Initialise(formats[i]);
                if (g_pDisplayManager->IsValidTextureFormat(&candidate)
                    && g_pDisplayManager->IsValidRenderTargetFormat(&candidate, &depthFormat)) {
                    chosen = candidate;
                    break;
                }
            }
            if (chosen.Format != -1) {
                long size = UserVideoOptionShadowBufferSize;
                if (shadowTexture.Initialise(&C2DExtentsI(size, size), 1, &chosen, 1, 0, false, true)) {
                    if (((CSurface*)ShadowBufferDepthSurface)->InitialiseAsDepthBuffer(&C2DExtentsI(size, size), &depthFormat, 0, true)) {
                        *(long*)RenderingTechnique = 2;
                        colourTexture = shadowTexture;
                        *(CSurface*)ShadowBufferColourSurface = shadowTexture.GetSurface(0);
                        BuildDepthPackingLookupTexture();
                    } else {
                        shadowTexture.Uninitialise();
                    }
                }
            }
        }
    }
    if (*(long*)RenderingTechnique == 0) {
        ((CRenderTarget*)RenderTarget)->Uninitialise();
        ((CSurface*)ShadowBufferColourSurface)->Uninitialise();
        ((CSurface*)ShadowBufferDepthSurface)->Uninitialise();
        colourTexture.Uninitialise();
        shadowTexture.Uninitialise();
        TexturesAllocated = false;
        return UserVideoOptionShadowBufferSize == 0;
    }
    ((CRenderTarget*)RenderTarget)->AttachTarget((CSurface*)ShadowBufferColourSurface);
    ((CRenderTarget*)RenderTarget)->AttachDepth((CSurface*)ShadowBufferDepthSurface);
    TexturesAllocated = true;
    return true;
}
