// CEngineShadowRenderer::BuildDepthPackingLookupTexture -- create 256-square ARGB lookup texture and encode column/row parity into pixels.
// Leaf: ?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ
// Retail00b4f420 complete231bytes. Native shared shadow DepthPackingLookupTexture32c and pixel-format/CSurface layouts; real virtual native Surface view avoids adding vptr to plain shared layout.
// Callees9e3790formatInitialise,9fa280textureInitialise,9f9e00GetSurface,9f33e0Lock,9f31c0SetPixel,9f2dc0Unlock,9f2e20Uninitialise;surfacevtable0122f84c.
// NF63 (round 7): moved onto engine/CEngineShadowRenderer.h (MEMBERS: DepthPackingLookupTexture +0x32C), engine/CTexture.h
// (Initialise, GetSurfaceLevel), engine/CSurface.h (LOADGRAPHICFRAME Lock / Unlock, overlay FABLE_RETAIL_CSURFACE_SETPIXEL),
// engine/CPixelFormat.h, C2DExtentsI.h, CRGBColour.h and CSurfaceLockedRect.h; binds 0x009F2DC0 ?Unlock@CSurface@@QBEXXZ (nf41:e41)
// and every callee on the header classes (were the PixelMethods / ExtentsNative / ColourNative / LockedRectNative / SurfaceNative /
// TextureMethods / ShadowLookupMethods stand-ins). Self ?BuildDepthPackingLookupTexture@CEngineShadowRenderer@@AAEXXZ (private, both
// PDBs; overlay _BUILDDEPTHPACKING).
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_BUILDDEPTHPACKING
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_LOADGRAPHICFRAME
#define FABLE_RETAIL_CSURFACE_SETPIXEL
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#include "engine/CMatrix4x4.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CRenderTarget.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CPixelFormat.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/ESurfacePool.h"

// The pixel is CRGBColour(column, row & ~1, 0, 0): red = column, green = even row (native B,G,R,A order; the old {R,G,B,A}
// stand-in wrote the column into its third byte, which is R).
void CEngineShadowRenderer::BuildDepthPackingLookupTexture()
{
    CPixelFormat format(8, 8, 8, 8);
    DepthPackingLookupTexture.Initialise(C2DExtentsI(256, 256), 1, format, 0, SURFACE_POOL_MANAGED, false, false);
    CSurface surface = DepthPackingLookupTexture.GetSurfaceLevel(0);
    CSurfaceLockedRect rect = surface.Lock(0);
    for (long row = 0; row < 256; ++row) {
        long col = 0;
        unsigned char green = (unsigned char)(row & ~1);
        for (; col < 256; ++col)
            surface.SetPixel(col, row, CRGBColour((unsigned char)col, green, 0, 0), rect);
    }
    surface.Unlock();
}
