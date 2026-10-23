// Behaviour test for CTCTavernGame::DrawBetAndMoney (0x005EA400): bet box always, money box with the money flag only in
// state 5, third box only when both coordinates are non-zero, positions scaled by the screen size, def width for box 3.
#include <cstdio>

struct Box { int handles; float x, y; long width; int text; bool money; };
static Box g_boxes[8];
static int g_count;
static void Draw(int handles, float x, float y, long width, int text, bool money)
{
    Box& b = g_boxes[g_count++];
    b.handles = handles; b.x = x; b.y = y; b.width = width; b.text = text; b.money = money;
}
struct Game {
    long state;
    long defWidth;
    long guiWidth;
    long screenW, screenH;
    void DrawBetAndMoney(float bx, float by, float mx, float my, float tx, float ty)
    {
        Draw(0x5c, bx * screenW, by * screenH, guiWidth, 1, false);
        if (state == 5)
            Draw(0x68, mx * screenW, my * screenH, guiWidth, 2, true);
        else
            Draw(0x68, mx * screenW, my * screenH, guiWidth, 2, false);
        if (tx != 0.0f && ty != 0.0f)
            Draw(0x74, tx * screenW, ty * screenH, defWidth, 3, false);
    }
};

int main()
{
    Game g;
    g.state = 5; g.defWidth = 77; g.guiWidth = 99; g.screenW = 800; g.screenH = 600;
    g_count = 0;
    g.DrawBetAndMoney(0.5f, 0.5f, 0.25f, 0.75f, 0.0f, 0.5f);
    bool ok = g_count == 2 && g_boxes[0].x == 400.0f && g_boxes[0].y == 300.0f && !g_boxes[0].money
              && g_boxes[1].money && g_boxes[1].x == 200.0f && g_boxes[1].y == 450.0f && g_boxes[1].width == 99;
    g.state = 1;
    g_count = 0;
    g.DrawBetAndMoney(0.5f, 0.5f, 0.25f, 0.75f, 0.5f, 0.5f);
    ok = ok && g_count == 3 && !g_boxes[1].money && g_boxes[2].handles == 0x74 && g_boxes[2].width == 77
         && g_boxes[2].x == 400.0f;
    if (!ok) {
        std::printf("005ea400_TEST FAIL\n");
        return 1;
    }
    std::printf("005ea400_TEST PASS\n");
    return 0;
}