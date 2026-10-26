// CTCOracleMinigame::GetTextWithNarrator(unsigned long textId) -- have the speech marker's talk component speak the text entry, then return the entry's text from its dialogue-voice group (or a "Missing text entry" placeholder)
// Leaf: ?GetTextWithNarrator@CTCOracleMinigame@@QAE?AVCWideString@@K@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E5FF0, 255 bytes, `ret 8`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (inlined one-ternary lookup of CTCTalk 0xf),
//   CTCTalk::PlaySpeech 0x006E3B10, CWideString(const wchar_t*) 0x0099B6B0 (static no_text_entry 0x013BAE8C, guard
//   0x013BAE90 bit 0, "Missing text entry" 0x01255FE8, atexit 0x004012BC with the destructor thunk 0x01229DE0),
//   CCharString(const CCharString&) 0x0099EC30 / ~CCharString 0x0099EAE0 (inline CTCTalk::GetDialogueVoice copy of +0x38),
//   CDataBank::GetTextEntryFromGroup 0x009C9280 (g_pMainGameComponent 0x013B86A0 +0x14), CTextEntry::GetText 0x009C7A40,
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
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct CTextEntry {
    CWideString GetText() const;                        // 0x009C7A40
};
struct CDataBank {
    CTextEntry* GetTextEntryFromGroup(unsigned long entryID, const CCharString& group, bool* found, long* index, bool flag);   // 0x009C9280
};
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    PDataBank;                            // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            *out = (T*)((it == map->last || type < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
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
    SpeechMarker->GetInterface(0xf, &talk);
    talk->PlaySpeech(textId, DIALOGUE_LAYER_0, true, true);
    static CWideString no_text_entry(L"Missing text entry");
    CDataBank* bank = g_pMainGameComponent->PDataBank;
    {
        bool found;
        CTextEntry* entry = bank->GetTextEntryFromGroup(textId, talk->GetDialogueVoice(), &found, 0, true);
        return entry ? entry->GetText() : no_text_entry;
    }
}