// Behaviour test for 0x0066F5C0 CInputTypeMouseWheelMovementDownEvent::IsEventOfThisType.
#include <cstdio>
class CInputEvent {
public:
    long type; float value;
    long GetType() const { return type; }
    float GetValue() const { return value; }
};
static const float g_InputDeadZone = 0.0001f;
struct WheelDown {
    bool IsEventOfThisType(const CInputEvent& event) const
    {
        if (event.GetType() == 14) {
            float movement = event.GetValue();
            if (movement < -g_InputDeadZone)
                return true;
        }
        return false;
    }
};
int main()
{
    WheelDown t; CInputEvent e;
    e.type = 14; e.value = -1.0f;    if (!t.IsEventOfThisType(e)) { std::printf("0066f5c0_TEST FAIL\n"); return 1; }
    e.type = 14; e.value = -0.00005f; if (t.IsEventOfThisType(e)) { std::printf("0066f5c0_TEST FAIL\n"); return 1; }
    e.type = 14; e.value = 1.0f;     if (t.IsEventOfThisType(e)) { std::printf("0066f5c0_TEST FAIL\n"); return 1; }
    e.type = 13; e.value = -1.0f;    if (t.IsEventOfThisType(e)) { std::printf("0066f5c0_TEST FAIL\n"); return 1; }
    std::printf("0066f5c0_TEST PASS\n");
    return 0;
}
