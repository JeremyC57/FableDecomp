// CTCSoundPlayer::PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d) -- play a line of attached dialogue named by its game-text symbol: CRC the name, map it to the text tag through the game's text bank, then call the tag overload
// Leaf: ?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BED0, 93 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: Ego_r.pdb / FableWin.pdb list the pair `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z`
//   and `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKK_N000@Z`; the tag overload is 0x0066BA80 (also called from
//   CCreatureAction_PerformExpression::Initialise 0x008469B3 with a tag), this one from CTCWife::GlobalUpdate 0x006D0D9A
//   and CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation 0x0092F10C; it sits in the
//   CTCSoundPlayer TU (PlayAttachedSoundEvent 0x0066B1D0, PlayAttachedSample 0x0066B300). The catalog label
//   "std::_Iterator_base12::_Getcont" is propagated and wrong; 0x0066BA80's "Sound_InitializeFromTextEntry" is descriptive.
// Callees: the CRC Calc 0x004014A0 (`__fastcall(seed 0, chars, length)`), NGameText::NGameText::CDataBank::LookupValue 0x009CC410
//   on the game-text bank (CMainGameComponent::Get() 0x013B86A0, +0x14), PlayAttachedDialogue(unsigned long, ...) 0x0066BA80.
// Shape: the bank pointer and the tag are statements before the call (`bank = ...; tag = bank->GetTagFromCRC(
//   textName.GetCRC()); return PlayAttachedDialogue(tag, a, b, c, d);`), so the bank load is hoisted first and the four
//   bools are re-pushed from their slots after the lookup; CCharString::GetCRC inline (length then chars, both
//   null-tested: "" @ 0x0129AAF4). The one-expression form pushes the bools before the CRC (94v93).
// NF47 (round 7): the local CCRC / CCharString / NGameText::CDataBank / CMainGameComponent models are replaced by the shared
//   fable_string.h (inline GetCRC), engine/CMainGameComponent.h (GetTextBank(), the PTextBank counted pointer +0x14) and
//   engine/NGameText_CDataBank.h (CDataBank : CBankFile); 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0
//   ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z (was the made-up NGameText::CDataBank::LookupValue, in neither PDB).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

class CTCSoundPlayer {
public:
    unsigned long PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d);
    unsigned long PlayAttachedDialogue(unsigned long textTag, bool a, bool b, bool c, bool d);   // 0x0066BA80
};

unsigned long CTCSoundPlayer::PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long textTag = bank.FindIndexBySymbolCRC(textName.GetCRC());
    return PlayAttachedDialogue(textTag, a, b, c, d);
}