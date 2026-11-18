// Behaviour test for CProgressDisplay::DisplayCenteredText (0x00497FD0): a standalone model of the row's logic -- no display manager
// or no render manager: nothing drawn; the line count from a first parser pass (read until '\n', skip whitespace, stop at an empty
// line); then per line (num_lines counting down from that count) start = ((boxlength - length) * 0.5 + TLX, BRY - height * num_lines)
// and four draws: black at (+1,+1), (+1,-1), (-1,+1) and white at the start, each position rounded to pixels. Every condition and
// stored value has a case (audit e12: distinct X / Y pixel helpers and fractional X and Y on all four draws, case 13).
#include <cstdio>
#include <cstring>
#include <cmath>

// pixel helpers: virtual X = actual * 2, virtual Y = actual * 3 (distinct, so an X/Y helper swap is visible); X snaps to a whole unit,
// Y to a half unit (distinct, so a VX/VY rounding swap or a dropped rounding is visible on fractional positions)
static float AXToVX(float x) { return x * 2.0f; }
static float AYToVY(float y) { return y * 3.0f; }
static float RoundVX(double x) { return (float)std::floor(x + 0.5); }
static float RoundVY(double y) { return (float)(std::floor(y * 2.0 + 0.5) / 2.0); }

struct Draw { float x, y; char text[32]; unsigned char r, g, b, a; };
static Draw g_draws[64]; static int g_ndraws;

