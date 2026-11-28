// NUISystem::CEditBox::KeyPressed(wchar_t ch) -- the edit box's key handler (vtable slot 146): left/right (0x6f/0x70) move the cursor and nudge the scroll Adjustment by 6 past the visible window; otherwise, unless the IME is active, escape (1) runs the escape actions, delete (0x75) clears the text (delete-all mode) or cuts the character after the cursor and refreshes, home (0x6c) puts the cursor at 0 and scrolls so the text that fits the text component's window shows, end (0x71) puts the cursor at the end, enter (0x1c) runs the enter actions; a handled key ends delete-all mode; finally the display text is rebuilt
// Grade: RELOCATION_MATCH (hand4d 2026-11-19). Closed by porting FableWin.pdb's /Od statement order (0x0225E8F0): every handled
//   branch clears DeleteAllOnKeyPress itself, the IME test wraps the inner chain, the delete-cut branch falls through to the shared
//   UpdateDisplayText (retail duplicates that tail at 0x005404EF), Ego_d.pdb locals (ch; it; part1 / part2; width, ptext, i) and the
//   home scroll spelled as FableWin computes it, Adjustment = -(Text.GetLength() - i + 5) (FableWin 0x0225EE1C sub / add 5 / neg;
//   retail 0x0054058B sub edi,eax ; add edi,-5). The old draft's three eax/edx vptr swaps closed with the same port.
// VC6 for scope: FableWin opens the escape, home and enter loops with `xor reg,reg ; je` (0x0225EA83, 0x0225ED7A, 0x0225EEC5), the
//   TLC `#define for if (0) {} else for` idiom; spelled `if (false) {} else for (...)` with the loop variable declared before it
//   (landed precedent 0x007C4430, GOTCHAS "VC6 for-scope idiom").
// Flags: /O2 /Oy
// Leaf: ?KeyPressed@CEditBox@NUISystem@@UAEXG@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00540320, 755 bytes, `ret 4`.
// Callees: CWideString GetLength 0x0099B220, operator const wchar_t* 0x0099AF00, CWideString() 0x0099AED0, operator=(const wchar_t*)
//   0x0099B800 (L"" 0x0122D70C), operator=(const CWideString&) 0x0099B7D0, ~CWideString 0x0099B510, GetSubString 0x0099BDB0,
//   operator+(const CWideString&, const CWideString&) 0x0099BE70 (__fastcall); CIME::Get 0x00404A80 / IsIMEActive 0x00404C50;
//   CManager::GetInstance 0x0041E5F2 then CManager::Action (slot 14, +0x38); CText::GetCharWidthW 0x0054F4B0; own virtuals
//   GetTextComponent (slot 144, +0x240) and UpdateDisplayText (slot 147, +0x24c); CText::GetTextWindow (slot 148, +0x250).
//   Constant 2^32 0x0122DCB4 (unsigned width to float).
// Layout: shared engine/NUISystem_CEditBox.h (ActionsOnEnter +0x15c, ActionsOnEscape +0x160, Text +0x164, CursorPos +0x168,
//   LeftMostCharacter +0x16c, RightMostCharacter +0x170, Adjustment +0x174, DeleteAllOnKeyPress +0x17c); text component
//   engine/NUISystem_CText.h. Repo opt-ins (no overlay): FABLE_RETAIL_CIME_ISIMEACTIVE (fable_ime.h), FABLE_RETAIL_CTEXT_GETCHARWIDTHW,
//   FABLE_RETAIL_CTEXT_WINDOW_SLOTS (retail order of CText slots 147..150; now the default, kept harmless), FABLE_RETAIL_C2DBOXF
//   (inline GetBRX, FableWin calls ?GetBRX@C2DBoxF@@QBEMXZ at 0x0225EDF6; retail reads the box +8 at 0x0054056E).
#define FABLE_RETAIL_C2DBOXF
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
void CEditBox::KeyPressed(wchar_t ch)
{
    if (ch == 0x6f) {
        --CursorPos;
        if (CursorPos < 0)
            CursorPos = 0;
        if (CursorPos < LeftMostCharacter)
            Adjustment -= 6;
        DeleteAllOnKeyPress = false;
    } else if (ch == 0x70) {
        ++CursorPos;
        if (CursorPos > Text.GetLength())
            CursorPos = Text.GetLength();
        if (CursorPos > RightMostCharacter - 1)
            Adjustment += 6;
        DeleteAllOnKeyPress = false;
    } else if (!CIME::Get().IsIMEActive()) {
        if (ch == 1) {
            std::list<CCountedPointer<CAction> >::iterator it;
            if (false) {} else for (it = ActionsOnEscape.begin(); it != ActionsOnEscape.end(); ++it)
                CManager::GetInstance()->Action(*it);
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x75) {
            if (DeleteAllOnKeyPress) {
                Text = L"";
                CursorPos = 0;
                DeleteAllOnKeyPress = false;
            } else {
                CWideString part1;
                CWideString part2;
                if (CursorPos > 0)
                    part1 = Text.GetSubString(0, CursorPos);
                if (CursorPos < Text.GetLength())
                    part2 = Text.GetSubString(CursorPos + 1, Text.GetLength());
                Text = part1 + part2;
            }
        } else if (ch == 0x6c) {
            CursorPos = 0;
            float width = 0.0f;
            CText* ptext = static_cast<CText*>(GetTextComponent());
            long i;
            if (false) {} else for (i = 0; i != Text.GetLength(); ++i) {
                width += (float)CText::GetCharWidthW(((const wchar_t*)Text)[i]);
                if (width > ptext->GetTextWindow().GetBRX())
                    break;
            }
            Adjustment = -(Text.GetLength() - i + 5);
            if (Adjustment < 0)
                Adjustment = 0;
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x71) {
            CursorPos = Text.GetLength();
            Adjustment = 0;
            DeleteAllOnKeyPress = false;
        } else if (ch == 0x1c) {
            std::list<CCountedPointer<CAction> >::iterator it;
            if (false) {} else for (it = ActionsOnEnter.begin(); it != ActionsOnEnter.end(); ++it)
                CManager::GetInstance()->Action(*it);
            DeleteAllOnKeyPress = false;
        }
    }
    UpdateDisplayText();
}
}