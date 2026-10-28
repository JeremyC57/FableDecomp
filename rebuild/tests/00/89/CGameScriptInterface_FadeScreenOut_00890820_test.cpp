extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
// Behaviour test for 0x00890820 CGameScriptInterface::FadeScreenOut: when the screen is already faded out the fade restarts
// only if AllowScreenFadingIfAlreadyFaded (else false); otherwise it starts; true whenever it starts.
class CRGBColour { public: unsigned char R, G, B, A; };
struct CGSI {
    bool fadedOut, allow; int starts; float lastFade, lastHold; CRGBColour lastColour;
    void StartFadeScreen(float f, float h, CRGBColour c) { ++starts; lastFade = f; lastHold = h; lastColour = c; }
    bool FadeScreenOut(float f, float h, CRGBColour c)
    {
        if (fadedOut) { if (allow) StartFadeScreen(f, h, c); else return false; }
        else StartFadeScreen(f, h, c);
        return true;
    }
};
int main()
{
    CRGBColour black = { 0, 0, 0, 255 };
    CGSI g; g.fadedOut = false; g.allow = false; g.starts = 0;
    CHECK(g.FadeScreenOut(1.0f, 2.0f, black) && g.starts == 1 && g.lastFade == 1.0f && g.lastHold == 2.0f && g.lastColour.A == 255);
    g.fadedOut = true; CHECK(!g.FadeScreenOut(1.0f, 2.0f, black) && g.starts == 1);
    g.allow = true; CHECK(g.FadeScreenOut(3.0f, 0.0f, black) && g.starts == 2 && g.lastFade == 3.0f);
    if (fails == 0) printf("00890820_TEST PASS\n"); else printf("00890820_TEST FAIL\n");
    return fails;
}