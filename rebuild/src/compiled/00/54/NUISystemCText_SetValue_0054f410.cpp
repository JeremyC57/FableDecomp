// NUISystem::CText::SetValue(const CWideString& text, bool lookUpTag, bool keepSize) -- optionally resolve the text's game-text tag (from the CRC of its narrow copy, through the game's or the front end's text bank), then set the text
// Leaf: ?SetValue@CText@NUISystem@@UAEXABVCWideString@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054F410, 160 bytes, virtual, `ret 0Ch`.
// Vtable 0x01249CCC (NUISystem::CText) slot 144; Ego_r.pdb `?SetValue@CText@NUISystem@@UAEXABVCWideString@@_N1@Z`.
// Callees: CWideString::ToCharString 0x0099B2C0, the CRC Calc 0x004014A0, CCharString::GetCRC's out-of-line
//   copy 0x00404310, CBankFile::FindIndexBySymbolCRC 0x009CC410 (Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0), ~CCharString
//   0x0099EAE0, and the (K, const CWideString&, bool) overload, slot 143 (+0x23c).
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (Get(); its text bank's object word +0x14),
//   CNewFrontendGameComponent::m_pNewFrontendGameComponent 0x013B871C (Get(); m_pTextBank's object word +0x60).
// Shape: as the CCharString overload 0x0054F370, with the narrow copy as a temporary in the dead `lookUpTag` slot:
//   GetCRC is inlined at the first site and called out of line at the second, the two arms' identical
//   `FindIndexBySymbolCRC ; ~CCharString` tails are cross-jumped into one. Each bank is read through its class's inline
//   const accessor GetTextBank() (Ego_r.pdb ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ and
//   FableWin.pdb ?GetTextBank@CNewFrontendGameComponent@@QBEABVCDataBank@NGameText@@XZ), which places the load after the
//   conversion call and before Calc, as retail.
// NF232 (landing nf231 decision 3): ported onto the shared headers: the row-local CCRC, CCharString(Data), CWideString,
//   NGameText::CDataBank, CMainGameComponent and the 98-placeholder-virtual NUISystem::CText models are gone (fable_string.h
//   FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE, engine/NGameText_CDataBank.h, engine/CMainGameComponent.h TEXTBANK,
//   engine/CNewFrontendGameComponent.h, engine/NUISystem_CText.h: slots 143..145 from the declaration order).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#include "fable_string.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CMainGameComponent.h"
#include "engine/CNewFrontendGameComponent.h"
#include "engine/NUISystem_CText.h"

namespace NUISystem {
void CText::SetValue(const CWideString& text, bool lookUpTag, bool keepSize)
{
    unsigned long tag = 0;
    if (lookUpTag) {
        if (CMainGameComponent::Get()) {
            tag = CMainGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.ToCharString().GetCRC());
        }
        else if (CNewFrontendGameComponent::Get()) {
            tag = CNewFrontendGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.ToCharString().GetCRC());
        }
    }
    SetValue(tag, text, keepSize);
}
}
