// CInputTypeXboxPadButtonEvent::IsInputTypeWithButton(EXboxControllerButton) const -- true when this pad-button input type is bound to `button`
// Leaf: IsInputTypeWithButton@CInputTypeXboxPadButtonEvent
// Retail: Fable.exe @ 0x00445630, 17 bytes, vtable entry at 0x012317C0; the label is PDB-confirmed (labels_rtti_port "agree").
// Layout: engine/CInputTypeXboxPadButtonEvent.h (vptr +0x00, Button +0x04). That header types Button as an opaque
// unsigned char[4]; it is declared here as the real EXboxControllerButton enum so the comparison is the source's.
enum EXboxControllerButton { XBOX_CONTROLLER_BUTTON_FORCE_DWORD = 0x7fffffff };

class CInputTypeXboxPadButtonEvent {
public:
    virtual bool IsInputTypeWithButton(EXboxControllerButton button) const;
    EXboxControllerButton Button;                        // +0x04
};

bool CInputTypeXboxPadButtonEvent::IsInputTypeWithButton(EXboxControllerButton button) const
{
    return Button == button;
}