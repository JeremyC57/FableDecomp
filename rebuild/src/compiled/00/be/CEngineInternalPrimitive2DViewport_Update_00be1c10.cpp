// CEngineInternalPrimitive2DViewport::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy the 2D viewport primitive: layer, viewport box, FOVs (x 2pi), 2D-FOV flag, alpha-mask bank/index, clip box, mask/filter bytes, and the alpha-mask position factors from the render-target dimensions
// Leaf: ?Update@CEngineInternalPrimitive2DViewport@@UAEXABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE1C10, 353 bytes, thiscall virtual, `ret 8`.
// Identity: Ego_r.pdb ?Update@CEngineInternalPrimitive2DViewport@@UAEXABVCEnginePrimitive@@K@Z; the description is Ego_r
//   CEnginePrimitive2DViewport (Layer +0xC, Viewport +0x10, FOVs +0x20/+0x24, Use2DFOV +0x28, AlphaMaskBank +0x2C, Index +0x30,
//   ClipBox +0x34, Mask +0x44, Filter +0x45); the object is the typed CEngineInternalPrimitive2DViewport (hdr opt-in).
// Callees: CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 on the display manager global 0x01436E1C.
// Data: 2*pi 0x0128F608, 0.5f 0x0122F59C, 1.0f 0x0122DED8.
// Shape: one block of 81+ IL tuples, so VC7.1's list scheduler cuts it into regions of at most 81 tuples (c2.dll 0x107B4E6A),
//   counting the no-code PAREN tuples of parenthesised float operands. Retail's first region ends at the Y factor's `fdivr`
//   (0x00BE1D06): the Y store then leads region 2 beside the factor copy's `mov eax,[esp+18h]` (0x00BE1D0C / 0x00BE1D10) and the
//   Z term interleaves with the X / Y copy (0x00BE1D14..0x00BE1D28). That needs ten PAREN tuples before it: the two half-extent
//   reciprocals, the four parenthesised clip-space products and the parenthesised X / Y factor arguments of the C4DVector
//   constructor (the Z / W arguments stay bare: parenthesised, they move a load in region 2, md 2). Without the product and
//   argument parentheses the cut falls after the Z term's `fadd` and Z is finished before the copy (17 lines).
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DVIEWPORT_MEMBERS
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_compiler.h"
#include "engine/CEngineInternalPrimitive2DViewport.h"
#include "engine/CEnginePrimitive2DViewport.h"
#include "engine/Win32Types.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"

extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

void CEngineInternalPrimitive2DViewport::Update(const CEnginePrimitive& primitive, unsigned long)
{
    const CEnginePrimitive2DViewport& viewport = static_cast<const CEnginePrimitive2DViewport&>(primitive);
    Layer = viewport.Layer;
    Viewport = viewport.Viewport;
    HorizontalFOV = viewport.HorizontalFOV * 6.2831855f;
    VerticalFOV = viewport.VerticalFOV * 6.2831855f;
    Use2DFOV = viewport.Use2DFOV;
    AlphaMaskInfo.AlphaMaskBank = viewport.AlphaMaskBank;
    AlphaMaskInfo.AlphaMaskBankIndex = viewport.AlphaMaskBankIndex;
    AlphaMaskInfo.AlphaMaskClipBox = viewport.AlphaMaskClipBox;
    C2DExtentsI dims = g_pDisplayManager_01436e1c->GetRenderTargetDimensions();
    float invHalfWidth = 1.0f / (dims.X * 0.5f);
    float invHalfHeight = 1.0f / (dims.Y * 0.5f);
    float right = (invHalfWidth * viewport.AlphaMaskClipBox.BRX) - 1.0f;
    float bottom = 1.0f - (invHalfHeight * viewport.AlphaMaskClipBox.BRY);
    float left = (invHalfWidth * viewport.AlphaMaskClipBox.TLX) - 1.0f;
    float top = 1.0f - (invHalfHeight * viewport.AlphaMaskClipBox.TLY);
    // The parenthesised products and X / Y arguments are retail's PAREN tuples (region cut at 0x00BE1D06, see Shape).
    AlphaMaskInfo.AlphaMaskPositionFactors = C4DVector((1.0f / (right - left)), (1.0f / (bottom - top)),
                                                       right / (left - right) + 1.0f, bottom / (top - bottom) + 1.0f);
    AlphaMaskInfo.MaskObjectsOutsideAlphaMask = viewport.MaskObjectsOutsideAlphaMask;
    AlphaMaskInfo.FilterMask = viewport.FilterMask;
}