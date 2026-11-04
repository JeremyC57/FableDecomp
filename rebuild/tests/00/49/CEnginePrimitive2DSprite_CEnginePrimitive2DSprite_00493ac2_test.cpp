// Behaviour test for CEnginePrimitive2DSprite::CEnginePrimitive2DSprite (0x00493AC2): a model of the row -- type 0x22 with the
// interpolation set and cleared channel bytes, both clip-box corners at the position, the scalar members copied, the colour
// built from (red, green, blue, alpha), ClipBoxIsPos set, no texture, and the two UV offsets copied.
#include <stdio.h>
struct C2DVector { float X, Y; };
struct C2DBoxF { float TLX, TLY, BRX, BRY; C2DBoxF(const C2DVector& a, const C2DVector& b) : TLX(a.X), TLY(a.Y), BRX(b.X), BRY(b.Y) {} };
struct CRGBColour { unsigned char B, G, R, A; CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; } };
struct CEnginePrimitive { long Type, InterpolationSet; unsigned char C0, C1, CB, Pad;
    CEnginePrimitive(long t, long i) : Type(t), InterpolationSet(i), C0(0), C1(0), CB(0) {} };
struct CEnginePrimitive2DSprite : CEnginePrimitive {
    C2DBoxF ClipBox; long Layer; unsigned long BankIndex, Frame; float StartAngle, EndAngle; long BlendMode; CRGBColour Colour;
    bool ClipBoxIsPos; unsigned long Flags; void* Texture; C2DVector UVOffsetsStart, UVOffsetsEnd;
    CEnginePrimitive2DSprite(unsigned long long, const C2DVector& p, long layer, unsigned long bank, unsigned long frame, long blend,
        long interp, float s, float e, unsigned char a, unsigned char r, unsigned char g, unsigned char b, unsigned long flags,
        const C2DVector& uv0, const C2DVector& uv1)
        : CEnginePrimitive(0x22, interp), ClipBox(p, p), Layer(layer), BankIndex(bank), Frame(frame), StartAngle(s), EndAngle(e),
          BlendMode(blend), Colour(r, g, b, a), ClipBoxIsPos(true), Flags(flags), Texture(0), UVOffsetsStart(uv0), UVOffsetsEnd(uv1) {}
};
int main()
{
    C2DVector p = { 10.0f, 20.0f }, u0 = { 0.25f, 0.5f }, u1 = { 0.75f, 1.0f };
    CEnginePrimitive2DSprite s(99, p, 3, 7, 2, 1, 5, 0.5f, 1.5f, 200, 10, 20, 30, 0x40, u0, u1);
    int ok = s.Type == 0x22 && s.InterpolationSet == 5 && s.C0 == 0 && s.CB == 0 && s.ClipBox.TLX == 10.0f && s.ClipBox.BRY == 20.0f
        && s.Layer == 3 && s.BankIndex == 7 && s.Frame == 2 && s.StartAngle == 0.5f && s.EndAngle == 1.5f && s.BlendMode == 1
        && s.Colour.A == 200 && s.Colour.R == 10 && s.Colour.G == 20 && s.Colour.B == 30 && s.ClipBoxIsPos && s.Flags == 0x40
        && s.Texture == 0 && s.UVOffsetsStart.Y == 0.5f && s.UVOffsetsEnd.X == 0.75f;
    printf(ok ? "00493ac2_TEST PASS\n" : "00493ac2_TEST FAIL\n");
    return ok ? 0 : 1;
}