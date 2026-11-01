// NShape::DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info, const CWorldMap& worldMap, const C3DVector& start, const C3DVector& end, const C3DVector& focus, const CRGBColour& colour) -- editor helper: draws the start->end line as a chain of ground-hugging segments (each vertex lifted 0.15 above the landscape), the segment length growing with the distance of the line's midpoint from the focus point (tanh(d * 0.05) * 5 + 0.25)
// Grade: DIFFER(733v733) -- one scheduling residue: the segment count's spill store (`mov [esp+20h],edi`) is emitted before the `previous = start` copy where retail emits it after the GetGroundSizeZAt argument push; every other byte matches
// Leaf: ?DrawPrimitiveLineOverLandscape@NShape@@YIXABVCEnginePrimitivePrepareInfo@@ABVCWorldMap@@ABVC3DVector@@22ABVCRGBColour@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00761280, 733 bytes, __fastcall (info in ecx, worldMap in edx, four stack references,
//   `ret 10h`). Ego_r.pdb names the free function in namespace NShape; the donor order (0x00770EA0, then the sole caller
//   NShape::CTrack::EditorDrawPreparePrimitivesForRendering 0x00771220) is retail's 0x00761280 / 0x00761560. Callees: the
//   tanh intrinsic _CItanh 0x00BFECE2, __ftol2 0x00BFEA70, CWorldMap::GetGroundSizeZAt 0x0063A300 (x2), the engine's
//   primitive slot 20 (+0x50) through CEnginePrimitivePrepareInfo::Engine (+0x34). Constants: 0.5f 0x0122F59C, 0.05f
//   0x0125A15C, 0.15f 0x0126A1F4, 1.0f 0x0122DED8, doubles 5.0 0x01231678 and 0.25 0x01238010.
// Layout / models: the landed NShape rows (CShape::EditorDrawHighlightPoints 0x007617A0: CEnginePrimitiveLine 0x3C with the
//   inline-constructed Type 6 / theme bytes / Width 0.05f / UseZBuffer, CRGBColour copy ctor assigning R, G, B, A; CTrack::
//   GetTrackPosAtLengthFraction 0x00760C40: the C3DVector operators with the reciprocal operator/). Length() sums Z, Y, X
//   in that order (both fsqrt chains start with the Z product).
// Shape: the segment point is a conditional expression (`last ? end : start + ...`: VC7.1 forms the temporary in its own
//   slot and copy-constructs `point` from a pointer to either); the loop is `i != count` on a signed count (the entry test
//   is `cmp count,0 ; je`, `(float)(i + 1)` is a plain fild). Probes: 12 (probe/00761280.log).
#include <math.h>
#pragma intrinsic(sqrt, tanh)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator/(float f) const { f = 1.0f / f; return C3DVector(X * f, Y * f, Z * f); }
    float Length() const { return sqrt(Z * Z + Y * Y + X * X); }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(const CRGBColour& c) { R = c.R; G = c.G; B = c.B; A = c.A; }
};
class CEnginePrimitiveThemeChannelDesc { public:
    bool Channel0, Channel1, Channel2;
    CEnginePrimitiveThemeChannelDesc() : Channel0(false), Channel1(false), Channel2(false) {}
};
class CEnginePrimitive { public:
    long                             Type;              // +0x00
    long                             InterpolationSet;  // +0x04
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;  // +0x08
    CEnginePrimitive(long type, long interpolation) : Type(type), InterpolationSet(interpolation) {}
};
class CEnginePrimitiveLine : public CEnginePrimitive { public:
    C3DVector     Start;                                // +0x0c
    C3DVector     End;                                  // +0x18
    CRGBColour    Colour;                               // +0x24
    float         Width;                                // +0x28
    unsigned long BankIndex;                            // +0x2c
    long          BlendMode;                            // +0x30
    long          BlendOp;                              // +0x34
    bool          UseZBuffer;                           // +0x38
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour,
                         float width, long blendMode, unsigned long flags, bool depthTest, long interpolation, long blendOp)
        : CEnginePrimitive(6, interpolation), Start(start), End(end), Colour(colour), Width(width), BankIndex(flags),
          BlendMode(blendMode), BlendOp(blendOp), UseZBuffer(depthTest) {}
};
class CIEngine { public:
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16)
    VF(17) VF(18) VF(19)
#undef VF
    virtual void SetPrimitive(const CEnginePrimitive& primitive);   // slot 20 (+0x50)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CIEngine* Engine;                                   // +0x34
};
class CWorldMap { public:
    float GetGroundSizeZAt(const C3DVector& localPos) const;  // 0x0063A300
};
namespace NShape {
void __fastcall DrawPrimitiveLineOverLandscape(const CEnginePrimitivePrepareInfo& info, const CWorldMap& worldMap,
    const C3DVector& start, const C3DVector& end, const C3DVector& focus, const CRGBColour& colour)
{
    C3DVector mid = (start + end) * 0.5f;
    float distance = (mid - focus).Length();
    float step = (float)(tanh(distance * 0.05f) * 5.0 + 0.25);
    C3DVector delta = end - start;
    float length = delta.Length();
    long count = (long)(length / step) + 1;
    C3DVector previous = start;
    previous.Z = worldMap.GetGroundSizeZAt(previous) + 0.15f;
    for (long i = 0; i != count; ++i) {
        C3DVector point = (i == count - 1) ? end : start + delta * step * (float)(i + 1) / length;
        point.Z = worldMap.GetGroundSizeZAt(point) + 0.15f;
        info.Engine->SetPrimitive(CEnginePrimitiveLine(0, previous, point, colour, 0.05f, 0, 0, true, 0, 0));
        previous = point;
    }
}
}
