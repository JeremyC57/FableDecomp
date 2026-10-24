// CInputTypeMouseWheelMovementDownEvent::IsEventOfThisType(const CInputEvent& event) const -- a wheel event moving down past the dead zone
// Retail: Fable.exe @ 0x0066F5C0, 55 bytes, vtable 0x01231880 slot 3, __thiscall `ret 4`.
// The event's type (CInputEvent accessor 0x00A03B40, +0x28; manifest label CWorld::DrawGetEnvironment is propagated)
// must be 14 (mouse wheel), and its float value (accessor 0x00A03BB0, +0x14) must be below minus the input dead
// zone 0.0001f held in a global (0x0129BA3C): VC loads the global and negates it (`fchs`) instead of folding a
// constant, so it is an extern const float, not a literal. `<` gives `test ah,5 ; jp`.
#include "engine/CInputTypeMouseWheelMovementDownEvent.h"
class CInputEvent {
public:
    long  GetType() const;                      // 0x00A03B40
    float GetValue() const;                     // 0x00A03BB0
};
extern const float g_InputDeadZone;             // 0x0129BA3C (0.0001f)
enum { INPUT_EVENT_MOUSE_WHEEL = 14 };
struct CInputTypeMouseWheelMovementDownEvent_Methods : CInputTypeMouseWheelMovementDownEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseWheelMovementDownEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == INPUT_EVENT_MOUSE_WHEEL) {
        float movement = event.GetValue();
        if (movement < -g_InputDeadZone)
            return true;
    }
    return false;
}
