// Behaviour test for CSubtitleRenderer::ProcessEmbeddedGraphicTags (0x00492381): a host model of the row, statement for statement as
// src/00492381.cpp, over a fixed-width font (10 units per character, 20 per line), graphics of 30 / 25 / 0 units and a word wrap that
// breaks the formatted text after every `wrap` characters:
//  - each [TAG] with a known graphic is recorded (graphic, character index) and replaced by floor(graphic width / space width)
//    spaces; an unknown tag, an empty tag or a missing ']' stops the scan;
//  - once formatted, each graphic's index moves past the line breaks before it (repeated until stable), and its offset is
//    (width of the last line before it, height of the text before it - one line when positive), centred on its line unless the
//    text is left aligned; the primitive handle list is resized to the graphic count.
#include <cstdio>
#include <cstring>
#include <cmath>

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

struct EmbeddedGraphic { long Graphic; long CharIndex; float X, Y; };

static long GraphicFromTag(const char* tag) { if (!std::strcmp(tag, "A")) return 1; if (!std::strcmp(tag, "B")) return 2; return 0; }
static unsigned long GraphicWidth(long g) { return g == 1 ? 30u : 25u; }
static long FloorToLong(float v) { return (long)std::floor(v); }
static long CountNewlines(const char* s, long n) { long c = 0; for (long i = 0; i < n && s[i]; ++i) if (s[i] == '\n') ++c; return c; }
static void Sub(const char* s, long from, long to, char* out) { long n = 0; for (long i = from; i < to && s[i]; ++i) out[n++] = s[i]; out[n] = 0; }

struct Renderer {
    char Text[256];
    EmbeddedGraphic Graphics[8]; long GraphicCount;
    long PrimitiveCount;
    long Wrap; bool LeftAlign; float BoxWidth;

    void Format(const char* text, char* out) const     // insert '\n' after every Wrap characters
    {
        long n = 0, col = 0;
        for (long i = 0; text[i]; ++i) { out[n++] = text[i]; if (++col == Wrap && text[i + 1]) { out[n++] = '\n'; col = 0; } }
        out[n] = 0;
    }
    static void Extents(const char* s, float& x, float& y)   // width of the longest line, height of all lines
    {
        long lines = 1, col = 0, maxcol = 0;
        for (long i = 0; s[i]; ++i) { if (s[i] == '\n') { ++lines; col = 0; } else if (++col > maxcol) maxcol = col; }
        x = 10.0f * maxcol; y = 20.0f * lines;
    }
    void Process()
    {
        PrimitiveCount = 0; GraphicCount = 0;
        char text[256]; std::strcpy(text, Text);
        for (;;) {
            const char* open = std::strchr(text, '['); if (!open) break;
            long start = (long)(open - text);
            const char* close = std::strchr(text, ']'); if (!close) break;
            long end = (long)(close - text);
            char tag[64]; Sub(text, start + 1, end, tag);
            if (!tag[0]) break;
            long graphic = GraphicFromTag(tag);
            if (graphic <= 0) break;
            Graphics[GraphicCount].Graphic = graphic; Graphics[GraphicCount].CharIndex = start; ++GraphicCount;
            long spaces = FloorToLong((float)GraphicWidth(graphic) / 10.0f);
            char before[256] = "", after[256] = "";
            if (start > 0) Sub(text, 0, start, before);
            if (end < (long)std::strlen(text)) Sub(text, end + 1, (long)std::strlen(text), after);
            std::strcpy(text, before);
            for (long i = 0; i < spaces; ++i) std::strcat(text, " ");
            std::strcat(text, after);
        }
        if (GraphicCount) {
            char formatted[256]; Format(text, formatted);
            for (long i = 0; i < GraphicCount; ++i) {
                char before[256]; Sub(formatted, 0, Graphics[i].CharIndex, before);
                bool done = false; long last = 0;
                while (!done) {
                    long breaks = CountNewlines(before, (long)std::strlen(before));
                    if (breaks == last) done = true;
                    else { Graphics[i].CharIndex += breaks; Sub(formatted, 0, Graphics[i].CharIndex, before); last = breaks; }
                }
                if (!std::strncmp(before, "(HC)", 4)) { char t[256]; std::strcpy(t, before + 4); std::strcpy(before, t); }
                float ex, ey; Extents(before, ex, ey);
                const char* nl = std::strrchr(before, '\n');
                char last_line[256]; std::strcpy(last_line, nl ? nl + 1 : before);
                float lx, ly; Extents(last_line, lx, ly);
                Graphics[i].X = lx; Graphics[i].Y = ey;
                if (Graphics[i].Y > 0.0f) Graphics[i].Y -= 20.0f;
                if (!LeftAlign) {
                    char rest[256]; Sub(formatted, Graphics[i].CharIndex, (long)std::strlen(formatted), rest);
                    char word[256]; const char* rnl = std::strchr(rest, '\n');
                    if (rnl) Sub(rest, 0, (long)(rnl - rest), word); else std::strcpy(word, rest);
                    if (!word[0]) std::strcpy(word, rest);
                    char whole[512]; std::strcpy(whole, last_line); std::strcat(whole, word);
                    float offset = (BoxWidth - 10.0f * (float)std::strlen(whole)) * 0.5f;
                    Graphics[i].X += offset;
                }
            }
            PrimitiveCount = GraphicCount;
        }
        std::strcpy(Text, text);
    }
};

