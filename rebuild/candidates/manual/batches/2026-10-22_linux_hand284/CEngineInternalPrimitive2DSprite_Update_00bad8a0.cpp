// CEngineInternalPrimitive2DSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a 2D sprite descriptor (clip box, layer, bank/frame, angles, blend mode, colour, interpolation set, texture, filter bit, UV offsets), derive WrapTexture and the render layer mask from the flags, start loading the bank graphic and grow the clip box by the frame / texture size when the box is a position
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAD8A0, length 671 bytes (`ret 8`, virtual `?Update@CEngineInternalPrimitive2DSprite@@UAEXABVCEnginePrimitive@@K@Z`).
// Callees: CGraphicInfoFrame::BeginLoading(unsigned long, unsigned long) @ 0x009FE620, CTexture::GetDimensions(unsigned long)
// @ 0x009F9DB0 (C2DExtentsI by hidden pointer). Globals: the engine @ 0x01436E98 (graphic bank at +0x954, its 0x2c-byte
// CGraphicInfo vector at +0x1e0), the UV epsilon float @ 0x0129BA3C (0.0001f, squared once and spilled).
// Layout: base = engine/CEngineInternalPrimitiveBase.h (EnginePrimitiveFlags +0x0c, InterpolationSet +0x2c, RenderLayerMask
// +0x30); the sprite members are those of engine/CEngineInternalPrimitive2DSprite.h at the same (pack 1, odd UV) offsets,
// re-declared on a derived shell because the generated header flattens the base into `_base_0x04` (as the landed
// CEngineInternalPrimitiveLine::Update 0x00BC7F70 does). The descriptor is the PDB CEnginePrimitive2DSprite (84 bytes);
// CGraphicInfo (0x2c: Header with FrameWidth +6 / FrameHeight +8, Frames +0x28) and CGraphicInfoFrame (16 bytes,
// GraphicInfo +0x0c) follow the PDB. The method is attached through the shell, so it is graded non-virtual.
// Shape: the colour goes through a named CRGBColour local (a direct member copy shifts the eax/edx alternation of every
// later copy by one); WrapTexture is `start.IsNonZero() || end.IsNonZero()` (`lenSq > eps*eps`, `test ah,41h ; je`); the
// frame pointer is `Get(i) ? &Get(i)->Frames[frame] : 0` over the __forceinline bounds-checked bank getter; the centre
// case halves the ushort sizes into `unsigned long` locals (unsigned fild fix-up, both conversions kept on the x87 stack);
// the texture case needs `unsigned long` locals for both dimensions (else VC filds the first straight from the result)
// and an inline GetTexture() accessor (a plain member read loads the second call's `this` after its pushes).
#include "engine/CEngineInternalPrimitiveBase.h"

