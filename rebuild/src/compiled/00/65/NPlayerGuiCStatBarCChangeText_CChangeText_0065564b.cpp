// NPlayerGui::CStatBar::CChangeText::CChangeText(const CChangeText& other) -- copy a floating stat-change text member by member
// Leaf: ??0CChangeText@CStatBar@NPlayerGui@@QAE@ABV012@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065564B, 93 bytes, thiscall, `ret 4`, /O1 TU.
// Identity (proven): Ego_r.pdb ??0CChangeText@CStatBar@NPlayerGui@@QAE@ABV012@@Z (written out member-wise, which mangles and compiles as the
//   implicit copy constructor the list<CChangeText> node construction needs); the member-by-member copy follows the PDB layout (CWideString copy ctor at +0x10,
//   the colour's byte-wise copy constructor at +0x18). The catalog spells both rows CChangeText::CChangeText.
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


CStatBar::CChangeText::CChangeText(const CChangeText& other)
    : OldOffset(other.OldOffset), Offset(other.Offset), Text(other.Text), Value(other.Value), Colour(other.Colour),
      StartToEnd(other.StartToEnd), PercentageThrough(other.PercentageThrough)
{
}

}