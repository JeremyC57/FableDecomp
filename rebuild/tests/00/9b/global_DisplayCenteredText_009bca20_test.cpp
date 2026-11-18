// Behaviour test for DisplayCenteredText (0x009BCA20, global __fastcall; twin of 0x00497FD0 without the manager null checks): a standalone
// model of the row's logic -- the line count from a first parser pass (read until '\n', skip whitespace, stop at an empty
// line); then per line (num_lines counting down from that count) start = ((boxlength - length) * 0.5 + TLX, BRY - height * num_lines)
// and four draws: black at (+1,+1), (+1,-1), (-1,+1) and white at the start, each position rounded to pixels. Every condition and
// stored value has a case. (Audit e13, 2026-11-18: the string extent is a 2D value with a distinct Y so an `.X` -> `.Y` mix-up fails;
// fractional cases now check EVERY draw's rounded X and Y, killing the dropped-rounding mutants of draws 0 / 1 (X) and 2 (Y).)
#include <cstdio>
#include <cstring>
#include <cmath>

// pixel helpers with DIFFERENT X / Y behaviour so an axis mix-up fails: virtual X = actual * 2, virtual Y = actual * 3; X rounds to the
// nearest unit, Y rounds down
static float AXToVX(float x) { return x * 2.0f; }
static float AYToVY(float y) { return y * 3.0f; }
static float RoundVX(double x) { return (float)std::floor(x + 0.5); }
static float RoundVY(double y) { return (float)std::floor(y); }

struct Font;
struct Draw { float x, y; char text[32]; unsigned char r, g, b, a; unsigned long flags; const Font* font; };
static Draw g_draws[64]; static int g_ndraws;

// CWStringParser model: ReadAsStringUntilCharacter / SkipWhitespace
struct Parser {
    const char* s; size_t pos;
    explicit Parser(const char* t) : s(t), pos(0) {}
    void ReadUntil(char c, char* out) { size_t n = 0; while (s[pos] && s[pos] != c) out[n++] = s[pos++]; out[n] = 0; }
    void SkipWhitespace() { while (s[pos] == ' ' || s[pos] == '\n' || s[pos] == '\t' || s[pos] == '\r') ++pos; }
};

struct Font { float width_per_char; float max_height; };
struct Extent { float X, Y; };
static Extent StringExtents(const Font& f, const char* t) { Extent e = { AXToVX(f.width_per_char * (float)std::strlen(t)), AYToVY(f.max_height) * 7.0f }; return e; }
static float VFontMaxHeight(const Font& f) { return AYToVY(f.max_height); }

struct Box { float TLX, TLY, BRX, BRY; };

static void Draw2DText(float x, float y, const char* t, const Font& f, unsigned char r, unsigned char g, unsigned char b, unsigned char a, unsigned long flags)
{
    Draw& d = g_draws[g_ndraws++]; d.x = x; d.y = y; std::strcpy(d.text, t); d.r = r; d.g = g; d.b = b; d.a = a; d.flags = flags; d.font = &f;
}

static void DisplayCenteredText(const Font& font, const char* text, const Box& vbox)
{
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
            float length = StringExtents(font, text_line).X;
            float left = (boxlength - length) * 0.5f;
            float sx = left + vbox.TLX, sy = vbox.BRY - VFontMaxHeight(font) * num_lines;
            float dx, dy;
            dx = sx + AXToVX(1.0f); dy = sy + AYToVY(1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, font, 0, 0, 0, 255, 0);
            dx = sx + AXToVX(1.0f); dy = sy + AYToVY(-1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, font, 0, 0, 0, 255, 0);
            dx = sx + AXToVX(-1.0f); dy = sy + AYToVY(1.0f);
            Draw2DText(RoundVX(dx), RoundVY(dy), text_line, font, 0, 0, 0, 255, 0);
            Draw2DText(RoundVX(sx), RoundVY(sy), text_line, font, 255, 255, 255, 255, 0);
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
    // one line "abcd": length 24, left 88, start (188, 500 - 30*1 = 470); shadows at (+2,+3), (+2,-3), (-2,+3)
    g_ndraws = 0; DisplayCenteredText(font, "abcd", box);
    Check(g_ndraws == 4, 1);
    Check(D(0, 190.0f, 473.0f, "abcd", false) && D(1, 190.0f, 467.0f, "abcd", false) && D(2, 186.0f, 473.0f, "abcd", false) &&
          D(3, 188.0f, 470.0f, "abcd", true), 2);
    // three lines, bottom-aligned: NumLines 3, 2, 1
    g_ndraws = 0; DisplayCenteredText(font, "ab\nabcdef\nx", box);
    Check(g_ndraws == 12, 3);
    Check(D(3, 194.0f, 410.0f, "ab", true) && D(7, 182.0f, 440.0f, "abcdef", true) && D(11, 197.0f, 470.0f, "x", true), 4);
    Check(D(4, 184.0f, 443.0f, "abcdef", false) && D(9, 199.0f, 467.0f, "x", false) && D(10, 195.0f, 473.0f, "x", false), 5);
    // whitespace between lines is skipped (a blank line does not end the text)
    g_ndraws = 0; DisplayCenteredText(font, "ab\n\n  cd", box);
    Check(g_ndraws == 8 && D(3, 194.0f, 440.0f, "ab", true) && D(7, 194.0f, 470.0f, "cd", true), 6);
    // empty text: nothing
    g_ndraws = 0; DisplayCenteredText(font, "", box); Check(g_ndraws == 0, 7);
    // text starting with a newline: the first read is empty -> nothing
    g_ndraws = 0; DisplayCenteredText(font, "\nab", box); Check(g_ndraws == 0, 8);
    // the draw flags argument is 0 and the font argument is the caller's font for every draw
    g_ndraws = 0; DisplayCenteredText(font, "ab\nc", box);
    { bool ok = g_ndraws == 8; for (int i = 0; i < g_ndraws; ++i) ok = ok && g_draws[i].flags == 0 && g_draws[i].font == &font; Check(ok, 9); }
    // fractional X centre: X rounds to nearest (length 18, left (201 - 18) / 2 = 91.5, start 191.5 -> 192; shadow 189.5 -> 190)
    Box box2 = { 100.0f, 0.0f, 301.0f, 500.0f };
    g_ndraws = 0; DisplayCenteredText(font, "abc", box2);
    Check(g_ndraws == 4 && D(3, 192.0f, 470.0f, "abc", true) && D(2, 190.0f, 473.0f, "abc", false), 11);
    // ... and the two +X shadows (193.5 -> 194) are rounded too
    Check(D(0, 194.0f, 473.0f, "abc", false) && D(1, 194.0f, 467.0f, "abc", false), 13);
    // fractional Y, TLX 0: V height 7.5, start (88, 92.5): Y rounds down (92; shadows 95.5 -> 95, 89.5 -> 89)
    Box box3 = { 0.0f, 0.0f, 200.0f, 100.0f }; Font f3 = { 3.0f, 2.5f };
    g_ndraws = 0; DisplayCenteredText(f3, "abcd", box3);
    Check(g_ndraws == 4 && D(3, 88.0f, 92.0f, "abcd", true) && D(0, 90.0f, 95.0f, "abcd", false) && D(1, 90.0f, 89.0f, "abcd", false), 12);
    // ... and the (-1,+1) shadow's Y (95.5 -> 95) is rounded too
    Check(D(2, 86.0f, 95.0f, "abcd", false), 14);
    if (fails) { std::printf("009bca20_TEST FAIL\n"); return 1; }
    std::printf("009bca20_TEST PASS\n");
    return 0;
}