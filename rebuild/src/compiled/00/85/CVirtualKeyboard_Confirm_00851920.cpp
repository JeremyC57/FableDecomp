// CVirtualKeyboard::Confirm() -- accept the entered profile name: when not already done and the name is non-empty, mark the keyboard done (not cancelled) and, if the IME is composing, finalise its string with commit
// Leaf: ?Confirm@CVirtualKeyboard@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00851920, length 121 bytes.
// Callees: CVirtualKeyboard::GetProfileName @ 0x00851890 (CWideString by value), CWideString::GetLength @ 0x0099B220 (catalog
// "CCharString::GetLength"), CWideString::~CWideString @ 0x0099B510 (conditional temporary), the IME singleton accessor
// @ 0x00404A80 (x2; propagated label "AddChildPrimitive"), CIME::IsIMEActive @ 0x00404C50, CIME::FinaliseString(bool) @ 0x00404B50.
// Layout: engine/CVirtualKeyboard.h (ProfileName +0x00, Canceled +0x04, Done +0x05), as the landed Cancel 0x00851860.
// Shape: the `&&` builds the name temporary only on its right arm (construction flag in bl, `test bl,1` before the
// destructor); the test result is a bool local in the frame.
#include "engine/CVirtualKeyboard.h"
class CWideString { public:
    void* PStringData;
    long GetLength() const;                        // 0x0099B220
    ~CWideString();                                // 0x0099B510
};
struct CIMECalls {
    bool IsIMEActive();                            // 0x00404C50
    void FinaliseString(bool commit);              // 0x00404B50
};
extern CIMECalls* GFGetIME();                      // 0x00404A80
struct CVirtualKeyboard_Methods : CVirtualKeyboard {
    CWideString GetProfileName() const;            // 0x00851890
    void Confirm();
};
void CVirtualKeyboard_Methods::Confirm()
{
    bool accept = !this->Done && GetProfileName().GetLength() > 0;
    if (accept) {
        this->Done = true;
        this->Canceled = false;
        if (GFGetIME()->IsIMEActive())
            GFGetIME()->FinaliseString(true);
    }
}