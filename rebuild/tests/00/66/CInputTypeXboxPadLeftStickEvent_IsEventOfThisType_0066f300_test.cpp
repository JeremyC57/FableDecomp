// behaviour test for 0x0066F300 CInputTypeXboxPadLeftStickEvent::IsEventOfThisType
#include <stdio.h>
enum EInputEventType { INPUT_EVENT_TYPE_PAD_LEFT_STICK = 0x11, INPUT_EVENT_TYPE_FORCE_DWORD = 0x7fffffff };
class CInputEvent {
public:
    EInputEventType Type;
    EInputEventType GetType() const;
};
EInputEventType CInputEvent::GetType() const { return Type; }
class CInputTypeXboxPadLeftStickEvent {
public:
    virtual void _v00() {}
    virtual void _v01() {}
    virtual void _v02() {}
    virtual bool IsEventOfThisType(const CInputEvent& event) const;
};
bool CInputTypeXboxPadLeftStickEvent::IsEventOfThisType(const CInputEvent& event) const
{
    return event.GetType() == INPUT_EVENT_TYPE_PAD_LEFT_STICK;
}
int main()
{
    CInputTypeXboxPadLeftStickEvent o; const CInputTypeXboxPadLeftStickEvent* p = &o; CInputEvent e; int ok = 1;
    e.Type = INPUT_EVENT_TYPE_PAD_LEFT_STICK; ok &= p->IsEventOfThisType(e) == true;
    e.Type = (EInputEventType)(0x11 + 1); ok &= p->IsEventOfThisType(e) == false;
    e.Type = (EInputEventType)0; ok &= p->IsEventOfThisType(e) == false;
    printf(ok ? "0066f300_TEST PASS\n" : "0066f300_TEST FAIL\n");
    return ok ? 0 : 1;
}