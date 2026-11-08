// Behaviour test for 0x00494900 CFrontendGameComponent::Output: the debug front end lists one menu per mode, the cursor line in
// yellow, y starting at GFAYToVY(32) and stepping one font height per line (one extra blank line before the quit line of the lists).
#include <cstdio>
#include <cstring>
struct Line { char text[64]; float y; bool highlighted; };
static Line g_lines[32]; static int g_n = 0;
static void Draw(const char* text, float y, bool highlighted)
{
    std::strncpy(g_lines[g_n].text, text, 63); g_lines[g_n].text[63] = 0; g_lines[g_n].y = y; g_lines[g_n].highlighted = highlighted; ++g_n;
}
struct Frontend {
    int Mode; long CursorHighlight; long Offset; long Count; bool async;
    void Output(float line_height)
    {
        bool load_asynchronously = async;
        async = false;
        float y = 0.0f;
        char buf[64];
        switch (Mode) {
        case 1:
            Draw("Fable", 0.0f, false);
            y = 32.0f;
            Draw("1 - Single Player", y, CursorHighlight == 1); y += line_height;
            Draw("2 - Select World", y, CursorHighlight == 2); y += line_height;
            Draw("3 - Select Save", y, CursorHighlight == 3); y += line_height;
            Draw("6 - Quit", y, CursorHighlight == 6); y += line_height;
            Draw("7 - Switch to Release Front End", y, CursorHighlight == 7); y += line_height;
            break;
        case 12:
            Draw("Select Profile", 0.0f, false);
            y = 32.0f;
            for (long i = 0; i < 8 && Offset + i < Count; ++i) {
                std::sprintf(buf, "%ld - name%ld", i + 1, Offset + i);
                Draw(buf, y, CursorHighlight == i + 1); y += line_height;
            }
            y += line_height;
            Draw("9 - Quit", y, CursorHighlight == 9); y += line_height;
            if (Count > 8) { Draw("0 - See more profiles", y, CursorHighlight == 0); y += line_height; }
            if (Offset == 0) {
                y += line_height;
                Draw("A - [Debug Profile]", y, CursorHighlight == 10); y += line_height;
                Draw("B - Create New Profile", y, CursorHighlight == 11); y += line_height;
            }
            break;
        }
        async = load_asynchronously;
    }
};
int main()
{
    Frontend f; f.Mode = 1; f.CursorHighlight = 3; f.Offset = 0; f.Count = 0; f.async = true;
    f.Output(10.0f);
    if (g_n != 6 || !g_lines[3].highlighted || g_lines[2].highlighted || g_lines[1].y != 32.0f || g_lines[5].y != 72.0f || !f.async)
    { std::printf("00494900_TEST FAIL\n"); return 1; }
    g_n = 0; f.Mode = 12; f.CursorHighlight = 10; f.Count = 10; f.Offset = 0;
    f.Output(10.0f);
    // title + 8 names + quit + see more + debug + create
    if (g_n != 13 || g_lines[9].y != 32.0f + 9 * 10.0f || std::strcmp(g_lines[11].text, "A - [Debug Profile]") != 0 || !g_lines[11].highlighted
        || g_lines[11].y != g_lines[10].y + 20.0f)
    { std::printf("00494900_TEST FAIL\n"); return 1; }
    g_n = 0; f.Offset = 8; f.Output(10.0f);
    if (g_n != 5) { std::printf("00494900_TEST FAIL\n"); return 1; }   // title, 2 names, quit, see more (no debug lines past the first page)
    std::printf("00494900_TEST PASS\n");
    return 0;
}
