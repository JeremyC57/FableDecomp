// Behaviour test for CPlayerGui::AddScreenMessage @ 0x00438AA0: the area's left edge is clamped up to the def's
// message margin, the text is formatted for that area, and the message is queued with fps x seconds frames.
#include <cstdio>
#include <list>
#include <string>
struct Box { float TLX, TLY, BRX, BRY; };
struct Def { float MessageLeftMargin; float MessageDisplaySeconds; int MessageCharactersPerFrame; int MessageFont; };
struct Msg { std::string Text; int Frames; int Speed; };
static Box g_lastArea;
static int g_lastFont;
static void FormatStringForArea(const Box& area, int font, const std::string& in, std::string& out)
{
    g_lastArea = area;
    g_lastFont = font;
    out = "[" + in + "]";
}
struct Gui {
    Def def;
    std::list<Msg> ScreenMessages;
    void AddScreenMessage(const std::string& text, Box screen, float fps)
    {
        Box area = screen;
        float margin = def.MessageLeftMargin;
        if (area.TLX < margin)
            area.TLX = margin;
        std::string formatted;
        FormatStringForArea(area, def.MessageFont, text, formatted);
        Msg m;
        m.Text = formatted;
        m.Frames = (int)(fps * def.MessageDisplaySeconds);
        m.Speed = def.MessageCharactersPerFrame;
        ScreenMessages.push_back(m);
    }
};
int main()
{
    Gui g;
    g.def.MessageLeftMargin = 40.0f;
    g.def.MessageDisplaySeconds = 2.5f;
    g.def.MessageCharactersPerFrame = 3;
    g.def.MessageFont = 7;
    Box s = { 10.0f, 0.0f, 640.0f, 480.0f };
    g.AddScreenMessage("hello", s, 30.0f);
    Box s2 = { 60.0f, 0.0f, 640.0f, 480.0f };
    g.AddScreenMessage("bye", s2, 30.0f);
    bool ok = g.ScreenMessages.size() == 2 && g.ScreenMessages.front().Text == "[hello]" &&
              g.ScreenMessages.front().Frames == 75 && g.ScreenMessages.front().Speed == 3 &&
              g_lastArea.TLX == 60.0f && g_lastFont == 7 && g.ScreenMessages.back().Text == "[bye]";
    Box s3 = { 10.0f, 0.0f, 640.0f, 480.0f };
    g.AddScreenMessage("x", s3, 30.0f);
    ok = ok && g_lastArea.TLX == 40.0f && g_lastArea.BRX == 640.0f;
    if (!ok) { std::printf("00438aa0_TEST FAIL\n"); return 1; }
    std::printf("00438aa0_TEST PASS\n");
    return 0;
}