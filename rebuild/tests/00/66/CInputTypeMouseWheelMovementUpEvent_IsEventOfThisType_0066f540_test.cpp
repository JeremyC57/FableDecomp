#include <cstdio>
#include "engine/CInputTypeMouseWheelMovementUpEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT = 14 };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    float GetMouseWheelMovement() const;                 // 0x00A03BB0
};
struct CInputTypeMouseWheelMovementUpEvent_Methods : CInputTypeMouseWheelMovementUpEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseWheelMovementUpEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT && event.GetMouseWheelMovement() > 0.0001f)
        return true;
    return false;
}

struct Ev { int type; float wheel; };
EInputEventType CInputEvent::GetType() const { return (EInputEventType)((const Ev*)this)->type; }
float CInputEvent::GetMouseWheelMovement() const { return ((const Ev*)this)->wheel; }
static bool T(int type, float w)
{
    Ev e = { type, w };
    CInputTypeMouseWheelMovementUpEvent_Methods m; m.__vftable = 0;
    return m.IsEventOfThisType(*(const CInputEvent*)&e);
}
int main()
{
    bool ok = T(14, 1.0f) && !T(14, 0.0f) && !T(14, -1.0f) && !T(14, 0.0001f) && T(14, 0.0002f) && !T(13, 1.0f) && !T(15, 1.0f);
    std::printf(ok ? "0066f540_TEST PASS\n" : "FAIL\n");
    return 0;
}