// NUISystem::CText::SetValue(const CCharString& text, bool lookUpTag, bool keepSize) -- optionally resolve the text's game-text tag (from its CRC, through the game's or the front end's text bank), then set the text as a wide string
// Leaf: ?SetValue@CText@NUISystem@@UAEXABVCCharString@@_N1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0054F370, 158 bytes (160 with padding), virtual, `ret 0Ch`.
// Vtable 0x01249CCC (NUISystem::CText) slot 145; Ego_r.pdb `?SetValue@CText@NUISystem@@UAEXABVCCharString@@_N1@Z`.
// Callees: the CRC Calc 0x004014A0 (`__fastcall(seed, data, length)`), CCharString::GetCRC's out-of-line copy 0x00404310,
//   CBankFile::FindIndexBySymbolCRC 0x009CC410 (Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0), CCharString::ToWideString
//   0x0099E480, ~CWideString 0x0099B510, and the (K, const CWideString&, bool) overload, slot 143 (+0x23c).
// Globals: CMainGameComponent::PMainGameComponent 0x013B86A0 (Get(); its text bank's object word +0x14),
//   CNewFrontendGameComponent::m_pNewFrontendGameComponent 0x013B871C (Get(); m_pTextBank's object word +0x60).
// Shape: the tag starts at 0 (esi); GetCRC is an in-class inline member called twice: VC7.1 expands it at the first
//   site (length then chars, the Calc call tail-duplicated into both null arms) and calls the out-of-line copy at the
//   second; the wide-string temporary of the final forward lands in the dead `lookUpTag` argument slot. Each bank is read through its class's inline
//   const accessor GetTextBank() (duplicated into both null arms; Ego_r.pdb ?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ and
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
void CText::SetValue(const CCharString& text, bool lookUpTag, bool keepSize)
{
    unsigned long tag = 0;
    if (lookUpTag) {
        if (CMainGameComponent::Get()) {
            tag = CMainGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.GetCRC());
        }
        else if (CNewFrontendGameComponent::Get()) {
            tag = CNewFrontendGameComponent::Get()->GetTextBank().FindIndexBySymbolCRC(text.GetCRC());
        }
    }
    SetValue(tag, text.ToWideString(), keepSize);
}
}
