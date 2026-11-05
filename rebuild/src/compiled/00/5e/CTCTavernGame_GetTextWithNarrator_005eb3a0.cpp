// CTCTavernGame::GetTextWithNarrator(unsigned long textId) -- have the game master's talk component speak the text entry, then return the entry's text from the game master's dialogue-voice group (or a "Missing text entry" placeholder)
// Leaf: GetTextWithNarrator
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005EB3A0, 264 bytes, `ret 8` (hidden CWideString result + textId). Ego_r.pdb
//   `?GetTextWithNarrator@CTCTavernGame@@QAE?AVCWideString@@K@Z` (with the function static 'no_text_entry').
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (interface 0xF = CTCTalk),
//   CTCTalk::PlaySpeech(unsigned long, EDialogueLayer, bool, bool) @ 0x006E3B10, CWideString(const wchar_t*) @ 0x0099B6B0
//   (L"Missing text entry" @ 0x01255FE8 into the static @ 0x013B8C58, guard bit 0x013B8C5C), _atexit @ 0x004012BC (its
//   destructor thunk @ 0x01229170), CCharString copy ctor @ 0x0099EC30, CDataBank::GetTextEntryFromGroup(unsigned long,
//   const CCharString&, bool*, long*, bool) @ 0x009C9280 (CMainGameComponent::Get() @ 0x013B86A0 +0x14), ~CCharString
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
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
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
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound(key);
            if (it == map->end() || type < it->first)
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
enum EDialogueLayer { DIALOGUE_LAYER_0 = 0 };
class CTCTalk { public:
    unsigned char _base[0x38];
    CCharString   DialogueVoice;                        // +0x38
    long PlaySpeech(unsigned long textId, EDialogueLayer layer, bool a, bool b);   // 0x006E3B10
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
class CTCTavernGame { public:
    void*         __vftable;
    unsigned char _pad04[0xd0 - 0x04];
    CBaseIntelligentPointer PGameMaster;                // +0xd0
    CWideString GetTextWithNarrator(unsigned long textId);
};

CWideString CTCTavernGame::GetTextWithNarrator(unsigned long textId)
{
    CTCTalk* talk = 0;
    ((CThing*)PGameMaster.GetPItem())->GetInterface(0xf, &talk);
    talk->PlaySpeech(textId, DIALOGUE_LAYER_0, true, true);
    static CWideString no_text_entry(L"Missing text entry");
    CDataBank* bank = CMainGameComponent::Get()->PDataBank;
    bool found;
    const CTextEntry* entry = bank->GetTextEntryFromGroup(textId, talk->GetDialogueVoice(), found, 0, true);
    return entry ? entry->GetText() : no_text_entry;
}