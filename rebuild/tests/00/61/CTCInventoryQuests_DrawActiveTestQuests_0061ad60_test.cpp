// Behaviour test for CTCInventoryQuests::DrawActiveTestQuests (0x0061AD60): a host model of the same drawing sequence over a recording
// renderer (backdrop colours, title, quest lines from the top index with their colours, flash and " [Active]" marks, "..." markers,
// the selected quest's HSP name and the three hints).
#include <cstdio>
#include <cstring>

struct Colour { unsigned char B, G, R, A; };
static Colour MakeColour(unsigned char r, unsigned char g, unsigned char b) { Colour c; c.R = r; c.G = g; c.B = b; c.A = 255; return c; }
static bool Same(const Colour& a, const Colour& b) { return a.R == b.R && a.G == b.G && a.B == b.B && a.A == b.A; }
static const Colour WHITE = MakeColour(255, 255, 255);
static const Colour TEST_QUEST_GREEN = MakeColour(25, 100, 25);
static const Colour TEST_QUEST_YELLOW = MakeColour(100, 100, 25);
static const Colour TEST_QUEST_RED = MakeColour(100, 25, 25);

struct Quest { const char* QuestName; const char* HSPName; long ColourIndex; const char* DisplayName; bool active; };
struct Line { char text[64]; float x, y; Colour colour; };

struct Screen {
    Colour backdrop[4]; Line lines[32]; long nlines; float font_height; bool flash;
    void Text(const char* t, float x, float y, const Colour& c) { Line& l = lines[nlines++]; std::strcpy(l.text, t); l.x = x; l.y = y; l.colour = c; }
    void Draw(const Quest* quests, long count, long top, long selected_index) {
        Colour colours[4];
        for (int k = 0; k < 4; ++k) colours[k] = MakeColour(0, 0, 0);
        colours[1] = MakeColour(100, 0, 100); colours[2] = MakeColour(0, 100, 100); colours[3] = MakeColour(0, 0, 128);
        std::memcpy(backdrop, colours, sizeof(colours));
        float title_x = 10.0f, title_y = 20.0f;
        Text("Test Quest Mode", title_x, title_y, MakeColour(0, 0, 0));
        float pos_x = 100.0f + 5.0f, pos_y = 200.0f - 60.0f;
        char selected_hsp[64] = "";
        long n = 0;
        if (top > 0) Text("...", pos_x, pos_y - font_height, WHITE);
        for (long i = top; i < count; ++i, ++n) {
            bool selected = false;
            if (n == 12) { Text("...", pos_x, pos_y, WHITE); break; }
            Colour colour = WHITE;
            if (n == selected_index - top) { selected = true; std::strcpy(selected_hsp, quests[i].HSPName); }
            switch (quests[i].ColourIndex) {
            case 0: colour = TEST_QUEST_RED; break;
            case 1: colour = TEST_QUEST_YELLOW; break;
            case 2: colour = TEST_QUEST_GREEN; break;
            }
            if (selected) { if (flash) colour = WHITE; flash = !flash; }
            char name[64]; std::strcpy(name, quests[i].DisplayName);
            if (name[0] == 0) std::strcpy(name, quests[i].QuestName);
            if (quests[i].active) std::strcat(name, " [Active]");
            Text(name, pos_x, pos_y, colour);
            pos_y += font_height + 4.0f;
        }
        title_y += font_height * 2.0f;
        Text("Selected HSP:", title_x, title_y, MakeColour(0, 0, 0));
        if (selected_hsp[0] == 0) std::strcpy(selected_hsp, "<unspecified>");
        Text("[X] - Activate", 380.0f, 370.0f, MakeColour(0, 0, 0));
        Text("[B] - Deactivate & Complete", 380.0f, 390.0f, MakeColour(0, 0, 0));
        Text("[Y] - Toggle Active Quests", 380.0f, 410.0f, MakeColour(0, 0, 0));
        title_y += font_height;
        Text(selected_hsp, title_x, title_y, MakeColour(0, 0, 0));
    }
};

static bool Near(float a, float b) { return a - b < 0.001f && b - a < 0.001f; }

int main()
{
    Quest quests[16];
    for (int k = 0; k < 16; ++k) { quests[k].QuestName = "Q"; quests[k].HSPName = "H"; quests[k].ColourIndex = 3; quests[k].DisplayName = ""; quests[k].active = false; }
    quests[0].DisplayName = "First"; quests[0].ColourIndex = 0; quests[0].active = true;
    quests[1].HSPName = "hsp1"; quests[1].ColourIndex = 1; quests[1].QuestName = "Second";
    quests[2].ColourIndex = 2;
    Screen s; std::memset(&s, 0, sizeof(s)); s.font_height = 10.0f;
    s.Draw(quests, 3, 0, 1);
    // title, 3 quests, "Selected HSP:", 3 hints, the HSP name
    if (s.nlines != 9 || std::strcmp(s.lines[0].text, "Test Quest Mode") != 0 || std::strcmp(s.lines[1].text, "First [Active]") != 0
        || !Same(s.lines[1].colour, TEST_QUEST_RED) || std::strcmp(s.lines[2].text, "Second") != 0 || !Same(s.lines[2].colour, TEST_QUEST_YELLOW)
        || !Same(s.lines[3].colour, TEST_QUEST_GREEN) || !Near(s.lines[2].y, 140.0f + 14.0f) || std::strcmp(s.lines[8].text, "hsp1") != 0
        || !Near(s.lines[4].y, 40.0f) || !Near(s.lines[8].y, 50.0f) || !s.flash || s.backdrop[3].B != 128 || s.backdrop[1].R != 100
        || s.backdrop[0].A != 255) {
        std::printf("0061ad60_TEST FAIL\n"); return 1;
    }
    // Second frame: the selected line flashes white.
    s.nlines = 0; s.Draw(quests, 3, 0, 1);
    if (!Same(s.lines[2].colour, WHITE) || s.flash) { std::printf("0061ad60_TEST FAIL\n"); return 1; }
    // Scrolled list of 16 from index 2: "..." above, 12 lines, "..." below; nothing selected -> "<unspecified>".
    Screen t; std::memset(&t, 0, sizeof(t)); t.font_height = 10.0f;
    t.Draw(quests, 16, 2, 0);
    if (std::strcmp(t.lines[1].text, "...") != 0 || !Near(t.lines[1].y, 130.0f) || std::strcmp(t.lines[14].text, "...") != 0
        || !Near(t.lines[14].y, 140.0f + 12 * 14.0f) || std::strcmp(t.lines[t.nlines - 1].text, "<unspecified>") != 0 || t.nlines != 20) {
        std::printf("0061ad60_TEST FAIL\n"); return 1;
    }
    std::printf("0061ad60_TEST PASS\n");
    return 0;
}