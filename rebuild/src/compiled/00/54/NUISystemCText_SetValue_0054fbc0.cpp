// NUISystem::CText::SetValue(unsigned long textId, const CWideString& value, bool spaceSeparated) -- set the displayed text from
// a game-text bank entry, else from the given string. A non-zero textId is looked up in the main game's text bank (or, with no
// game running, the new front end's): a plain text entry is copied into Value; a group entry's text ids are joined into Value,
// separated by a space (spaceSeparated) or a newline. Then (in game) the game-action tags are substituted, the text is
// word-wrapped to Window (Size.Y = line count) and Size.X takes the font's measured width.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054FBC0, 848 bytes, __fastcall virtual (vtable 0x01249CCC slot 143), `ret 0xc`.
// Leaf: ?SetValue@CText@NUISystem@@UAEXKABVCWideString@@_N@Z
// Callees: CBankFile::GetEntryDataType 0x009CC210, NGameText::CDataBank::GetPTextEntry 0x009C9070 / GetPGroupEntry 0x009C8890,
//   NGameText::CTextEntry::GetText 0x009C7A40, CWideString operator= 0x0099B7D0 / operator+=(const CWideString&) 0x0099B8D0 /
//   operator+=(const wchar_t*) 0x0099B940 / GetLength 0x0099B220 / ~CWideString 0x0099B510 / GetAsCharArray 0x0099AF10,
//   std::vector<long> copy ctor 0x00514160 (CArray<long> copy), free 0x00BFEA14 (inline vector dtor),
//   CSubtitleRenderer::FormatTextWithGameActionNames 0x004917B5, CText::FitInWindow 0x0054F8E0, CFontBank slot 1 GetStringExtents.
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (text bank +0x14), CNewFrontendGameComponent::
//   m_pNewFrontendGameComponent 0x013B871C (text bank +0x60), CWideString::NullString 0x013BCA24, L" " 0x01249F28, L"\n" 0x01231728.
// Shape notes: the plain-entry arm is written first (`!GetEntryDataType`) with a named CWideString local (the counted pointer temporary
// dies before the assignment); the two plain arms are identical so VC tail-merges the front-end one into the game one. The game
// group loop keeps a reference to the group's Entries (no copy, size re-read per pass, id read as ids.begin()[i] -- ids[i] swaps
// eax/edx); the front-end loop copies the CArray<long>. The join is an if/else (`?:` selects into a register).
// Overlay opt-ins: FABLE_RETAIL_CTEXT_FITINWINDOW (engine/NUISystem_CText.h), FABLE_RETAIL_CSUBTITLERENDERER_FORMATTEXTWITHGAMEACTIONNAMES
//   (engine/CSubtitleRenderer.h).
#define FABLE_RETAIL_CTEXT_VSLOTS
#define FABLE_RETAIL_CTEXT_FITINWINDOW
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CWIDESTRING_NULLSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_NGAMETEXT_GROUPENTRY
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CSUBTITLERENDERER_FORMATTEXTWITHGAMEACTIONNAMES
#define FABLE_RETAIL_CFONTBANK
#include "fable_string.h"
#include "engine/CArray.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/CFontBank.h"
#include "engine/NUISystem_CText.h"
namespace NUISystem {
void CText::SetValue(unsigned long textId, const CWideString& value, bool spaceSeparated)
{
    if (textId > 0) {
        if (CMainGameComponent::Get()) {
            if (!CMainGameComponent::Get()->GetTextBank().GetEntryDataType(textId)) {
                CWideString text = CMainGameComponent::Get()->GetTextBank().GetPTextEntry(textId, true)->GetText();
                Value = text;
            } else {
                const CArray<long>& ids = CMainGameComponent::Get()->GetTextBank().GetPGroupEntry(textId, true)->Entries;
                Value = CWideString::NullString;
                for (unsigned long i = 0; i < ids.size(); ++i) {
                    if (Value.GetLength()) {
                        if (spaceSeparated)
                            Value += L" ";
                        else
                            Value += L"\n";
                    }
                    Value += CMainGameComponent::Get()->GetTextBank().GetPTextEntry(ids.begin()[i], true)->GetText();
                }
            }
        } else if (CNewFrontendGameComponent::Get()) {
            if (!CNewFrontendGameComponent::Get()->GetTextBank().GetEntryDataType(textId)) {
                CWideString text = CNewFrontendGameComponent::Get()->GetTextBank().GetPTextEntry(textId, true)->GetText();
                Value = text;
            } else {
                CArray<long> ids(CNewFrontendGameComponent::Get()->GetTextBank().GetPGroupEntry(textId, true)->Entries);
                Value = CWideString::NullString;
                for (unsigned long i = 0; i < ids.size(); ++i) {
                    if (Value.GetLength()) {
                        if (spaceSeparated)
                            Value += L" ";
                        else
                            Value += L"\n";
                    }
                    Value += CNewFrontendGameComponent::Get()->GetTextBank().GetPTextEntry(ids[i], true)->GetText();
                }
            }
        } else {
            Value = value;
        }
    } else {
        Value = value;
    }
    if (CMainGameComponent::Get())
        Value = CSubtitleRenderer::FormatTextWithGameActionNames(Value, 0);
    Size.Y = (float)FitInWindow(Value, Window);
    Size.X = PFont->GetStringExtents(Value.GetAsCharArray(), false).X;
}
}