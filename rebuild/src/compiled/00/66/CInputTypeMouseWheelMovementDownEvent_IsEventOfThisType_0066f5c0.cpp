// CInputTypeMouseWheelMovementDownEvent::IsEventOfThisType(const CInputEvent& event) const -- a wheel event moving down past the dead zone
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Retail: Fable.exe @ 0x0066F5C0, 55 bytes, vtable 0x01231880 slot 3, __thiscall `ret 4`.
// The event's type (CInputEvent accessor 0x00A03B40, +0x28; manifest label CWorld::DrawGetEnvironment is propagated)
// must be 14 (mouse wheel), and its float value (accessor 0x00A03BB0, +0x14) must be below minus the input dead
// zone 0.0001f held in a global (0x0129BA3C): VC loads the global and negates it (`fchs`) instead of folding a
// constant, so it is an extern const float, not a literal. `<` gives `test ah,5 ; jp`.
#include "engine/CInputTypeMouseWheelMovementDownEvent.h"
enum EInputEventType { };   // Ego_r.pdb enum (values unused here)
class CInputEvent {
public:
    EInputEventType  GetType() const;                      // 0x00A03B40
    float GetValue() const;                     // 0x00A03BB0
};
extern const float DELTA_NOTIONAL_ZERO;        // 0x0129BA3C (0.0001f)
enum { INPUT_EVENT_MOUSE_WHEEL = 14 };
struct CInputTypeMouseWheelMovementDownEvent_Methods : CInputTypeMouseWheelMovementDownEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseWheelMovementDownEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == INPUT_EVENT_MOUSE_WHEEL) {
        float movement = event.GetValue();
        if (movement < -DELTA_NOTIONAL_ZERO)
            return true;
    }
    return false;
}
