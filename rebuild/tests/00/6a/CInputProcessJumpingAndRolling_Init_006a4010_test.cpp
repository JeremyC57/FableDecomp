#include <cstdio>
#include <cstring>
static const bool g_is32 = sizeof(void*) == 4 && sizeof(long) == 4;   // the layout checks hold for the 32-bit VC7.1 build
struct C2DVector { float X, Y; C2DVector(float x, float y) : X(x), Y(y) { } };
struct C3DVector { float X, Y, Z; };
struct CInputProcessJumpingAndRolling { virtual void v00() { } virtual void v01() { }
    virtual void Init() { PulseStartTimer = 0; PulseEndTimer = 0; PulseVector = C2DVector(0.0f, 0.0f); StickPulled = false; WFLastRoll = -1; }
    unsigned char _pad04[0x24]; C2DVector PulseVector; long PulseStartTimer; long PulseEndTimer; void* Pulses; void* InitialFacingDirections; C3DVector FacingDirection; bool StickPulled; long WFLastRoll;
    CInputProcessJumpingAndRolling() : PulseVector(1.0f, 2.0f) { PulseStartTimer = 3; PulseEndTimer = 4; StickPulled = true; WFLastRoll = 5; Pulses = InitialFacingDirections = (void*)1; FacingDirection.X = 9.0f; } };
int main()
{
    CInputProcessJumpingAndRolling p; p.Init();
    if (p.PulseVector.X != 0.0f || p.PulseVector.Y != 0.0f || p.PulseStartTimer || p.PulseEndTimer || p.StickPulled || p.WFLastRoll != -1 || p.Pulses != (void*)1 || p.FacingDirection.X != 9.0f
        || (g_is32 && (char*)&p.StickPulled - (char*)&p != 0x4c) || (g_is32 && (char*)&p.WFLastRoll - (char*)&p != 0x50)) { std::printf("006a4010_TEST FAIL\n"); return 1; }
    std::printf("006a4010_TEST PASS\n");
    return 0;
}