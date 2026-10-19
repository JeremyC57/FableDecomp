// Behaviour test for 0x00445800 CGamePlayerInterface::GetButtonPressureForEvent over every event type 0..0x22.
#include <cstdio>
enum EInputEventType { T_KP = 1, T_KH = 2, T_LP = 4, T_LH = 5, T_MP = 7, T_MH = 8, T_RP = 10, T_RH = 11,
    T_JP = 0x13, T_JH = 0x14, T_JR = 0x15, T_A0 = 0x16, T_A1 = 0x1A, T_B0 = 0x1B, T_B1 = 0x1F };
static int g_calls = 0;
class CInputEvent {
public:
    long type; float pressure;
    EInputEventType GetType() const { ++g_calls; return (EInputEventType)type; }
    float GetJoystickButtonPressureFraction() const { return pressure; }
};
class CGamePlayerInterface { public: float GetButtonPressureForEvent(const CInputEvent& event) const; };
float CGamePlayerInterface::GetButtonPressureForEvent(const CInputEvent& event) const
{
    if (event.GetType() == T_JP || event.GetType() == T_JH || event.GetType() == T_JR)
        return event.GetJoystickButtonPressureFraction();
    if (event.GetType() == T_KP || event.GetType() == T_KH || event.GetType() == T_LP || event.GetType() == T_LH ||
        event.GetType() == T_MP || event.GetType() == T_MH || event.GetType() == T_RP || event.GetType() == T_RH ||
        (event.GetType() >= T_A0 && event.GetType() <= T_A1) || (event.GetType() >= T_B0 && event.GetType() <= T_B1))
        return 1.0f;
    return 0.0f;
}
int main()
{
    CGamePlayerInterface gpi;
    for (long t = 0; t <= 0x22; ++t) {
        CInputEvent e; e.type = t; e.pressure = 0.25f;
        float want = 0.0f;
        if (t == 0x13 || t == 0x14 || t == 0x15) want = 0.25f;
        else if (t == 1 || t == 2 || t == 4 || t == 5 || t == 7 || t == 8 || t == 10 || t == 11 || (t >= 0x16 && t <= 0x1f)) want = 1.0f;
        float got = gpi.GetButtonPressureForEvent(e);
        if (got != want) { std::printf("00445800_TEST FAIL\n"); return 1; }
    }
    if (g_calls < 35) { std::printf("00445800_TEST FAIL\n"); return 1; }
    std::printf("00445800_TEST PASS\n");
    return 0;
}
