// CVirtualKeyboard::Confirm() -- accept the entered profile name: when not already done and the name is non-empty, mark the keyboard done (not cancelled) and, if the IME is composing, finalise its string with commit
// Leaf: ?Confirm@CVirtualKeyboard_Methods@@QAEXXZ
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
// NF156 (2026-11-20, q6170): the IME accessor 0x00404A80 is NUISystem::CIME::Get() (Ego_r.pdb 0x00404100 + FableWin.pdb
//   ?Get@CIME@NUISystem@@SIAAV12@XZ, static fastcall, returns the singleton by reference; the self row and 7 landed callers bind it);
//   moved off the free ?GFGetIME@@YAPAUCIMECalls@@XZ / CIMECalls stand-ins onto the shared fable_ime.h (FABLE_RETAIL_CIME, with the
//   _ISIMEACTIVE and the NF156 _FINALISESTRING opt-ins), as the landed twin Cancel 0x00851860.
#define FABLE_RETAIL_CIME
#define FABLE_RETAIL_CIME_ISIMEACTIVE
#define FABLE_RETAIL_CIME_FINALISESTRING
#include "fable_ime.h"
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
        if (NUISystem::CIME::Get().IsIMEActive())
            NUISystem::CIME::Get().FinaliseString(true);
    }
}