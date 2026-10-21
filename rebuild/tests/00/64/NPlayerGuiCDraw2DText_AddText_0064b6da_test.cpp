// behaviour test for 0064b6da NPlayerGui::CDraw2DText::AddText
#include <cstdio>
struct V { float X, Y; };
struct Item { V pos; int layer; unsigned char r, g, b, a; };
struct List { Item items[8]; int n; };
static float RoundPixel(float v) { return (float)(int)(v + 0.5f); }
static void AddText(List& list, V position, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int layer, bool shadow)
{
    V p; p.X = RoundPixel(position.X); p.Y = RoundPixel(position.Y);   // absolute == virtual in this stub
    if (shadow) {
        static const float dx[3] = { 1.0f, 1.0f, -1.0f };
        static const float dy[3] = { 1.0f, -1.0f, 1.0f };
        for (int i = 0; i < 3; ++i) {
            Item& it = list.items[list.n++];
            it.pos.X = p.X + dx[i]; it.pos.Y = p.Y + dy[i]; it.layer = layer + 1;
            it.r = 0; it.g = 0; it.b = 0; it.a = a;
        }
    }
    Item& it = list.items[list.n++];
    it.pos = p; it.layer = layer; it.r = r; it.g = g; it.b = b; it.a = a;
}
int main()
{
    int bad = 0;
    List l; l.n = 0;
    V pos; pos.X = 10.2f; pos.Y = 20.7f;
    AddText(l, pos, 200, 100, 50, 128, 3, false);
    if (l.n != 1 || l.items[0].pos.X != 10.0f || l.items[0].pos.Y != 21.0f || l.items[0].layer != 3 || l.items[0].r != 200) bad |= 1;
    l.n = 0;
    AddText(l, pos, 200, 100, 50, 128, 3, true);
    if (l.n != 4) bad |= 2;
    if (l.items[0].pos.X != 11.0f || l.items[0].pos.Y != 22.0f || l.items[0].layer != 4 || l.items[0].r != 0 || l.items[0].a != 128) bad |= 4;
    if (l.items[1].pos.X != 11.0f || l.items[1].pos.Y != 20.0f) bad |= 8;
    if (l.items[2].pos.X != 9.0f || l.items[2].pos.Y != 22.0f) bad |= 16;
    if (l.items[3].layer != 3 || l.items[3].g != 100) bad |= 32;
    if (bad) { std::printf("0064b6da_TEST FAIL\n"); return 1; }
    std::printf("0064b6da_TEST PASS\n");
    return 0;
}