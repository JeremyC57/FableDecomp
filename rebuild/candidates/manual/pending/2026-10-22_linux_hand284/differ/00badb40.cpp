// CEngineInternalPrimitive2DClockSprite::Update(const CEnginePrimitive& primitive, unsigned long flags) -- copy a 2D clock-sprite descriptor (clip box, layer, both bank/frame pairs, angles, blend mode, colours, interpolation set, filter bit, UV offsets), derive WrapTexture, start loading both bank graphics and grow the clip box by the larger frame size
// Grade: DIFFER(764v764) -- eax/edx phase swap on the Colour2 (+0x40) and InterpolationSet (+0x4) copies, 4 lines; everything else byte-exact
// Leaf: Update
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
#pragma pack(push, 1)
class CEnginePrimitive2DClockSprite : public CEnginePrimitive {   // PDB, 96 bytes
public:
    C2DBoxF       ClipBox;          // +0x0c
    long          Layer;            // +0x1c
    unsigned long BankIndex;        // +0x20
    unsigned long Frame;            // +0x24
    unsigned long BankIndex2;       // +0x28
    unsigned long Frame2;           // +0x2c
    float         AngleStart;       // +0x30
    float         AngleEnd;         // +0x34
    long          BlendMode;        // +0x38
    CRGBColour    Colour;           // +0x3c
    CRGBColour    Colour2;          // +0x40
    bool          ClipBoxIsPos;     // +0x44
    unsigned char _pad_0x45[3];
    unsigned long Flags;            // +0x48
    bool          Filter;           // +0x4c
    C2DVector     UVOffsetsStart;   // +0x4d
    C2DVector     UVOffsetsEnd;     // +0x55
};
#pragma pack(pop)
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
struct CEngineInternalPrimitive2DClockSprite : CEngineInternalPrimitiveBase {
    C2DBoxF       ClipBox;          // +0x48
    long          Layer;            // +0x58
    unsigned long BankIndex;        // +0x5c
    unsigned long Frame;            // +0x60
    CRGBColour    Colour;           // +0x64
    unsigned long BankIndex2;       // +0x68
    unsigned long Frame2;           // +0x6c
    CRGBColour    Colour2;          // +0x70
    float         AngleStart;       // +0x74
    float         AngleEnd;         // +0x78
    bool          Filter;           // +0x7c
    C2DVector     UVOffsetsStart;   // +0x7d
    C2DVector     UVOffsetsEnd;     // +0x85
    bool          WrapTexture;      // +0x8d
    unsigned char _pad_0x8e[2];
    long          BlendMode;        // +0x90
    void Update(const CEnginePrimitive& primitive, unsigned long flags);
};
#pragma pack(pop)

void CEngineInternalPrimitive2DClockSprite::Update(const CEnginePrimitive& primitive, unsigned long flags)
{
    EnginePrimitiveFlags = flags;
    const CEnginePrimitive2DClockSprite& sprite = static_cast<const CEnginePrimitive2DClockSprite&>(primitive);
    ClipBox = sprite.ClipBox;
    Layer = sprite.Layer;
    BankIndex = sprite.BankIndex;
    Frame = sprite.Frame;
    BankIndex2 = sprite.BankIndex2;
    Frame2 = sprite.Frame2;
    AngleStart = sprite.AngleStart;
    AngleEnd = sprite.AngleEnd;
    BlendMode = sprite.BlendMode;
    Colour = sprite.Colour;
    CRGBColour colour2 = sprite.Colour2;
    Colour2 = colour2;
    *reinterpret_cast<long*>(InterpolationSet) = primitive.InterpolationSet;
    Filter = (sprite.Flags >> 1) & 1;
    UVOffsetsStart = sprite.UVOffsetsStart;
    UVOffsetsEnd = sprite.UVOffsetsEnd;
    WrapTexture = UVOffsetsStart.IsNonZero() || UVOffsetsEnd.IsNonZero();
    if (flags & 0x80)
        EnginePrimitiveFlags |= 0x80;
    if (BankIndex || BankIndex2) {
        unsigned long width = 0;
        unsigned long height = 0;
        if (BankIndex) {
            CGraphicInfoFrame* frame = g_pEngine_01436e98->m_pGraphicBank->GetFrame(BankIndex, Frame);
            frame->BeginLoading(0, 0);
            CGraphicInfo* info = frame->GraphicInfo;
            height = info->Header.FrameHeight;
            width = info->Header.FrameWidth;
        }
        if (BankIndex2) {
            CGraphicInfoFrame* frame = g_pEngine_01436e98->m_pGraphicBank->GetFrame(BankIndex2, Frame2);
            frame->BeginLoading(0, 0);
            CGraphicInfo* info = frame->GraphicInfo;
            if (info->Header.FrameWidth > width)
                width = info->Header.FrameWidth;
            if (info->Header.FrameHeight > height)
                height = info->Header.FrameHeight;
        }
        if (sprite.ClipBoxIsPos) {
            if (sprite.Flags & 1) {
                unsigned long halfWidth = width >> 1;
                unsigned long halfHeight = height >> 1;
                ClipBox.TLX -= halfWidth;
                ClipBox.TLY -= halfHeight;
                ClipBox.BRX += halfWidth;
                ClipBox.BRY += halfHeight;
            } else {
                ClipBox.BRX += width;
                ClipBox.BRY += height;
            }
        }
    }
}
