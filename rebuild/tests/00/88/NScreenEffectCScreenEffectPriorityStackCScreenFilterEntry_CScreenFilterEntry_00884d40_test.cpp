// Behaviour test for 0x00884D40 CScreenFilterEntry's implicit copy constructor: base CStackEntry members, the 32-byte
// CScreenFilterParams, then the reset map, all copied member-wise (the copy is independent of the source).
#include <cstdio>

struct FilterParams { float Alpha, Saturation, Contrast, Brightness; float R, G, B, A; };
struct ResetMap { int keys[4]; int count; };

class StackEntry {
public:
    long curdurationcount; long durationcount; bool fadingout;
    virtual ~StackEntry() { }
};
class FilterEntry : public StackEntry {
public:
    FilterParams param;
    ResetMap reset;
};

int main()
{
    FilterEntry a;
    a.curdurationcount = 3; a.durationcount = 30; a.fadingout = true;
    FilterParams p = { 0.5f, 1.0f, 1.25f, 0.75f, 0.1f, 0.2f, 0.3f, 1.0f };
    a.param = p;
    a.reset.count = 2; a.reset.keys[0] = 11; a.reset.keys[1] = 12;
    FilterEntry b(a);
    if (b.curdurationcount != 3 || b.durationcount != 30 || !b.fadingout) { std::printf("00884d40_TEST FAIL\n"); return 1; }
    if (b.param.Contrast != 1.25f || b.param.B != 0.3f || b.reset.count != 2 || b.reset.keys[1] != 12) { std::printf("00884d40_TEST FAIL\n"); return 1; }
    a.reset.keys[1] = 99; a.param.Alpha = 0.0f;
    if (b.reset.keys[1] != 12 || b.param.Alpha != 0.5f) { std::printf("00884d40_TEST FAIL\n"); return 1; }
    std::printf("00884d40_TEST PASS\n");
    return 0;
}