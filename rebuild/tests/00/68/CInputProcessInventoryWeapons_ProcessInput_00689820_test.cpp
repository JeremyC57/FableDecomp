// behaviour test for 0x00689820: stick and D-pad inputs map to front-end events 0..3
#include <cstdio>
static int Map(int type, float x, float y, int pressed, int released, int action)
{
    if (type == 0x11) {
        if (y > 0.6f) return 1; if (y < -0.6f) return 0; if (x < -0.9f) return 2; if (x > 0.9f) return 3; return -1;
    }
    if (pressed) {
        if (action == 0x25) return 0; if (action == 0x26) return 1;
        if (action == 0x27 || action == 0x22) return 2; if (action == 0x28 || action == 0x21) return 3; return -1;
    }
    if (released) { if (action == 0x25) return 0; if (action == 0x26) return 1; }
    return -1;
}
int main()
{
    if (Map(0x11, 0, 0.7f, 0, 0, 0) != 1 || Map(0x11, -0.95f, 0, 0, 0, 0) != 2 || Map(0x11, 0.5f, 0.5f, 0, 0, 0) != -1 ||
        Map(1, 0, 0, 1, 0, 0x22) != 2 || Map(1, 0, 0, 1, 0, 0x21) != 3 || Map(1, 0, 0, 0, 1, 0x27) != -1 || Map(1, 0, 0, 0, 1, 0x26) != 1)
        { std::printf("00689820_TEST FAIL\n"); return 1; }
    std::printf("00689820_TEST PASS\n");
    return 0;
}