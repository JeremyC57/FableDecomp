// CGameScriptInterface::TextEntryExists(const CCharString& symbol) const -- true when the engine text bank (0x013B86A0 -> +0x14) has an entry for the CRC of the symbol (Calc @ 0x004014A0 over the string's data and length, or "" and 0 without a data block; the lookup @ 0x009CC410)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892070, length 65 bytes.
// Callees: Calc @ 0x004014A0 (__fastcall: seed 0, the data, the length pushed), the bank lookup @ 0x009CC410 (labelled CVectorMap::LookupValue). Shape: the bank kept in esi from the start, the length and the data each through their own inline accessor (two separate null tests on the data block, "" @ 0x0129AAF4), `neg ; sbb ; neg` for the non-null result, `ret 4`.
// Leaf: TextEntryExists
// Naming wave 18: the global stand-in g_pEngineGlobals_013b86a0 (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
struct CDisplayEngine_Retail;
struct CPlayerManager_Retail;
struct CMainGameComponent_Retail;
struct CDefinitionManager_Retail;
class CWorld;
class CCharString;
class CScriptThing;
struct CDefPointer_Retail;
// NF47 (round 7): 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0 ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z, called on the
//   game text bank (CMainGameComponent::GetTextBank(), PTextBank +0x14 / CNewFrontendGameComponent m_pTextBank +0x60;
//   NGameText::CDataBank : CBankFile, engine/NGameText_CDataBank.h); was the made-up CDataBank_Retail::LookupText (in neither PDB).
//   The local CCharString / CWideString / CDataBank_Retail / CMainGameComponent / CCRC models are replaced by fable_string.h
//   (inline GetCRC), engine/CMainGameComponent.h (GetTextBank() +0x14) and engine/NGameText_CDataBank.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::TextEntryExists(const CCharString& symbol) const
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    return bank.FindIndexBySymbolCRC(symbol.GetCRC()) != 0;
}