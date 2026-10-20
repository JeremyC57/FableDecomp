// CTCTavernGame::GetTextWithNarrator(unsigned long textId) -- have the game master's talk component speak the text entry, then return the entry's text from the game master's dialogue-voice group (or a "Missing text entry" placeholder)
// Leaf: GetTextWithNarrator
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB3A0, 264 bytes, `ret 8` (hidden CWideString result + textId). Ego_r.pdb
//   `?GetTextWithNarrator@CTCTavernGame@@QAE?AVCWideString@@K@Z` (with the function static 'no_text_entry').
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (interface 0xF = CTCTalk),
//   CTCTalk::PlaySpeech(unsigned long, EDialogueLayer, bool, bool) @ 0x006E3B10, CWideString(const wchar_t*) @ 0x0099B6B0
//   (L"Missing text entry" @ 0x01255FE8 into the static @ 0x013B8C58, guard bit 0x013B8C5C), _atexit @ 0x004012BC (its
//   destructor thunk @ 0x01229170), CCharString copy ctor @ 0x0099EC30, CDataBank::GetTextEntryFromGroup(unsigned long,
//   const CCharString&, bool*, long*, bool) @ 0x009C9280 (g_pMainGameComponent @ 0x013B86A0 +0x14), ~CCharString
//   @ 0x0099EAE0, CTextEntry::GetText() const @ 0x009C7A40, CWideString copy ctor @ 0x0099B720, ~CWideString @ 0x0099B510.
// Layout: retail CTCTavernGame PGameMaster +0xD0 (PDB 0xEC, the 0x1C retail shift); CTCTalk DialogueVoice +0x38 (landed
//   CTCTalk::DoesSpeechExist 0x006E2910); retail CThing interface bitset +0x20 (0xF = word +0x20 bit 15), map +0x44.
// Shape: the talk component is a zero-initialised out word filled by the out-parameter GetInterface (its frame word later
//   holds the key); the voice group is passed as a by-value copy (the dead textId slot); `found` is an uninitialised bool;
//   the result is the conditional `entry ? entry->GetText() : no_text_entry` (construction flag in ebx).
#include <stddef.h>
class CTCBase;
class CWideString {
public:
    void* PStringData;
    CWideString(const wchar_t* text);                   // 0x0099B6B0
    CWideString(const CWideString& other);              // 0x0099B720
    ~CWideString();                                     // 0x0099B510
};
struct CCharString {
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
struct CMainGameComponent {
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
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
enum EDialogueLayer { DIALOGUE_LAYER_0 = 0 };
struct CTCTalk {
    unsigned char _base[0x38];
    CCharString   DialogueVoice;                        // +0x38
    long PlaySpeech(unsigned long textId, EDialogueLayer layer, bool a, bool b);   // 0x006E3B10
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
struct CTCTavernGame {
    void*         __vftable;
    unsigned char _pad04[0xd0 - 0x04];
    CBaseIntelligentPointer PGameMaster;                // +0xd0
    CWideString GetTextWithNarrator(unsigned long textId);
};

CWideString CTCTavernGame::GetTextWithNarrator(unsigned long textId)
{
    CTCTalk* talk = 0;
    PGameMaster.GetPItem()->GetInterface(0xf, &talk);
    talk->PlaySpeech(textId, DIALOGUE_LAYER_0, true, true);
    static CWideString no_text_entry(L"Missing text entry");
    CDataBank* bank = g_pMainGameComponent->PDataBank;
    bool found;
    CTextEntry* entry = bank->GetTextEntryFromGroup(textId, talk->GetDialogueVoice(), &found, 0, true);
    return entry ? entry->GetText() : no_text_entry;
}
