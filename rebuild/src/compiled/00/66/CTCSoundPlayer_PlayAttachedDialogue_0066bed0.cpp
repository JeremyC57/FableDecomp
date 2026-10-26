// CTCSoundPlayer::PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d) -- play a line of attached dialogue named by its game-text symbol: CRC the name, map it to the text tag through the game's text bank, then call the tag overload
// Leaf: ?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0066BED0, 93 bytes (+ int3 padding), thiscall `ret 14h`.
// Identity: Ego_r.pdb / FableWin.pdb list the pair `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKABVCCharString@@_N111@Z`
//   and `?PlayAttachedDialogue@CTCSoundPlayer@@QAEKK_N000@Z`; the tag overload is 0x0066BA80 (also called from
//   CCreatureAction_PerformExpression::Initialise 0x008469B3 with a tag), this one from CTCWife::GlobalUpdate 0x006D0D9A
//   and CAIStateGroup_MinionReactToPlayerInteraction::UpdateWaitingForConfirmation 0x0092F10C; it sits in the
//   CTCSoundPlayer TU (PlayAttachedSoundEvent 0x0066B1D0, PlayAttachedSample 0x0066B300). The catalog label
//   "std::_Iterator_base12::_Getcont" is propagated and wrong; 0x0066BA80's "Sound_InitializeFromTextEntry" is descriptive.
// Callees: the CRC Calc 0x004014A0 (`__fastcall(seed 0, chars, length)`), NGameText::NGameText::CDataBank::LookupValue 0x009CC410
//   on the game-text bank (g_pMainGameComponent 0x013B86A0, +0x14), PlayAttachedDialogue(unsigned long, ...) 0x0066BA80.
// Shape: the bank pointer and the tag are statements before the call (`bank = ...; tag = bank->GetTagFromCRC(
//   textName.GetCRC()); return PlayAttachedDialogue(tag, a, b, c, d);`), so the bank load is hoisted first and the four
//   bools are re-pushed from their slots after the lookup; CCharString::GetCRC inline (length then chars, both
//   null-tested: "" @ 0x0129AAF4). The one-expression form pushes the bools before the CRC (94v93).
class CCRC {
public:
    static unsigned long __fastcall Calc(unsigned long, const void*, unsigned long);   // 0x004014A0
};   // 0x004014A0

class CCharStringData { public:
    char*         PData;                            // +0x00
    unsigned long StringLength;                     // +0x04
};
class CCharString {
public:
    CCharStringData* PStringData;
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned long GetCRC() const { return CCRC::Calc(0, GetString(), GetLength()); }   // out-of-line copy 0x00404310
};

namespace NGameText {
class CDataBank {
public:
public:
    unsigned long LookupValue(unsigned long) const;   // 0x009CC410
};
}
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    NGameText::CDataBank* PGameTextDataBank;        // +0x14
    NGameText::CDataBank* GetGameTextDataBank() const { return PGameTextDataBank; }
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CTCSoundPlayer {
public:
    unsigned long PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d);
    unsigned long PlayAttachedDialogue(unsigned long textTag, bool a, bool b, bool c, bool d);   // 0x0066BA80
};

unsigned long CTCSoundPlayer::PlayAttachedDialogue(const CCharString& textName, bool a, bool b, bool c, bool d)
{
    const NGameText::CDataBank* bank = g_pMainGameComponent->GetGameTextDataBank();
    unsigned long textTag = bank->LookupValue(textName.GetCRC());
    return PlayAttachedDialogue(textTag, a, b, c, d);
}