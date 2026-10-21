// behaviour test for 0x0066F420 CInputTypeMouseMovementEvent::IsEventOfThisType
#include <stdio.h>
enum EInputEventType { INPUT_EVENT_TYPE_MOUSE_MOVEMENT = 0xd, INPUT_EVENT_TYPE_FORCE_DWORD = 0x7fffffff };
class CInputEvent {
public:
    EInputEventType Type;
    EInputEventType GetType() const;
};
EInputEventType CInputEvent::GetType() const { return Type; }
class CInputTypeMouseMovementEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeMouseMovementEvent::IsEventOfThisType(const CInputEvent& event) const
{
    return event.GetType() == INPUT_EVENT_TYPE_MOUSE_MOVEMENT;
}
int main()
{
    CInputTypeMouseMovementEvent o; const CInputTypeMouseMovementEvent* p = &o; CInputEvent e; int ok = 1;
    e.Type = INPUT_EVENT_TYPE_MOUSE_MOVEMENT; ok &= p->IsEventOfThisType(e) == true;
    e.Type = (EInputEventType)(0xd + 1); ok &= p->IsEventOfThisType(e) == false;
    e.Type = (EInputEventType)0; ok &= p->IsEventOfThisType(e) == false;
    printf(ok ? "0066f420_TEST PASS\n" : "0066f420_TEST FAIL\n");
    return ok ? 0 : 1;
}