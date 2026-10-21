// behaviour test for 0x0066F4B0 CInputTypeMouseWheelMovementEvent::IsEventOfThisType
#include <stdio.h>
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT = 0xe, INPUT_EVENT_TYPE_FORCE_DWORD = 0x7fffffff };
class CInputEvent {
public:
    EInputEventType Type;
    EInputEventType GetType() const;
};
EInputEventType CInputEvent::GetType() const { return Type; }
class CInputTypeMouseWheelMovementEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseWheelMovementEvent::IsEventOfThisType(const CInputEvent& event) const
{
    return event.GetType() == INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT;
}
int main()
{
    CInputTypeMouseWheelMovementEvent o; const CInputTypeMouseWheelMovementEvent* p = &o; CInputEvent e; int ok = 1;
    e.Type = INPUT_EVENT_TYPE_MOUSE_WHEEL_MOVEMENT; ok &= p->IsEventOfThisType(e) == true;
    e.Type = (EInputEventType)(0xe + 1); ok &= p->IsEventOfThisType(e) == false;
    e.Type = (EInputEventType)0; ok &= p->IsEventOfThisType(e) == false;
    printf(ok ? "0066f4b0_TEST PASS\n" : "0066f4b0_TEST FAIL\n");
    return ok ? 0 : 1;
}