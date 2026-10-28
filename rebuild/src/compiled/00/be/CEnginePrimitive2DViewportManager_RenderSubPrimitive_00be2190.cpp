// CEnginePrimitive2DViewportManager::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass) -- render a 2D viewport with its alpha mask made current, then reset the current alpha mask
// Leaf: ?RenderSubPrimitive@CEnginePrimitive2DViewportManager@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BE2190, 89 bytes, thiscall, `ret 8`.
// Identity (proven): Ego_r.pdb ?RenderSubPrimitive@CEnginePrimitive2DViewportManager@@UAEXAAVCEngineSubPrimitiveBase@@W4ERenderPassType@@@Z;
//   the sub-primitive's Parent (+0x0C) is the CEngineInternalPrimitive2DViewport whose AlphaMaskInfo (+0x64, 0x2C bytes)
//   is copied into the manager's AlphaMaskInfo (+0x0C) before ?Render@CEngineInternalPrimitive2DViewport@@QAEXXZ 0x00BE1E70.
// Shape: the implicit C2DAlphaMaskInfo copy (`rep movsd` x 0xB); afterwards the inline ?Reset@C2DAlphaMaskInfo@@QAEXXZ
//   (Ego_r.pdb): bank/index nulled, the position factors assigned from a C4DVector(0, 0, 0, 0) temporary (C4DVector has
//   no user copy, so the copy is constant-folded but the dead 16-byte temporary's zero stores stay in the frame), both
//   flags cleared; the clip box is left alone.
// Header debt: no engine header models C2DAlphaMaskInfo or the viewport's AlphaMaskInfo member typed.
class CGraphicDataBank;
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
class C2DBoxF {
public:
    float TLX, TLY, BRX, BRY;
    C2DBoxF() { }
};
class C2DAlphaMaskInfo {                                   // 0x2C bytes (Ego_r.pdb)
public:
    CGraphicDataBank* AlphaMaskBank;                      // +0x00
    unsigned long     AlphaMaskBankIndex;                 // +0x04
    C4DVector         AlphaMaskPositionFactors;           // +0x08
    C2DBoxF           AlphaMaskClipBox;                   // +0x18
    bool              MaskObjectsOutsideAlphaMask;        // +0x28
    bool              FilterMask;                         // +0x29
    void Reset()
    {
        AlphaMaskBank = 0;
        AlphaMaskBankIndex = 0;
        AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
        MaskObjectsOutsideAlphaMask = false;
        FilterMask = false;
    }
};

class CEngineInternalPrimitiveBase {                      // 0x48 bytes (Ego_r.pdb), through CIVCountedPointeeBase
public:
    virtual ~CEngineInternalPrimitiveBase();
    unsigned char _pad04[0x48 - 0x04];
};
class CEngineInternalPrimitive2DViewport : public CEngineInternalPrimitiveBase {
public:
    C2DBoxF          Viewport;                            // +0x48
    float            HorizontalFOV;                       // +0x58
    float            VerticalFOV;                         // +0x5c
    bool             Use2DFOV;                            // +0x60
    C2DAlphaMaskInfo AlphaMaskInfo;                       // +0x64
    void Render();                                        // 0x00BE1E70
};

class CEngineSubPrimitiveBase {
public:
    virtual ~CEngineSubPrimitiveBase();
    unsigned long                 RenderFlags;            // +0x04
    unsigned char                 Alpha;                  // +0x08
    CEngineInternalPrimitiveBase* Parent;                 // +0x0c
};

enum ERenderPassType { RENDER_PASS_TYPE_FORCE_DWORD = 0x7fffffff };

class CEnginePrimitive2DViewportManager {
public:
    virtual void RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType pass);
    unsigned char    _pad04[0x0c - 0x04];                 // CEnginePrimitiveManager words
    C2DAlphaMaskInfo AlphaMaskInfo;                       // +0x0c
};

void CEnginePrimitive2DViewportManager::RenderSubPrimitive(CEngineSubPrimitiveBase& subPrimitive, ERenderPassType)
{
    CEngineInternalPrimitive2DViewport& viewport = static_cast<CEngineInternalPrimitive2DViewport&>(*subPrimitive.Parent);
    AlphaMaskInfo = viewport.AlphaMaskInfo;
    viewport.Render();
    AlphaMaskInfo.Reset();
}