int main()
{
    Renderer r; std::memset(&r, 0, sizeof(r));
    std::strcpy(r.Text, "Press [A] to talk or [B] now");
    r.Wrap = 100; r.LeftAlign = true; r.BoxWidth = 400.0f;
    r.Process();
    CHECK(std::strcmp(r.Text, "Press     to talk or    now") == 0);
    CHECK(r.GraphicCount == 2 && r.PrimitiveCount == 2);
    CHECK(r.Graphics[0].Graphic == 1 && r.Graphics[0].CharIndex == 6);
    CHECK(r.Graphics[1].Graphic == 2 && r.Graphics[1].CharIndex == 21);
    CHECK(std::fabs(r.Graphics[0].X - 60.0f) < 0.01f && std::fabs(r.Graphics[0].Y - 0.0f) < 0.01f);
    CHECK(std::fabs(r.Graphics[1].X - 210.0f) < 0.01f);

    // a wrap before the second graphic moves it past the inserted line break, onto the second line; centred lines
    Renderer w; std::memset(&w, 0, sizeof(w));
    std::strcpy(w.Text, "Press [A] to talk or [B] now");
    w.Wrap = 12; w.LeftAlign = false; w.BoxWidth = 200.0f;
    w.Process();
    CHECK(w.GraphicCount == 2);
    CHECK(w.Graphics[0].CharIndex == 6);
    CHECK(w.Graphics[1].CharIndex == 22);                    // one '\n' before it (after 12 characters)
    CHECK(std::fabs(w.Graphics[1].Y - 20.0f) < 0.01f);       // second line: two lines high, less one line
    // the line holding graphic 0 is "Press     to" (12 chars), its offset is 6 chars in, centred: 60 + (200 - 120) / 2
    CHECK(std::fabs(w.Graphics[0].X - 100.0f) < 0.01f);

    // an unknown tag stops the scan and leaves the rest of the text as it is
    Renderer u; std::memset(&u, 0, sizeof(u));
    std::strcpy(u.Text, "[A] then [Q] then [B]");
    u.Wrap = 100; u.LeftAlign = true; u.BoxWidth = 100.0f;
    u.Process();
    CHECK(u.GraphicCount == 1 && std::strcmp(u.Text, "    then [Q] then [B]") == 0);

    // no tags: nothing recorded, the primitive list is left empty
    Renderer n; std::memset(&n, 0, sizeof(n));
    std::strcpy(n.Text, "plain text"); n.Wrap = 4; n.PrimitiveCount = 9;
    n.Process();
    CHECK(n.GraphicCount == 0 && n.PrimitiveCount == 0 && std::strcmp(n.Text, "plain text") == 0);

    if (g_fail) { std::printf("00492381_TEST FAIL\n"); return 1; }
    std::printf("00492381_TEST PASS\n");
    return 0;
}
