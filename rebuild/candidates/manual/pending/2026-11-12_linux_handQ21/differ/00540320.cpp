// NUISystem::CEditBox::KeyPressed(wchar_t key) -- the edit box's key handler (vtable slot 146): left/right (0x6f/0x70) move the cursor and nudge the scroll Adjustment by 6 past the visible window; otherwise, unless the IME is active, escape (1) runs the escape actions, delete (0x75) clears the text (delete-all mode) or cuts the character after the cursor and refreshes, home (0x6c) puts the cursor at 0 and scrolls so the text that fits the text component's window shows, end (0x71) puts the cursor at the end, enter (0x1c) runs the enter actions; a handled key ends delete-all mode; finally the display text is rebuilt
// Grade: DIFFER(755v755) -- Q21 PORTED onto engine/NUISystem_CEditBox.h + NUISystem_CText.h / NUISystem_CManager.h / fable_ime.h (overlay opt-ins CIME_ISIMEACTIVE, CTEXT_GETCHARWIDTHW, CTEXT_WINDOW_SLOTS; 7 Q21 grades). Residue (5 lines): three vptr-load scratch registers swapped eax/edx (UpdateDisplayText +0x24c in the delete branch, GetTextComponent +0x240, GetTextWindow +0x250) and `sub edi,5` where retail has `add edi,-5` (Adjustment = i - (len + 5); `i - (len+5)` stored to the member gives `lea eax,[edi-5]`, every in-place form gives sub). Tried: -(len-i+5), (i-5)-len, i -= len+5, goto-free tail, width declared after GetTextComponent.
// Leaf: ?KeyPressed@CEditBox@NUISystem@@UAEX_W@Z
// Flags: /Zc:wchar_t
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540320, 755 bytes, `ret 4`.
// Callees: CWideString GetLength 0x0099B220, operator const wchar_t* 0x0099AF00, CWideString() 0x0099AED0, operator=(const wchar_t*)
//   0x0099B800 (L"" 0x0122D70C), operator=(const CWideString&) 0x0099B7D0, ~CWideString 0x0099B510, GetSubString 0x0099BDB0,
//   operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall); CIME::Get 0x00404A80 / IsIMEActive 0x00404C50;
//   CManager::GetInstance 0x0041E5F2 then CManager::Action (slot 14, +0x38); CText::GetCharWidthW 0x0054F4B0; own virtuals
//   GetTextComponent (slot 144, +0x240) and UpdateDisplayText (slot 147, +0x24c); CText::GetTextWindow (slot 148, +0x250).
//   Constant 2^32 0x0122DCB4 (unsigned width to float).
// Layout: shared engine/NUISystem_CEditBox.h (ActionsOnEnter +0x15c, ActionsOnEscape +0x160, Text +0x164, CursorPos +0x168,
//   LeftMostCharacter +0x16c, RightMostCharacter +0x170, Adjustment +0x174, DeleteAllOnKeyPress +0x17c); text component
//   engine/NUISystem_CText.h. Overlay opt-ins: FABLE_RETAIL_CIME_ISIMEACTIVE (fable_ime.h), FABLE_RETAIL_CTEXT_GETCHARWIDTHW,
//   FABLE_RETAIL_CTEXT_WINDOW_SLOTS (retail order of CText slots 147..150).
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_SUBSTRING
#define FABLE_RETAIL_CIME
#define FABLE_RETAIL_CIME_ISIMEACTIVE
#define FABLE_RETAIL_CTEXT_GETCHARWIDTHW
#define FABLE_RETAIL_CTEXT_WINDOW_SLOTS
#include "engine/NUISystem_CEditBox.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CText.h"
#include "fable_ime.h"
namespace NUISystem {
void CEditBox::KeyPressed(wchar_t key)
{
    if (key == 0x6f) {
        if (--CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
    } else if (key == 0x70) {
        ++CursorPos;
        if (CursorPos > Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
    } else {
        if (CIME::Get().IsIMEActive())
            goto refresh;
        if (key == 1) {
            for (std::list<CCountedPointer<CAction> >::iterator i = ActionsOnEscape.begin(); i != ActionsOnEscape.end(); ++i)
                CManager::GetInstance()->Action(*i);
        } else if (key == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = L"";
                CursorPos = 0;
            } else {
                {
                    CWideString before;
                    CWideString after;
                    if (CursorPos > 0)
                        before = Text.GetSubString(0, CursorPos);
                    if (CursorPos < Text.GetLength())
                        after = Text.GetSubString(CursorPos + 1, Text.GetLength());
                    Text = before + after;
                }
                UpdateDisplayText();
                return;
            }
        } else if (key == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CText* text = static_cast<CText*>(GetTextComponent());
            long i = 0;
            if (Text.GetLength() != 0) {
                do {
                    width += (float)CText::GetCharWidthW(((const wchar_t*)Text)[i]);
                    if (width > text->GetTextWindow().BRX)
                        break;
                    ++i;
                } while (i != Text.GetLength());
            }
            long adjustment = i - (Text.GetLength() + 5);
            Adjustment = adjustment;
            if (adjustment < 0)
                Adjustment = 0;
        } else if (key == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
        } else if (key == 0x1c) {
            for (std::list<CCountedPointer<CAction> >::iterator i = ActionsOnEnter.begin(); i != ActionsOnEnter.end(); ++i)
                CManager::GetInstance()->Action(*i);
        } else {
            goto refresh;
        }
    }
    DeleteAllOnKeyPress = false;
refresh:
    UpdateDisplayText();
}
}
