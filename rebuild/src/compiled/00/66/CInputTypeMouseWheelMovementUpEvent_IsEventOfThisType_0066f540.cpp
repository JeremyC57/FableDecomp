// CInputTypeMouseWheelMovementUpEvent::IsEventOfThisType(const CInputEvent& event) -- a mouse-wheel event (type 14) whose movement is above 0.0001
// Retail: Fable.exe @ 0x0066F540, 49 bytes, `ret 4`; referenced only from the vtable slot at 0x01231868; Ego_r.pdb
// ?IsEventOfThisType@CInputTypeMouseWheelMovementUpEvent@@UBE_NABVCInputEvent@@@Z. Callees CInputEvent::GetType
// (0x00A03B40, cmp eax,0Eh) and CInputEvent::GetMouseWheelMovement (0x00A03BB0, `fld [ecx+14h]`); the float constant
// at 0x0129BA3C is 0.0001f; `test ah,41h / jne false` is `movement > 0.0001f`.
// Leaf: ?IsEventOfThisType@CInputTypeMouseWheelMovementUpEvent@@UBE_NABVCInputEvent@@@Z
// Class-model move (NF322, 2026-11-23): defined on the real class of engine/CInputTypeMouseWheelMovementUpEvent.h under FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTUPEVENT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CInputTypeMouseWheelMovementUpEvent_Methods::IsEventOfThisType); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CINPUTTYPEMOUSEWHEELMOVEMENTUPEVENT_VIRTUAL_MODEL
#include "engine/CInputTypeMouseWheelMovementUpEvent.h"
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT = 14 };
class CInputEvent {
public:
    EInputEventType GetType() const;                     // 0x00A03B40
    float GetMouseWheelMovement() const;                 // 0x00A03BB0
};
bool CInputTypeMouseWheelMovementUpEvent::IsEventOfThisType(const CInputEvent& event) const
{
    if (event.GetType() == INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT && event.GetMouseWheelMovement() > 0.0001f)
        return true;
    return false;
}
