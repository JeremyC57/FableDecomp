// CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap(unsigned long mipmap, const CGraphicInfoFrame* frame) -- the first mipmap level to preload: the frame's larger side shifted down by the requested level, then halved (one level per step) until it fits the renderer's preload size limit, plus the renderer's mipmap bias
// Leaf: ?GetAcceptablePreloadMipmap@CEnginePrimitiveManagerMeshBase@@CIKKPBVCGraphicInfoFrame@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBBE0, 65 bytes (+ int3 padding). Ego_r.pdb:
// ?GetAcceptablePreloadMipmap@CEnginePrimitiveManagerMeshBase@@CIKKPBVCGraphicInfoFrame@@@Z (private static, __fastcall: the
// level in ecx, the frame in edx, no stack arguments). Callers: PreloadReflectionTexture / PreloadDiffuseTexture (landed
// 0x00BEC000) / PreloadBumpTexture (landed 0x00BEC470).
// Layout: CGraphicInfoFrame (Ego_r.pdb) GraphicInfo +0x0c; CGraphicInfo.Header (CGraphicHeader) FrameWidth +6 / FrameHeight +8
// (unsigned short: `jbe`, `movzx`). The renderer record behind the pointer at 0x01436E78 (the landed GetFadeDistance 0x00BEB5C0
// reads its fade settings at +0x7c..+0x84) holds the mipmap bias at +0xb4 and the largest preload dimension at +0xb8
// (names descriptive). CEnginePrimitiveManagerMeshBase's engine header is an opaque layout without methods; this static
// member needs no `this`, so the class is declared here only to scope it.
// Shape: the larger side is an if/else into an unsigned long local (one `movzx` per arm; a ternary widens once after the
// merge), read through the frame each time (the else arm re-reads frame->GraphicInfo); the level shift is `shr eax,cl`; the
// halving loop counts in a SEPARATE level local (`mov edx,ecx`: incrementing the parameter keeps it in ecx and loads the
// limit into edx), a top-tested while (`cmp ; jbe` then the rotated `shr eax,1 ; inc edx ; cmp ; ja`); the result adds the
// level to the bias. Grades: 3 (ternary: one movzx after the merge and an ebx copy; parameter counter: register swap).
struct CGraphicHeader {
    unsigned short Width;                          // +0x00
    unsigned short Height;                         // +0x02
    unsigned short Depth;                          // +0x04
    unsigned short FrameWidth;                     // +0x06
    unsigned short FrameHeight;                    // +0x08
    unsigned short FrameCount;                     // +0x0a
};
class CGraphicInfo { public:
    CGraphicHeader Header;                         // +0x00
};
class CGraphicInfoFrame {
public:
    void*          __vftable;                      // +0x00
    unsigned short FrameNum;                       // +0x04
    signed char    SmallMipmapLevel;               // +0x06
    void*          Data;                           // +0x08 CIVCountedPointer<CFrameData>
    CGraphicInfo*  GraphicInfo;                    // +0x0c
};
struct SPrimitiveRendererSettings {                // the record behind the pointer at 0x01436E78
    unsigned char _pad[0xb4];
    unsigned long PreloadMipmapBias;               // +0xb4
    unsigned long MaxPreloadTextureSize;           // +0xb8
};
extern SPrimitiveRendererSettings* g_pPrimitiveRenderer;   // 0x01436E78

class CEnginePrimitiveManagerMeshBase {
    static unsigned long __fastcall GetAcceptablePreloadMipmap(unsigned long mipmap, const CGraphicInfoFrame* frame);
};

unsigned long __fastcall CEnginePrimitiveManagerMeshBase::GetAcceptablePreloadMipmap(unsigned long mipmap, const CGraphicInfoFrame* frame)
{
    unsigned long size;
    if (frame->GraphicInfo->Header.FrameWidth > frame->GraphicInfo->Header.FrameHeight)
        size = frame->GraphicInfo->Header.FrameWidth;
    else
        size = frame->GraphicInfo->Header.FrameHeight;
    size >>= mipmap;
    unsigned long level = mipmap;
    while (size > g_pPrimitiveRenderer->MaxPreloadTextureSize) {
        size >>= 1;
        ++level;
    }
    return g_pPrimitiveRenderer->PreloadMipmapBias + level;
}