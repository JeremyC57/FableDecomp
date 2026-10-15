// Behaviour test for 0x00882C90: CRadialBlurEntry copy constructor copies the stack-entry base, blur params and position params.
#include <cstdio>
struct V { float x, y, z; V() {} V(const V& o) : x(o.x), y(o.y), z(o.z) {} };
struct Col { float r, g, b, a; };
struct Params { Col ThresholdColour, IntensityColour; float Length, Fade, Range, Twist; };
struct Pos { V AreaCenter, BlurCenter; int AreaTrackPos, BlurTrackPos; };
struct StackEntry { virtual ~StackEntry() {} int curdurationcount, durationcount; bool fadingout; };
struct Entry : StackEntry { Params param; Pos posparam; Entry() {} Entry(const Entry& o) : StackEntry(o), param(o.param), posparam(o.posparam) {} virtual int Kind() const { return 2; } };
int main()
{
    Entry a; a.curdurationcount = 1; a.durationcount = 2; a.fadingout = true; a.param.ThresholdColour.r = 0.5f; a.param.IntensityColour.a = 0.25f;
    a.param.Twist = 3.0f; a.posparam.AreaCenter.x = 4.0f; a.posparam.BlurCenter.z = 5.0f; a.posparam.AreaTrackPos = 6; a.posparam.BlurTrackPos = 7;
    Entry b(a);
    bool ok = b.curdurationcount == 1 && b.durationcount == 2 && b.fadingout && b.param.ThresholdColour.r == 0.5f && b.param.IntensityColour.a == 0.25f
           && b.param.Twist == 3.0f && b.posparam.AreaCenter.x == 4.0f && b.posparam.BlurCenter.z == 5.0f && b.posparam.AreaTrackPos == 6 && b.posparam.BlurTrackPos == 7 && b.Kind() == 2;
    if (!ok) { std::printf("00882c90_TEST FAIL\n"); return 1; }
    std::printf("00882c90_TEST PASS\n");
    return 0;
}
