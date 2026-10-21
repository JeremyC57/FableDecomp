// behaviour test for 0x00445630 CInputTypeXboxPadButtonEvent::IsInputTypeWithButton
#include <stdio.h>
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_FORCE_DWORD = 0x7fffffff };
class CInputTypeXboxPadButtonEvent {
public:
    CInputTypeXboxPadButtonEvent(EXboxControllerButton b) : Button(b) {}
    virtual bool IsInputTypeWithButton(EXboxControllerButton button) const;
    EXboxControllerButton Button;
};
bool CInputTypeXboxPadButtonEvent::IsInputTypeWithButton(EXboxControllerButton button) const
{
    return Button == button;
}
int main()
{
    CInputTypeXboxPadButtonEvent e((EXboxControllerButton)5);
    const CInputTypeXboxPadButtonEvent* p = &e;
    int ok = p->IsInputTypeWithButton((EXboxControllerButton)5) && !p->IsInputTypeWithButton((EXboxControllerButton)4)
          && !p->IsInputTypeWithButton((EXboxControllerButton)0);
    printf(ok ? "00445630_TEST PASS\n" : "00445630_TEST FAIL\n");
    return ok ? 0 : 1;
}