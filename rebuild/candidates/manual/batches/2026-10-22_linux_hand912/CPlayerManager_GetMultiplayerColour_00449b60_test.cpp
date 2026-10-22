#include <cstdio>
// Behaviour test for 00449b60: CPlayerManager::GetMultiplayerColour slot colours.
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct PlayerManager {
    CRGBColour GetMultiplayerColour(long player) const
    {
        switch (player) {
        case 0: return CRGBColour(255, 0, 0, 255);
        case 1: return CRGBColour(0, 0, 255, 255);
        case 2: return CRGBColour(255, 255, 0, 255);
        case 3: return CRGBColour(0, 255, 0, 255);
        }
        return CRGBColour(255, 255, 255, 255);
    }
};
static int Is(const CRGBColour& c, int r, int g, int b) { return c.R == r && c.G == g && c.B == b && c.A == 255; }
int main()
{
    PlayerManager pm;
    int ok = Is(pm.GetMultiplayerColour(0), 255, 0, 0) && Is(pm.GetMultiplayerColour(1), 0, 0, 255)
          && Is(pm.GetMultiplayerColour(2), 255, 255, 0) && Is(pm.GetMultiplayerColour(3), 0, 255, 0)
          && Is(pm.GetMultiplayerColour(4), 255, 255, 255) && Is(pm.GetMultiplayerColour(-1), 255, 255, 255);
    if (!ok) { std::printf("00449b60_TEST FAIL\n"); return 1; }
    std::printf("00449b60_TEST PASS\n");
    return 0;
}
