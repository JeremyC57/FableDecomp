// Behaviour test for NPlayerGui::CDrawScriptScreenText::Draw (0x0064560D): nothing is drawn when script text is hidden; every free text
// entry is drawn with its own font or the default font for an empty font name; script lines advance the pen by step * line count,
// highlighted lines are grey, and drawing stops once the accumulated line count reaches the definition's maximum.
#include <cstdio>
#include <cstring>

struct Vec { float x, y; };
struct Entry { const char* font; Vec pos; };
struct Line { bool highlighted; int lineCount; };
struct Call { Vec pos; int font; bool grey; };

static Call calls[32];
static int ncalls;

static int FontId(const char* name) { return std::strcmp(name, "") == 0 ? 0 : (int)std::strlen(name); }

void Draw(bool show, bool* drawing, const Entry* entries, int nentries, const Line* lines, int nlines, Vec origin, Vec step, int maxLines)
{
    if (!show)
        return;
    if (nentries) {
        *drawing = true;
        for (int i = 0; i < nentries; ++i) {
            Call c; c.pos = entries[i].pos; c.font = FontId(entries[i].font); c.grey = false;
            calls[ncalls++] = c;
        }
    }
    if (nlines) {
        Vec pos = origin;
        int count = 0;
        for (int i = 0; i < nlines; ++i) {
            Call c; c.pos = pos; c.font = 99; c.grey = lines[i].highlighted;
            calls[ncalls++] = c;
            float n = (float)lines[i].lineCount;
            Vec delta; delta.x = step.x * n; delta.y = step.y * n;
            pos.x += delta.x;
            pos.y += delta.y;
            count += lines[i].lineCount;
            if (count >= maxLines)
                break;
        }
    }
}

int main()
{
    int ok = 1;
    bool drawing = false;
    Entry entries[2] = { { "", { 1.0f, 2.0f } }, { "Big", { 3.0f, 4.0f } } };
    Line lines[4] = { { false, 1 }, { true, 2 }, { false, 1 }, { false, 1 } };
    Vec origin = { 10.0f, 20.0f };
    Vec step = { 0.0f, 5.0f };
    ncalls = 0;
    Draw(false, &drawing, entries, 2, lines, 4, origin, step, 4);
    if (ncalls != 0 || drawing) ok = 0;
    Draw(true, &drawing, entries, 2, lines, 4, origin, step, 4);
    if (!drawing || ncalls != 5) ok = 0;
    if (calls[0].font != 0 || calls[1].font != 3 || calls[1].pos.x != 3.0f) ok = 0;
    if (calls[2].pos.y != 20.0f || calls[3].pos.y != 25.0f || !calls[3].grey || calls[4].pos.y != 35.0f || calls[4].grey) ok = 0;
    ncalls = 0; drawing = false;
    Draw(true, &drawing, entries, 0, lines, 4, origin, step, 10);
    if (drawing || ncalls != 4 || calls[3].pos.y != 40.0f) ok = 0;
    std::printf(ok ? "0064560d_TEST PASS\n" : "0064560d_TEST FAIL\n");
    return ok ? 0 : 1;
}
