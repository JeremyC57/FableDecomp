// CTCOracleMinigame::GetTextWithNarrator(unsigned long textId) -- have the speech marker's talk component speak the text entry, then return the entry's text from its dialogue-voice group (or a "Missing text entry" placeholder)
// Leaf: ?GetTextWithNarrator@CTCOracleMinigame@@QAE?AVCWideString@@K@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5FF0, 255 bytes, `ret 8`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined one-ternary lookup of CTCTalk 0xf),
//   CTCTalk::PlaySpeech 0x006E3B10, CWideString(const wchar_t*) 0x0099B6B0 (static no_text_entry 0x013BAE8C, guard
//   0x013BAE90 bit 0, "Missing text entry" 0x01255FE8, atexit 0x004012BC with the destructor thunk 0x01229DE0),
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 (inline CTCTalk::GetDialogueVoice copy of +0x38),
//   CDataBank::GetTextEntryFromGroup 0x009C9280 (CMainGameComponent::Get() 0x013B86A0 +0x14), CTextEntry::GetText 0x009C7A40,
//   CWideString(const CWideString&) 0x0099B720 / ~CWideString 0x0099B510.
// Layout: CTCOracleMinigame SpeechMarker +0xe4 (Ego_r.pdb +0xEC, as a plain CThing* here); CThing bitset +0x20, map +0x44.
// Shape: the landed CTCTavernGame twin 0x005EB3A0 with the thing reached directly; the found flag and the entry lookup
//   and return sit in their own block so the flag takes the dead textId slot and the three temporaries share the one
//   frame slot.
// Layout (supersedes any local-model note above): engine/CTCOracleMinigame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCOracleMinigame CTCOracleMinigameDonor
#include "engine/CTCOracleMinigame.h"
#undef CTCOracleMinigame
#include <stddef.h>
class CTCBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"

namespace NGameText {
class CTextEntry { public:
    CWideString GetText() const;                        // 0x009C7A40
};
}
using NGameText::CTextEntry;
namespace NGameText {
class CDataBank { public:
    const CTextEntry* GetTextEntryFromGroup(unsigned long entryID, const CCharString& group, bool& found, long* index, bool flag) const;   // 0x009C9280
};
}
using NGameText::CDataBank;
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char _pad00[0x14];
    CDataBank*    PDataBank;                            // +0x14
};

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

enum EDialogueLayer { DIALOGUE_LAYER_0 = 0 };
class CTCTalk { public:
    unsigned char _base[0x38];
    CCharString   DialogueVoice;                        // +0x38
    long PlaySpeech(unsigned long textId, EDialogueLayer layer, bool a, bool b);   // 0x006E3B10
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
class CTCOracleMinigame : public CTCOracleMinigameDonor {
 public:
    CWideString GetTextWithNarrator(unsigned long textId);
};

CWideString CTCOracleMinigame::GetTextWithNarrator(unsigned long textId)
{
    CTCTalk* talk = 0;
    SpeechMarker->GetTC(TCI_TALK, &talk);
    talk->PlaySpeech(textId, DIALOGUE_LAYER_0, true, true);
    static CWideString no_text_entry(L"Missing text entry");
    CDataBank* bank = CMainGameComponent::Get()->PDataBank;
    {
        bool found;
        const CTextEntry* entry = bank->GetTextEntryFromGroup(textId, talk->GetDialogueVoice(), found, 0, true);
        return entry ? entry->GetText() : no_text_entry;
    }
}