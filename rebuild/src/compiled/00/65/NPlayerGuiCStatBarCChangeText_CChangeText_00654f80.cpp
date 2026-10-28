// NPlayerGui::CStatBar::CChangeText::CChangeText(const C2DVector& offset, const CRGBColour& colour, const CWideString& text, long value, bool startToEnd) -- a floating stat-change text at an offset (old and current offset both start there), not yet moved
// Leaf: ??0CChangeText@CStatBar@NPlayerGui@@QAE@ABVC2DVector@@ABVCRGBColour@@ABVCWideString@@J_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00654F80, 93 bytes, thiscall, ebp frame, `ret 14h`, /O1 TU.
// Identity (proven): Ego_r.pdb ??0CChangeText@CStatBar@NPlayerGui@@QAE@ABVC2DVector@@ABVCRGBColour@@ABVCWideString@@J_N@Z; five arguments
//   (`ret 14h`) and the stores match the PDB layout exactly (OldOffset/Offset from the first argument, Text +0x10 copied from the third,
//   Value +0x14 from the fourth, Colour +0x18 from the second, StartToEnd +0x1c from the fifth, PercentageThrough +0x20 = 0.0f).
// Callees: CWideString::CWideString(const CWideString&) 0x0099B720.
// Shape: the colour copy is `CRGBColour(const CRGBColour&) { Set(R, G, B, A); }` with a __forceinline four-byte Set: all four loads come
//   before the stores (a member-wise copy interleaves them); /O1 inlines only __forceinline helpers.
#pragma optimize("s", on)
class C2DVector { public: float X, Y; };
class CRGBColour {
public:
    unsigned char B, G, R, A;
    __forceinline CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
    __forceinline void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        R = r;
        G = g;
        B = b;
        A = a;
    }
};
class CWideString {
public:
    CWideString(const CWideString& other);         // 0x0099B720
    void* PData;
};

namespace NPlayerGui {
class CStatBar {
public:
    class CChangeText {                            // Ego_r.pdb, 0x24 bytes
    public:
        C2DVector  OldOffset;                      // +0x00
        C2DVector  Offset;                         // +0x08
        CWideString Text;                          // +0x10
        long       Value;                          // +0x14
        CRGBColour Colour;                         // +0x18
        bool       StartToEnd;                     // +0x1c
        float      PercentageThrough;              // +0x20
        CChangeText(const C2DVector& offset, const CRGBColour& colour, const CWideString& text, long value, bool startToEnd);
        CChangeText(const CChangeText& other);
    };
};

CStatBar::CChangeText::CChangeText(const C2DVector& offset, const CRGBColour& colour, const CWideString& text, long value, bool startToEnd)
    : OldOffset(offset), Offset(offset), Text(text), Value(value), Colour(colour), StartToEnd(startToEnd), PercentageThrough(0.0f)
{
}

}