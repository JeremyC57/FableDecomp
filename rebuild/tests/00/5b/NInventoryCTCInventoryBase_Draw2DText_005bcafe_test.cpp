// Behaviour test for NInventory::CTCInventoryBase::Draw2DText (0x005BCAFE): without the drop shadow only the text is
// drawn, at the pixel-rounded position in the text colour; with it three black copies carrying the colour's alpha are
// drawn first at (x+1, y+1), (x+1, y-1), (x-1, y+1) pixels, then the text -- through a copy of the body over recording
// stand-ins for the render manager and the coordinate helpers.
#include <cstdio>

class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct Draw { C3DVector position; float scale; int text; int font; CRGBColour colour; unsigned int flags; };

static Draw g_draws[8];
static int g_drawCount = 0;

struct RenderManager {
    bool Draw3DScaledText(const C3DVector& position, float scale, int text, int font, const CRGBColour& colour,
                          unsigned int flags)
    {
        Draw d = { position, scale, text, font, colour, flags };
        g_draws[g_drawCount++] = d;
        return true;
    }
};
static RenderManager g_renderManager;
static RenderManager* g_pRenderManager2D = &g_renderManager;

// one absolute unit is 0.5 virtual units in X and 0.25 in Y; rounding snaps to quarter units
static float GFAXToVX(float x) { return x * 0.5f; }
static float GFAYToVY(float y) { return y * 0.25f; }
static float Snap(double v) { return (float)((double)(long)(v * 4.0 + (v < 0 ? -0.5 : 0.5)) / 4.0); }
static float GFRoundVXToNearestPixel(double x) { return Snap(x); }
static float GFRoundVYToNearestPixel(double y) { return Snap(y); }

static void Draw2DText(const C2DVector& position, int text, int font, const CRGBColour& colour, float scale,
                       bool dropShadow)
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

static bool Is(const Draw& d, float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    return d.position.X == x && d.position.Y == y && d.position.Z == 0.0f && d.scale == 1.5f && d.text == 11 &&
           d.font == 22 && d.colour.R == r && d.colour.G == g && d.colour.B == b && d.colour.A == a && d.flags == 0;
}

int main()
{
    C2DVector position = { 10.1f, 20.0f };
    CRGBColour colour(200, 150, 100, 0x80);
    Draw2DText(position, 11, 22, colour, 1.5f, false);
    if (g_drawCount != 1 || !Is(g_draws[0], 10.0f, 20.0f, 200, 150, 100, 0x80)) {
        std::printf("005bcafe_TEST FAIL\n");
        return 1;
    }
    g_drawCount = 0;
    Draw2DText(position, 11, 22, colour, 1.5f, true);
    if (g_drawCount != 4 || !Is(g_draws[0], 10.5f, 20.25f, 0, 0, 0, 0x80) || !Is(g_draws[1], 10.5f, 19.75f, 0, 0, 0, 0x80) ||
        !Is(g_draws[2], 9.5f, 20.25f, 0, 0, 0, 0x80) || !Is(g_draws[3], 10.0f, 20.0f, 200, 150, 100, 0x80)) {
        std::printf("005bcafe_TEST FAIL\n");
        return 1;
    }
    std::printf("005bcafe_TEST PASS\n");
    return 0;
}