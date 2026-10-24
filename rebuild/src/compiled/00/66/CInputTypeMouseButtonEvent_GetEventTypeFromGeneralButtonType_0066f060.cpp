// Grade: DIFFER(424v280) -- the catalog row holds only the 280 code bytes; the compiled symbol also carries its three inline jump tables (144 bytes). Against the full 0x1A8-byte retail range (code + tables, qx.py, default and raw-COFF extractors) this source is a RELOCATION_MATCH. Re-bound the row to 424 bytes, then land.
// CInputTypeMouseButtonEvent::GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const -- map (button, pressed/held/released) to the mouse input event type
// Leaf: ?GetEventTypeFromGeneralButtonType@CInputTypeMouseButtonEvent@@
// Retail: Fable.exe @ 0x0066F060, 280 bytes of code (`ret 4`) followed by its three 12-entry jump tables (0x0066F178,
// 0x0066F1A8, 0x0066F1D8). Called by IsEventOfThisTypeInQueue (0x0066F6D0, landed). The general type (1..3) picks one
// inner switch on Button (+0x04, EMouseButtonControl 1..12; 4..7 and anything else give 0): the left / middle / right
// buttons map to 4/7/10 (type 1), 5/8/11 (type 2), 6/9/12 (type 3); buttons 8..12 to 22..26, 27..31, 32..36.
// Case order left, middle, right lays the return blocks out as retail (VC emits them in source order).
// Layout: engine/CInputTypeMouseButtonEvent.h (Button +0x04).
#include "engine/CInputTypeMouseButtonEvent.h"
enum EInputButtonEventType { EINPUTBUTTONEVENTTYPE_FORCE_DWORD = 0x7fffffff };
enum EInputEventType { EINPUTEVENTTYPE_NONE = 0, EINPUTEVENTTYPE_FORCE_DWORD = 0x7fffffff };
struct CInputTypeMouseButtonEvent_Methods : CInputTypeMouseButtonEvent {
    EInputEventType GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const;
    long GetButton() const { return *reinterpret_cast<const long*>(Button); }
};
EInputEventType CInputTypeMouseButtonEvent_Methods::GetEventTypeFromGeneralButtonType(EInputButtonEventType type) const
{
    switch (type) {
    case 1:
        switch (GetButton()) {
        case 1: return EInputEventType(4);
        case 3: return EInputEventType(7);
        case 2: return EInputEventType(10);
        case 8: return EInputEventType(22);
        case 9: return EInputEventType(23);
        case 10: return EInputEventType(24);
        case 11: return EInputEventType(25);
        case 12: return EInputEventType(26);
        }
        break;
    case 2:
        switch (GetButton()) {
        case 1: return EInputEventType(5);
        case 3: return EInputEventType(8);
        case 2: return EInputEventType(11);
        case 8: return EInputEventType(27);
        case 9: return EInputEventType(28);
        case 10: return EInputEventType(29);
        case 11: return EInputEventType(30);
        case 12: return EInputEventType(31);
        }
        break;
    case 3:
        switch (GetButton()) {
        case 1: return EInputEventType(6);
        case 3: return EInputEventType(9);
        case 2: return EInputEventType(12);
        case 8: return EInputEventType(32);
        case 9: return EInputEventType(33);
        case 10: return EInputEventType(34);
        case 11: return EInputEventType(35);
        case 12: return EInputEventType(36);
        }
        break;
    }
    return EINPUTEVENTTYPE_NONE;
}

