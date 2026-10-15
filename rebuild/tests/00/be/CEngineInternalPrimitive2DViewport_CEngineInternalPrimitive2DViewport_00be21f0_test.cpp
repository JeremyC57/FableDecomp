#include <cstdio>
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxF() { } };
struct C4DVector {
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
};
struct C2DAlphaMaskInfo {
    void* AlphaMaskBank; unsigned int AlphaMaskBankIndex; C4DVector AlphaMaskPositionFactors; C2DBoxF AlphaMaskClipBox;
    bool MaskObjectsOutsideAlphaMask; bool FilterMask;
    C2DAlphaMaskInfo() : AlphaMaskBank(0), AlphaMaskBankIndex(0)
    {
        AlphaMaskPositionFactors = C4DVector(0.0f, 0.0f, 0.0f, 0.0f);
        MaskObjectsOutsideAlphaMask = false;
        FilterMask = false;
    }
};
struct CHandleArray { void* m_pFirst; void* m_pLast; void* m_pEnd; CHandleArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CEngineInternalPrimitiveBase {
    long Type; unsigned int RenderLayerMask;
    unsigned char Enable : 1; unsigned char NonPersistent : 1; unsigned char OverrideFadeDistance : 1; unsigned char PostRenderEnable : 1;
    CEngineInternalPrimitiveBase(long type, unsigned int mask) : Type(type), RenderLayerMask(mask)
    { Enable = 1; NonPersistent = 0; OverrideFadeDistance = 0; PostRenderEnable = 0; }
};
struct CEngineInternalPrimitive2DViewport : CEngineInternalPrimitiveBase {
    C2DBoxF Viewport; float HorizontalFOV; float VerticalFOV; bool Use2DFOV;
    C2DAlphaMaskInfo AlphaMaskInfo; long Layer; CHandleArray Primitives;
    CEngineInternalPrimitive2DViewport() : CEngineInternalPrimitiveBase(0x21, 0x80000000u) { PostRenderEnable = 1; }
};
int main()
{
    CEngineInternalPrimitive2DViewport v;
    if (v.Type != 0x21 || v.RenderLayerMask != 0x80000000u || !v.PostRenderEnable || !v.Enable
        || v.AlphaMaskInfo.AlphaMaskBank != 0 || v.AlphaMaskInfo.AlphaMaskBankIndex != 0
        || v.AlphaMaskInfo.AlphaMaskPositionFactors.X != 0.0f || v.AlphaMaskInfo.AlphaMaskPositionFactors.W != 0.0f
        || v.AlphaMaskInfo.MaskObjectsOutsideAlphaMask || v.AlphaMaskInfo.FilterMask
        || v.Primitives.m_pFirst != 0 || v.Primitives.m_pLast != 0 || v.Primitives.m_pEnd != 0) {
        std::printf("00be21f0_TEST FAIL\n"); return 1;
    }
    std::printf("00be21f0_TEST PASS\n");
    return 0;
}