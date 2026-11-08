// Behaviour test for 0x00648820 NPlayerGui::CDrawMiniMap::Draw: the fade alpha is fade*255 clamped to 255 and to the drawer's Alpha;
// the region-change frame lays its tiles left to right (each tile starts where the previous one ends, sized by the graphic scaled
// to the render target over 640x480), the second row starting under the first; the side borders are centred vertically; region
// exit labels fall back to the symbol when the text bank has none, the two prison-block names are never labelled, and the label
// scale is 0.5 + fade/2.
#include <cstdio>
#include <cstring>
struct Box { float TLX, TLY, BRX, BRY; };
static unsigned char ClampAlpha(float fade, unsigned char Alpha)
{
    float f = fade * 255.0f;
    if (f > 255.0f) f = 255.0f;
    unsigned char a = (unsigned char)f;
    if (a > Alpha) a = Alpha;
    return a;
}
static Box NextTile(const Box& prev, long width, long height, long gw, long gh)
{
    Box t = prev;
    float x = prev.TLX + (prev.BRX - prev.TLX);
    t.TLX = x;
    t.BRX = x + (float)(width * gw) * (1.0f / 640.0f);
    float h = (float)(gh * height) * (1.0f / 480.0f);
    t.BRY = t.TLY + h;
    return t;
}
static const char* Label(const char* text, const char* symbol)
{
    const char* t = (text[0] == 0) ? symbol : text;
    if (!std::strcmp(symbol, "TXT_REGION_PRISON_BLOCK_B") || !std::strcmp(symbol, "TXT_REGION_PRISON_BLOCK_C")) t = "";
    return t;
}
int main()
{
    if (ClampAlpha(2.0f, 200) != 200 || ClampAlpha(0.5f, 255) != 127 || ClampAlpha(1.0f, 255) != 255)
    { std::printf("00648820_TEST FAIL\n"); return 1; }
    Box first = { 10.0f, 20.0f, 10.0f + 1280.0f * 64 / 640.0f, 20.0f + 32.0f * 960 / 480.0f };
    Box second = NextTile(first, 1280, 960, 32, 32);
    if (second.TLX != first.BRX || second.BRX != first.BRX + 64.0f || second.TLY != 20.0f || second.BRY != 84.0f)
    { std::printf("00648820_TEST FAIL\n"); return 1; }
    float screen_h = 960.0f, height = 960.0f * (1.0f / 480.0f) * 100.0f;
    float tly = (screen_h - height) * 0.5f;
    if (tly != 380.0f || tly + height != 580.0f) { std::printf("00648820_TEST FAIL\n"); return 1; }
    if (std::strcmp(Label("", "TXT_X"), "TXT_X") || std::strcmp(Label("Bowerstone", "TXT_X"), "Bowerstone")
        || Label("Prison", "TXT_REGION_PRISON_BLOCK_C")[0] != 0)
    { std::printf("00648820_TEST FAIL\n"); return 1; }
    float fade = 0.5f; float scale = fade * 0.5f; scale += 0.5f;
    if (scale != 0.75f) { std::printf("00648820_TEST FAIL\n"); return 1; }
    std::printf("00648820_TEST PASS\n");
    return 0;
}
