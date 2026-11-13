// Behaviour test for 0x0064ED4F NPlayerGui::CDrawDamage::Draw(const CEnginePrimitivePrepareInfo&): a host model of the
// row's logic (the queued damage values drawn as up to four red digit sprites each: anchored bottom-right, or at the hero
// position plus a duration-scaled hero offset; the alpha from the remaining duration; digits 22 apart, the first one pulled
// left by 11 / 22 / 33 and clamped at the screen edge for three and four digits; three handles per value) with recording
// stand-ins for the engine, the anchoring helper and CDrawBase::ScaleAlpha.
#include <cstdio>
#include <cmath>

struct C2DVector { float X, Y; C2DVector() { } C2DVector(float x, float y) : X(x), Y(y) { } };

struct SDamageValue { long Duration; float Change; bool Hero; };

struct SRecord { long handle; unsigned long graphic; float x, y; unsigned char alpha; };

static SRecord g_records[64];
static int g_count = 0;
static long g_resized = -1;
static float VALUE_DISPLAY_DURATION = 30.0f;
static float g_gui_alpha = 0.5f;

struct CPlayerGuiDef {
    C2DVector DigitPos, DigitHeroPos, DigitHeroOffset;
    long DigitGraphics[10];
};

static C2DVector GFAnchorToBottomRight(const C2DVector& pos) { return C2DVector(pos.X + 160.0f, pos.Y + 120.0f); }
static C2DVector GFAnchorToTopLeft(const C2DVector& pos) { return pos; }
static unsigned char ScaleAlpha(unsigned char alpha) { return (unsigned char)(long)(alpha * g_gui_alpha); }
static void SetPersistentPrimitive(long handle, unsigned long graphic, const C2DVector& pos, unsigned char alpha)
{
    SRecord& r = g_records[g_count++];
    r.handle = handle; r.graphic = graphic; r.x = pos.X; r.y = pos.Y; r.alpha = alpha;
}

struct CDrawDamage {
    bool Drawing;
    SDamageValue Values[16];
    long Count;
    void Draw(const CPlayerGuiDef& def);
};

void CDrawDamage::Draw(const CPlayerGuiDef& def)
{
    if (Count == 0)
        return;
    Drawing = true;
    g_resized = Count * 4;
    long index = 0;
    for (long i = 0; i < Count; ++i) {
        const SDamageValue* it = &Values[i];
        C2DVector pos = GFAnchorToBottomRight(def.DigitPos);
        if (it->Hero)
            pos = GFAnchorToTopLeft(def.DigitHeroPos);
        long duration = it->Duration;
        float time = (float)duration;
        unsigned char alpha = (unsigned char)(long)(255.0f - time / VALUE_DISPLAY_DURATION * 128.0f);
        if (duration < 0) {
            if (duration * 20 < -128)
                alpha = 0;
            else
                alpha = (unsigned char)(it->Duration * 20 + 128);
        }
        C2DVector offset(0.0f, time * 2.0f);
        if (it->Hero)
            offset = C2DVector(def.DigitHeroOffset.X * time, def.DigitHeroOffset.Y * time);
        C2DVector draw_pos(pos.X + offset.X, pos.Y + offset.Y);
        if (it->Change >= 0.0f) {
            if (it->Change < 10.0f) {
                SetPersistentPrimitive(index, def.DigitGraphics[(long)it->Change], draw_pos, ScaleAlpha(alpha));
            } else if (it->Change < 100.0f) {
                draw_pos.X -= 11.0f;
                SetPersistentPrimitive(index, def.DigitGraphics[(long)it->Change % 100 / 10], draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 1, def.DigitGraphics[(long)it->Change % 100 % 10], draw_pos, ScaleAlpha(alpha));
            } else if (it->Change < 1000.0f) {
                long graphic = def.DigitGraphics[(long)it->Change % 1000 / 100];
                draw_pos.X -= 22.0f;
                if (draw_pos.X < 0.0f)
                    draw_pos.X = 0.0f;
                SetPersistentPrimitive(index, graphic, draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 1, def.DigitGraphics[(long)it->Change % 100 / 10], draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 2, def.DigitGraphics[(long)it->Change % 100 % 10], draw_pos, ScaleAlpha(alpha));
            } else if (it->Change < 10000.0f) {
                long graphic = def.DigitGraphics[(long)it->Change % 10000 / 1000];
                draw_pos.X -= 33.0f;
                if (draw_pos.X < 0.0f)
                    draw_pos.X = 0.0f;
                SetPersistentPrimitive(index, graphic, draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 1, def.DigitGraphics[(long)it->Change % 1000 / 100], draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 2, def.DigitGraphics[(long)it->Change % 100 / 10], draw_pos, ScaleAlpha(alpha));
                draw_pos.X += 22.0f;
                SetPersistentPrimitive(index + 3, def.DigitGraphics[(long)it->Change % 100 % 10], draw_pos, ScaleAlpha(alpha));
            }
        }
        index += 3;
    }
}

static int Near(float a, float b) { return std::fabs(a - b) < 0.01f; }

int main()
{
    CPlayerGuiDef def;
    def.DigitPos = C2DVector(600.0f, 400.0f);
    def.DigitHeroPos = C2DVector(20.0f, 30.0f);
    def.DigitHeroOffset = C2DVector(1.0f, -1.0f);
    for (int d = 0; d < 10; ++d)
        def.DigitGraphics[d] = 100 + d;

    CDrawDamage draw;
    draw.Drawing = false;
    draw.Count = 0;
    draw.Draw(def);
    if (draw.Drawing || g_count != 0 || g_resized != -1) { std::printf("0064ed4f_TEST FAIL\n"); return 1; }

    SDamageValue values[7] = {
        { 15, 7.0f, false }, { 30, 42.0f, true }, { 0, 345.0f, true }, { -2, 1234.0f, false },
        { -10, -5.0f, false }, { 5, 20000.0f, false }, { 3, 9.9f, false }
    };
    for (int i = 0; i < 7; ++i)
        draw.Values[i] = values[i];
    draw.Count = 7;
    draw.Draw(def);

    static const SRecord expected[] = {
        { 0, 107, 760.0f, 550.0f, 95 },
        { 3, 104, 39.0f, 0.0f, 63 }, { 4, 102, 61.0f, 0.0f, 63 },
        { 6, 103, 0.0f, 30.0f, 127 }, { 7, 104, 22.0f, 30.0f, 127 }, { 8, 105, 44.0f, 30.0f, 127 },
        { 9, 101, 727.0f, 516.0f, 44 }, { 10, 102, 749.0f, 516.0f, 44 }, { 11, 103, 771.0f, 516.0f, 44 }, { 12, 104, 793.0f, 516.0f, 44 },
        { 18, 109, 760.0f, 526.0f, 121 }
    };
    const int n = sizeof(expected) / sizeof(expected[0]);
    if (!draw.Drawing || g_resized != 28 || g_count != n) { std::printf("0064ed4f_TEST FAIL\n"); return 1; }
    for (int i = 0; i < n; ++i) {
        const SRecord& r = g_records[i];
        const SRecord& e = expected[i];
        if (r.handle != e.handle || r.graphic != e.graphic || !Near(r.x, e.x) || !Near(r.y, e.y) || r.alpha != e.alpha) {
            std::printf("0064ed4f_TEST FAIL\n");
            return 1;
        }
    }
    std::printf("0064ed4f_TEST PASS\n");
    return 0;
}