extern const float g_fUVOffsetEpsilon;               // 0x0129BA3C (0.0001f)
struct C2DVector {
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
    bool IsNonZero() const { return GetLengthSquared() > g_fUVOffsetEpsilon * g_fUVOffsetEpsilon; }
};
struct C2DBoxF { float TLX, TLY, BRX, BRY; };
struct CRGBColour { unsigned char B, G, R, A; };
struct C2DExtentsI { long X, Y; };
struct CTexture {
    C2DExtentsI GetDimensions(unsigned long level);   // 0x009F9DB0
};
class CEnginePrimitive {
public:
    long          Type;
    long          InterpolationSet;
    unsigned long ThemeChannelDesc;
};
class CEnginePrimitive2DSprite : public CEnginePrimitive {   // PDB, 84 bytes
public:
    C2DBoxF       ClipBox;          // +0x0c
    long          Layer;            // +0x1c
    unsigned long BankIndex;        // +0x20
    unsigned long Frame;            // +0x24
    float         StartAngle;       // +0x28
    float         EndAngle;         // +0x2c
    long          BlendMode;        // +0x30
    CRGBColour    Colour;           // +0x34
    bool          ClipBoxIsPos;     // +0x38
    unsigned long Flags;            // +0x3c
    CTexture*     Texture;          // +0x40
    C2DVector     UVOffsetsStart;   // +0x44
    C2DVector     UVOffsetsEnd;     // +0x4c
};
struct CGraphicHeader {
    unsigned short Width, Height, Depth;
    unsigned short FrameWidth;      // +0x06
    unsigned short FrameHeight;     // +0x08
};
struct CGraphicInfo;
struct CGraphicInfoFrame {          // 16 bytes
    unsigned char _pad[0xc];
    CGraphicInfo* GraphicInfo;      // +0x0c
    void BeginLoading(unsigned long priority, unsigned long flags);   // 0x009FE620
};
struct CGraphicInfo {               // 0x2c bytes
    CGraphicHeader     Header;
    unsigned char      _pad[0x28 - sizeof(CGraphicHeader)];
    CGraphicInfoFrame* Frames;      // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<CGraphicInfo> m_records;   // +0x1e0
    __forceinline CGraphicInfo* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    __forceinline CGraphicInfoFrame* GetFrame(unsigned long index, unsigned long frame) { return Get(index) ? &Get(index)->Frames[frame] : 0; }
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;   // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98

#pragma pack(push, 1)
struct CEngineInternalPrimitive2DSprite : CEngineInternalPrimitiveBase {
    C2DBoxF       ClipBox;          // +0x48
    long          Layer;            // +0x58
    unsigned long BankIndex;        // +0x5c
    unsigned long Frame;            // +0x60
    float         StartAngle;       // +0x64
    float         EndAngle;         // +0x68
    CRGBColour    Colour;           // +0x6c
    CTexture*     Texture;          // +0x70
    bool          Filter;           // +0x74
    C2DVector     UVOffsetsStart;   // +0x75
    C2DVector     UVOffsetsEnd;     // +0x7d
    bool          WrapTexture;      // +0x85
    unsigned char _pad_0x86[2];
    long          BlendMode;        // +0x88
    CTexture* GetTexture() const { return Texture; }
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};
#pragma pack(pop)

void CEngineInternalPrimitive2DSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    const CEnginePrimitive2DSprite& sprite = static_cast<const CEnginePrimitive2DSprite&>(primitive);
    ClipBox = sprite.ClipBox;
    Layer = sprite.Layer;
    BankIndex = sprite.BankIndex;
    Frame = sprite.Frame;
    StartAngle = sprite.StartAngle;
    EndAngle = sprite.EndAngle;
    BlendMode = sprite.BlendMode;
    CRGBColour colour = sprite.Colour;
    Colour = colour;
    *reinterpret_cast<long*>(InterpolationSet) = primitive.InterpolationSet;
    Texture = sprite.Texture;
    Filter = (sprite.Flags >> 1) & 1;
    UVOffsetsStart = sprite.UVOffsetsStart;
    UVOffsetsEnd = sprite.UVOffsetsEnd;
    WrapTexture = UVOffsetsStart.IsNonZero() || UVOffsetsEnd.IsNonZero();
    if (flags & 0x80)
        EnginePrimitiveFlags |= 0x80;
    RenderLayerMask = (flags & 0x400) ? 0x00200000 : 0x80000000;
    if (BankIndex) {
        CGraphicInfoFrame* frame = g_pEngine_01436e98->m_pGraphicBank->GetFrame(BankIndex, Frame);
        frame->BeginLoading(0, 0);
        if (sprite.ClipBoxIsPos) {
            if (sprite.Flags & 1) {
                unsigned long halfWidth = frame->GraphicInfo->Header.FrameWidth >> 1;
                unsigned long halfHeight = frame->GraphicInfo->Header.FrameHeight >> 1;
                ClipBox.TLX -= halfWidth;
                ClipBox.TLY -= halfHeight;
                ClipBox.BRX += halfWidth;
                ClipBox.BRY += halfHeight;
            } else {
                ClipBox.BRX += frame->GraphicInfo->Header.FrameWidth;
                ClipBox.BRY += frame->GraphicInfo->Header.FrameHeight;
            }
        }
    } else if (sprite.Texture && sprite.ClipBoxIsPos) {
        unsigned long width = GetTexture()->GetDimensions(0).X;
        ClipBox.BRX += width;
        unsigned long height = GetTexture()->GetDimensions(0).Y;
        ClipBox.BRY += height;
    }
}