// CWStringParser model: ReadAsStringUntilCharacter / SkipWhitespace
struct Parser {
    const char* s; size_t pos;
    explicit Parser(const char* t) : s(t), pos(0) {}
    void ReadUntil(char c, char* out) { size_t n = 0; while (s[pos] && s[pos] != c) out[n++] = s[pos++]; out[n] = 0; }
    void SkipWhitespace() { while (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\t' || s[pos] == '\r') ++pos; }
};

struct Font { float width_per_char; float max_height; };
static float StringExtentX(const Font& f, const char* t) { return AXToVX(f.width_per_char * (float)std::strlen(t)); }
static float VFontMaxHeight(const Font& f) { return AYToVY(f.max_height); }

struct Box { float TLX, TLY, BRX, BRY; };

static void Draw2DText(float x, float y, const char* t, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    Draw& d = g_draws[g_ndraws++]; d.x = x; d.y = y; std::strcpy(d.text, t); d.r = r; d.g = g; d.b = b; d.a = a;
}

static void DisplayCenteredText(bool have_display, bool have_renderer, const Font& font, const char* text, const Box& vbox)
{
    if (!have_display)
        return;
    if (!have_renderer)
        return;
    Parser parser(text);
    float boxlength = vbox.BRX - vbox.TLX;
    long num_lines;
    long max_num_lines = 0;
    Parser counter(text);
    char text_line[64];
    counter.ReadUntil('\n', text_line);
    while (text_line[0] != 0) {
        counter.SkipWhitespace();
        counter.ReadUntil('\n', text_line);
        ++max_num_lines;
    }
    parser.ReadUntil('\n', text_line);
    if (text_line[0] != 0) {
        num_lines = max_num_lines;
        do {
            float length = StringExtentX(font, text_line);
            float left = (boxlength - length) * 0.5f;
            float sx = left + vbox.TLX, sy = vbox.BRY - VFontMaxHeight(font) * num_lines;
            float dx, dy;
            dx = sx + AXToVX(1.0f); dy = sy + AYToVY(1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, 0, 0, 0, 255);
            dx = sx + AXToVX(1.0f); dy = sy + AYToVY(-1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, 0, 0, 0, 255);
            dx = sx + AXToVX(-1.0f); dy = sy + AYToVY(1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, 0, 0, 0, 255);
            Draw2DText(RoundVX(sx), RoundVY(sy), text_line, 255, 255, 255, 255);
            parser.SkipWhitespace();
            parser.ReadUntil('\n', text_line);
            --num_lines;
        } while (text_line[0] != 0);
    }
}

static int fails = 0;
static void Check(bool ok, int id) { if (!ok) { std::printf("case %d failed\n", id); ++fails; } }
static bool D(int i, float x, float y, const char* t, bool white)
{
    const Draw& d = g_draws[i];
    unsigned char c = white ? 255 : 0;
    return d.x == x && d.y == y && std::strcmp(d.text, t) == 0 && d.r == c && d.g == c && d.b == c && d.a == 255;
}

int main()
{
    Font font = { 3.0f, 10.0f };                 // V width 6 per char, V height 30
    Box box = { 100.0f, 0.0f, 300.0f, 500.0f };  // boxlength 200
    // one line "abcd": length 24, left 88, start (188, 500 - 30*1 = 470); shadows at X +-2, Y +-3
    g_ndraws = 0; DisplayCenteredText(true, true, font, "abcd", box);
    Check(g_ndraws == 4, 1);
    Check(D(0, 190.0f, 473.0f, "abcd", false) && D(1, 190.0f, 467.0f, "abcd", false) && D(2, 186.0f, 473.0f, "abcd", false) &&
          D(3, 188.0f, 470.0f, "abcd", true), 2);
    // three lines, bottom-aligned: num_lines 3, 2, 1
    g_ndraws = 0; DisplayCenteredText(true, true, font, "ab\nabcdef\nx", box);
    Check(g_ndraws == 12, 3);
    Check(D(3, 194.0f, 410.0f, "ab", true) && D(7, 182.0f, 440.0f, "abcdef", true) && D(11, 197.0f, 470.0f, "x", true), 4);
    Check(D(4, 184.0f, 443.0f, "abcdef", false) && D(9, 199.0f, 467.0f, "x", false) && D(10, 195.0f, 473.0f, "x", false), 5);
    // whitespace between lines is skipped (blank line does not end the text)
    g_ndraws = 0; DisplayCenteredText(true, true, font, "ab\n\n  cd", box);
    Check(g_ndraws == 8 && D(3, 194.0f, 440.0f, "ab", true) && D(7, 194.0f, 470.0f, "cd", true), 6);
    // empty text: nothing
    g_ndraws = 0; DisplayCenteredText(true, true, font, "", box); Check(g_ndraws == 0, 7);
    // text starting with a newline: the first read is empty -> nothing
    g_ndraws = 0; DisplayCenteredText(true, true, font, "\nab", box); Check(g_ndraws == 0, 8);
    // no display / no renderer
    g_ndraws = 0; DisplayCenteredText(false, true, font, "abcd", box); Check(g_ndraws == 0, 9);
    g_ndraws = 0; DisplayCenteredText(true, false, font, "abcd", box); Check(g_ndraws == 0, 10);
    // fractional centre: rounding to whole units (length 6*3 = 18, left = (201 - 18) / 2 = 91.5, start 191.5 -> 192)
    Box box2 = { 100.0f, 0.0f, 301.0f, 500.0f };
    g_ndraws = 0; DisplayCenteredText(true, true, font, "abc", box2);
    Check(g_ndraws == 4 && D(3, 192.0f, 470.0f, "abc", true) && D(2, 190.0f, 473.0f, "abc", false), 11);
    // TLX offset matters (left + TLX), BRY / height matter
    Box box3 = { 0.0f, 0.0f, 200.0f, 100.0f }; Font f2 = { 3.0f, 5.0f };
    g_ndraws = 0; DisplayCenteredText(true, true, f2, "abcd", box3);
    Check(g_ndraws == 4 && D(3, 88.0f, 85.0f, "abcd", true), 12);
    // fractional X and Y on every draw: start (191.5, 500.375 - 30 = 470.375); each of the four positions rounded (X whole, Y half)
    Box box4 = { 100.0f, 0.0f, 301.0f, 500.375f };
    g_ndraws = 0; DisplayCenteredText(true, true, font, "abc", box4);
    Check(g_ndraws == 4 && D(0, 194.0f, 473.5f, "abc", false) && D(1, 194.0f, 467.5f, "abc", false) && D(2, 190.0f, 473.5f, "abc", false) &&
          D(3, 192.0f, 470.5f, "abc", true), 13);
    if (fails) { std::printf("00497fd0_TEST FAIL\n"); return 1; }
    std::printf("00497fd0_TEST PASS\n");
    return 0;
}