// CMemoryDataInputStream_006e3ab0::OpenStringSource(const CCharString& name, long a, long b, long c) -- look the named string up in the world's text vector map (global 0x013B86A0 -> +0x14, CVectorMap::LookupValue @ 0x009CC410 with the name's CRC: Calc @ 0x004014A0 `__fastcall(0, chars, length)`, the empty literal 0x0129AAF4 for a null string), then play it (0x006E3890 with (the entry, the record at +0x38, a, b, c))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E3AB0, length 96 bytes.
// Leaf: OpenStringSource
// Callees: Calc @ 0x004014A0, CVectorMap::LookupValue @ 0x009CC410, 0x006E3890 (labelled CTCTalk::PlaySpeech). The chunk label is propagated; named by address. Shape: the map read before the string, `this` in esi, the map in edi, `ret 10h`.
// NF47 (round 7): 0x009CC410 is Ego_r.pdb 0x00B1A5C0 / FableWin.pdb 0x02F978C0 ?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z, called on the
//   game text bank (CMainGameComponent::GetTextBank(), PTextBank +0x14 / CNewFrontendGameComponent m_pTextBank +0x60;
//   NGameText::CDataBank : CBankFile, engine/NGameText_CDataBank.h); was the made-up CVectorMap_006e3ab0::LookupValue (in neither PDB).
// NF47 (round 7): 0x013B86A0 is ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static, read through the inline
//   CMainGameComponent::Get(), engine/CMainGameComponent.h), its text bank PTextBank +0x14 through GetTextBank(); was the stand-in SWorldText_006e3ab0* g_pWorldText_013b86a0.
//   The local CCharString / CCRC / stand-in map models are replaced by fable_string.h (inline GetCRC), engine/CMainGameComponent.h and
//   engine/NGameText_CDataBank.h. The stand-in Play's first parameter is now the looked-up index (unsigned long, as the callee's PDB
//   spelling: 0x006E3890 is ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@KABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z
//   and this row itself ?PlaySpeech@CTCTalk@@QAE?AW4ESpeechPlaybackResult@1@ABVCCharString@@W4EDialogueLayer@NSpeechGainManager@@_N2@Z,
//   both PDBs; the self rename and its caller 0x006E3B30 are left for a NAME_FIX pass).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_GETCRC_INLINE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CBANKFILE_FINDINDEXBYSYMBOLCRC
#include "fable_string.h"
#include "engine/CMainGameComponent.h"
#include "engine/NGameText_CDataBank.h"

struct CMemoryDataInputStream_006e3ab0 {
    unsigned char _pad00[0x38];
    unsigned char m_record[4];                     // +0x38
    void Play(unsigned long entry, void* record, long a, long b, long c);   // 0x006E3890
    void OpenStringSource(const CCharString& name, long a, long b, long c);
};

void CMemoryDataInputStream_006e3ab0::OpenStringSource(const CCharString& name, long a, long b, long c)
{
    const NGameText::CDataBank& bank = CMainGameComponent::Get()->GetTextBank();
    unsigned long entry = bank.FindIndexBySymbolCRC(name.GetCRC());
    Play(entry, m_record, a, b, c);
}