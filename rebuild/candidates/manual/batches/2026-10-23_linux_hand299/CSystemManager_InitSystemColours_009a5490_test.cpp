#include <cstdio>
// Behaviour model of CSystemManager::InitSystemColours: COLORREF 0x00BBGGRR -> {B,G,R,0xff}.
struct Colour { unsigned char B, G, R, A; };
static unsigned long FakeSysColor(int index) { return 0x00102030ul + (unsigned long)index * 0x00010101ul; }
static void SetFromSystem(Colour& colour, unsigned long c)
{
    colour.R = (unsigned char)c;
    colour.G = (unsigned char)((unsigned short)c >> 8);
    colour.B = (unsigned char)(c >> 16);
    colour.A = 0xff;
}
int main()
{
    static const int order[25] = { 21, 15, 20, 22, 16, 18, 10, 2, 27, 3, 28, 1, 9, 19, 17, 13, 14, 24, 23, 4, 7, 0, 5, 6, 8 };
    Colour colours[25];
    for (int i = 0; i < 25; ++i) SetFromSystem(colours[i], FakeSysColor(order[i]));
    for (int i = 0; i < 25; ++i)
    {
        unsigned long c = FakeSysColor(order[i]);
        if (colours[i].R != (c & 0xff) || colours[i].G != ((c >> 8) & 0xff) || colours[i].B != ((c >> 16) & 0xff) || colours[i].A != 0xff)
        { std::printf("009a5490_TEST FAIL\n"); return 1; }
    }
    if (colours[0].R != 0x30 + 21 || colours[0].B != 0x10 + 21) { std::printf("009a5490_TEST FAIL\n"); return 1; }
    std::printf("009a5490_TEST PASS\n");
    return 0;
}
