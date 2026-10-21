// CInputTypeKeyboardKeyEvent::IsEventOfThisType(const CInputEvent& event) -- does the event match this input type (keyboard key press/release/repeat (types 1-3)) and its KeyboardKey
// Leaf: ?IsEventOfThisType@CInputTypeKeyboardKeyEvent_Methods@@QBE_NABVCInputEvent@@@Z
// Retail: Fable.exe @ 0x0066EE20, 71 bytes, `ret 4`; Ego_r.pdb ?IsEventOfThisType@CInputTypeKeyboardKeyEvent@@UBE_NABVCInputEvent@@@Z (0x00676580);
// label correct.
// Body: CInputEvent::GetType (0x00A03B40, out of line, called once per test) against 1, 2, 3; on a hit
// CInputEvent::GetKey (0x00A03B70) == KeyboardKey (+0x04, `sub / neg / sbb / inc`), else false.
// Layout: engine/CInputTypeKeyboardKeyEvent.h (KeyboardKey +0x04 as raw bytes, read in place).
#include "engine/CInputTypeKeyboardKeyEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_NONE };
enum EInputKey { INPUT_KEY_NONE };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    EInputKey GetKey() const;             // 0x00A03B70
};
struct CInputTypeKeyboardKeyEvent_Methods : CInputTypeKeyboardKeyEvent {
    bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeKeyboardKeyEvent_Methods::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == 1 || event.GetType() == 2 || event.GetType() == 3)
        return event.GetKey() == *reinterpret_cast<const EInputKey*>(KeyboardKey);
    return false;
}