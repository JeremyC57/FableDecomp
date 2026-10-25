// NInventory::CTCInventoryBase::Draw2DText(const C2DVector& position, const CWideString& text, const CFontBank& font, const CRGBColour& colour, float scale, bool dropShadow) -- draws inventory text through the 2D render manager at the pixel-rounded position, after three black one-pixel drop-shadow copies (x+1/y+1, x+1/y-1, x-1/y+1) carrying the text colour's alpha when asked
// Leaf: ?Draw2DText@CTCInventoryBase@NInventory@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x005BCAFE, 499 bytes (0x1F3, no padding), static __fastcall `ret 10h` (position in
//   ecx, text in edx). Six callers (0x005BCE9B, 0x0061AFEC, 0x0061B141, 0x0061B1B7, 0x0063A127, 0x0063A1D3).
// Identity: Ego_r.pdb ?Draw2DText@CTCInventoryBase@NInventory@@SIXABVC2DVector@@ABVCWideString@@ABVCFontBank@@
//   ABVCRGBColour@@M_N@Z (static, __fastcall); the catalog's bare "Draw2DText" lacks the class.
// Callees: GFAXToVX 0x009E1DA0 / GFAYToVY 0x009E1DD0 (__fastcall float(float)), GFRoundVXToNearestPixel 0x009E1D00 /
//   GFRoundVYToNearestPixel 0x009E1D50 (__fastcall float(double)), CWideString(const CWideString&) 0x0099B720 (the
//   by-value text argument; the callee destroys it), CRenderManager2D::Draw3DScaledText 0x009DBFF0 (landed).
// Globals: the render manager 0x013B8384 (the engine's CRenderManagerCore, whose CRenderManager2D base is at +0; the
//   landed ChangeMaxTextureSize 0x004179EE reads its display caps).
// Layout: C2DVector {X, Y}, C3DVector {X, Y, Z}, CRGBColour in D3DCOLOR byte order {B, G, R, A} (as the landed
//   CIMEFont::DrawTextW 0x00405020).
// Shape: /Os frame-pointer TU. The one-pixel offsets are computed once; each shadow copy assigns a fresh C3DVector
//   (arguments right to left, so Y rounds first) into one named shadow position (temporary + copy), and the black
//   colour temporary takes the dead `dropShadow` slot; the final draw's position is a named function-scope
//   C3DVector (its own frame slot below the shadow temporary; as a temporary it shares the shadow temporary's slot).
#pragma optimize("s", on)
#pragma optimize("y", off)

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                                             // 0x0099B720
    ~CWideString();                                                                    // 0x0099B510
};

class CFontBank;

class C2DVector {
public:
    float X, Y;
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};

class CRGBColour {
public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};

class CRenderManager2D {
public:
    bool Draw3DScaledText(const C3DVector& position, float scale, CWideString text, const CFontBank& font,
                          const CRGBColour& colour, unsigned long flags);                 // 0x009DBFF0
};

extern CRenderManager2D* g_pRenderManager2D;                                          // 0x013B8384

float __fastcall GFAXToVX(float x);                                                   // 0x009E1DA0
float __fastcall GFAYToVY(float y);                                                   // 0x009E1DD0
float __fastcall GFRoundVXToNearestPixel(double x);                                   // 0x009E1D00
float __fastcall GFRoundVYToNearestPixel(double y);                                   // 0x009E1D50

namespace NInventory {
class CTCInventoryBase {
public:
    static void __fastcall Draw2DText(const C2DVector& position, const CWideString& text, const CFontBank& font,
                                      const CRGBColour& colour, float scale, bool dropShadow);
};

void __fastcall CTCInventoryBase::Draw2DText(const C2DVector& position, const CWideString& text, const CFontBank& font,
                                             const CRGBColour& colour, float scale, bool dropShadow)
{
    if (dropShadow) {
        float pixelWidth = GFAXToVX(1.0f);
        float pixelHeight = GFAYToVY(1.0f);
        C3DVector shadowPosition;
        shadowPosition = C3DVector(GFRoundVXToNearestPixel(pixelWidth + position.X),
                                   GFRoundVYToNearestPixel(pixelHeight + position.Y), 0.0f);
        g_pRenderManager2D->Draw3DScaledText(shadowPosition, scale, text, font, CRGBColour(0, 0, 0, colour.A), 0);
        shadowPosition = C3DVector(GFRoundVXToNearestPixel(pixelWidth + position.X),
                                   GFRoundVYToNearestPixel(position.Y - pixelHeight), 0.0f);
        g_pRenderManager2D->Draw3DScaledText(shadowPosition, scale, text, font, CRGBColour(0, 0, 0, colour.A), 0);
        shadowPosition = C3DVector(GFRoundVXToNearestPixel(position.X - pixelWidth),
                                   GFRoundVYToNearestPixel(pixelHeight + position.Y), 0.0f);
        g_pRenderManager2D->Draw3DScaledText(shadowPosition, scale, text, font, CRGBColour(0, 0, 0, colour.A), 0);
    }
    C3DVector drawPosition(GFRoundVXToNearestPixel(position.X), GFRoundVYToNearestPixel(position.Y), 0.0f);
    g_pRenderManager2D->Draw3DScaledText(drawPosition, scale, text, font, colour, 0);
}
}