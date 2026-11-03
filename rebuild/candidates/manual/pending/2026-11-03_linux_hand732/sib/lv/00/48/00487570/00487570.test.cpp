#include <cstdio>
#include <cmath>
// Model of CPlayer::DrawFreeCamDebug: a title row, then label rows whose values are laid out one title-width apart; the
// position and view values are rounded to two places (GFRoundFloat), the FOV (turns * 360) to one place then printed whole.
static float GFRoundFloat(float value, long places)
{
    float multiplier = 1.0f;
    while (places--)
        multiplier *= 10.0f;
    float rounded = (float)(long)(value * multiplier);
    return rounded / multiplier;
}
struct Item { float x, y; int kind; float value; };
static int Layout(float width, float height, unsigned long lineHeight, float titleWidth, const float pos[3], const float dir[3], float fovTurns, Item* out)
{
    int n = 0;
    float fov = GFRoundFloat(fovTurns * 360.0f, 1);
    float x = (float)(width * 0.25), y = (float)(height * 0.125);
    Item t0 = { x, y, 0, 0 }; out[n++] = t0;
    y += lineHeight * 2.0f;
    Item l1 = { x, y, 1, 0 }; out[n++] = l1;
    for (int i = 0; i < 3; ++i) { x += titleWidth; Item v = { x, y, 2, GFRoundFloat(pos[i], 2) }; out[n++] = v; }
    x = (float)(width * 0.25); y += lineHeight;
    Item l2 = { x, y, 1, 0 }; out[n++] = l2;
    for (int i = 0; i < 3; ++i) { x += titleWidth; Item v = { x, y, 2, GFRoundFloat(dir[i], 2) }; out[n++] = v; }
    x = (float)(width * 0.25); y += lineHeight;
    Item l3 = { x, y, 1, 0 }; out[n++] = l3;
    x += titleWidth; Item f = { x, y, 3, (float)(long)fov }; out[n++] = f;
    return n;
}
int main()
{
    if (std::fabs(GFRoundFloat(1.23456f, 2) - 1.23f) > 1e-5f || std::fabs(GFRoundFloat(-7.89f, 1) + 7.8f) > 1e-5f) { std::printf("00487570_TEST FAIL\n"); return 1; }
    Item items[16];
    float p[3] = { 10.126f, -3.333f, 0.5f }, d[3] = { 0.707f, 0.0f, -0.707f };
    int n = Layout(640.0f, 480.0f, 12, 50.0f, p, d, 0.25f, items);
    if (n != 11 || items[0].x != 160.0f || items[0].y != 60.0f || items[1].y != 84.0f || items[2].x != 210.0f || items[4].x != 310.0f ||
        std::fabs(items[2].value - 10.12f) > 1e-4f || items[5].x != 160.0f || items[5].y != 96.0f || items[10].value != 90.0f || items[10].y != 108.0f)
    { std::printf("00487570_TEST FAIL\n"); return 1; }
    std::printf("00487570_TEST PASS\n");
    return 0;
}