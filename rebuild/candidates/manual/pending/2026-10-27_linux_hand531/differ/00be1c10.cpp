// CEngineInternalPrimitive2DViewport::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy the 2D viewport primitive: layer, viewport box, FOVs (x 2pi), 2D-FOV flag, alpha-mask bank/index, clip box, mask/filter bytes, and the alpha-mask position factors from the render-target dimensions
// Grade: DIFFER(353v353) -- scheduling only: retail copies the computed factor X/Y into AlphaMaskPositionFactors (+0x6c) before it computes Z; VC7.1 computes all three first (reference target, reordered locals, named sx/sy and `1 +` order were inert)
// Leaf: ?Update@CEngineInternalPrimitive2DViewport@@UAEXABVCEnginePrimitive@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE1C10, 353 bytes, thiscall virtual, `ret 8`. 1 grade used.
// Identity (proven): Ego_r.pdb ?Update@CEngineInternalPrimitive2DViewport@@UAEXABVCEnginePrimitive@@K@Z; desc layout = Ego_r
//   CEnginePrimitive2DViewport (Layer +0xc, Viewport +0x10, FOVs +0x20/+0x24, Use2DFOV +0x28, AlphaMaskBank +0x2c, Index +0x30,
//   ClipBox +0x34, Mask +0x44, Filter +0x45).
// Callees: CDisplayManager::GetRenderTargetDimensions 0x009BEDC0 on the display manager global 0x01436E1C.
class C2DBoxF { public: float TLX, TLY, BRX, BRY; };
class C2DExtentsI { public:
    long X, Y;
    C2DExtentsI() { }
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};
class C4DVector { public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
enum EEngine2DLayer { };
class CGraphicDataBank;
class CDisplayManager { public:
    const C2DExtentsI GetRenderTargetDimensions();     // 0x009BEDC0
};
extern CDisplayManager* g_pDisplayManager_01436e1c;   // 0x01436E1C

class C2DAlphaMaskInfo { public:
    CGraphicDataBank* AlphaMaskBank;                  // +0x00
    unsigned long     AlphaMaskBankIndex;             // +0x04
    C4DVector         AlphaMaskPositionFactors;       // +0x08
    C2DBoxF           AlphaMaskClipBox;               // +0x18
    bool              MaskObjectsOutsideAlphaMask;    // +0x28
    bool              FilterMask;                     // +0x29
};
class CEnginePrimitive { public:
    long          Type;                                // +0x00
    long          InterpolationSet;                    // +0x04
    unsigned long ThemeChannelDesc;                    // +0x08
};
class CEnginePrimitive2DViewport : public CEnginePrimitive { public:
    EEngine2DLayer    Layer;                           // +0x0c
    C2DBoxF           Viewport;                        // +0x10
    float             HorizontalFOV;                   // +0x20
    float             VerticalFOV;                     // +0x24
    bool              Use2DFOV;                        // +0x28
    CGraphicDataBank* AlphaMaskBank;                   // +0x2c
    unsigned long     AlphaMaskBankIndex;              // +0x30
    C2DBoxF           AlphaMaskClipBox;                // +0x34
    bool              MaskObjectsOutsideAlphaMask;     // +0x44
    bool              FilterMask;                      // +0x45
};
class CEngineInternalPrimitive2DViewport { public:
    virtual ~CEngineInternalPrimitive2DViewport();
    unsigned char     _pad04[0x48 - 4];
    C2DBoxF           Viewport;                        // +0x48
    float             HorizontalFOV;                   // +0x58
    float             VerticalFOV;                     // +0x5c
    bool              Use2DFOV;                        // +0x60
    C2DAlphaMaskInfo  AlphaMaskInfo;                   // +0x64
    EEngine2DLayer    Layer;                           // +0x90
    virtual void Update(const CEnginePrimitive& primitive, unsigned long flags);
};

void CEngineInternalPrimitive2DViewport::Update(const CEnginePrimitive& primitive, unsigned long flags)
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
    float right = invHalfWidth * viewport.AlphaMaskClipBox.BRX - 1.0f;
    float bottom = 1.0f - invHalfHeight * viewport.AlphaMaskClipBox.BRY;
    float left = invHalfWidth * viewport.AlphaMaskClipBox.TLX - 1.0f;
    float top = 1.0f - invHalfHeight * viewport.AlphaMaskClipBox.TLY;
    AlphaMaskInfo.AlphaMaskPositionFactors = C4DVector(1.0f / (right - left), 1.0f / (bottom - top),
                                                       right / (left - right) + 1.0f, bottom / (top - bottom) + 1.0f);
    AlphaMaskInfo.MaskObjectsOutsideAlphaMask = viewport.MaskObjectsOutsideAlphaMask;
    AlphaMaskInfo.FilterMask = viewport.FilterMask;
}
