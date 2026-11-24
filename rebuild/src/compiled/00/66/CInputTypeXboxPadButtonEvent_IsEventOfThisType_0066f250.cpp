// CInputTypeXboxPadButtonEvent::IsEventOfThisType(const CInputEvent& event) -- does the event match this input type (joystick button types 19-21) and its Button
// Retail: Fable.exe @ 0x0066F250, 71 bytes, `ret 4`; Ego_r.pdb ?IsEventOfThisType@CInputTypeXboxPadButtonEvent@@UBE_NABVCInputEvent@@@Z (0x006769A0);
// catalog label names CInputTypeKeyboardKeyEvent for the class: wrong.
// Body: CInputEvent::GetType (0x00A03B40, out of line, called once per test) against 19, 20, 21; on a hit
// CInputEvent::GetJoystickButton (0x00A03B90) == Button (+0x04, `sub / neg / sbb / inc`), else false.
// Layout: engine/CInputTypeXboxPadButtonEvent.h (Button +0x04 as raw bytes, read in place).
// Leaf: ?IsEventOfThisType@CInputTypeXboxPadButtonEvent@@UBE_NABVCInputEvent@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CInputTypeXboxPadButtonEvent.h under FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CInputTypeXboxPadButtonEvent_Methods::IsEventOfThisType); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CINPUTTYPEXBOXPADBUTTONEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeXboxPadButtonEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_NONE };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    long GetJoystickButton() const;       // 0x00A03B90
};
bool CInputTypeXboxPadButtonEvent::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == 19 || event.GetType() == 20 || event.GetType() == 21)
        return event.GetJoystickButton() == *reinterpret_cast<const long*>(Button);
    return false;
}
