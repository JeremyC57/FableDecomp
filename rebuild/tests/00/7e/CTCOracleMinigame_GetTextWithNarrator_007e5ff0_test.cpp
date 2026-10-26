// Behaviour test for 007e5ff0: src/007e5ff0.cpp copied verbatim, callees stubbed below so calls and arguments are observable.
// CTCOracleMinigame::GetTextWithNarrator(unsigned long textId)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
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
class CTCOracleMinigame { public:
    void*         __vftable;
    unsigned char _pad04[0xe4 - 0x04];
    CThing*       SpeechMarker;                         // +0xe4 (PDB +0xEC)
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
// ---- stubs ----
static char g_log[512];
static int g_step = 0;
static void Log(const char* s) { char b[64]; _snprintf(b, sizeof b, "%d:%s;", ++g_step, s); strcat(g_log, b); }
static int g_wides = 0, g_chars = 0, g_staticCtor = 0;
CWideString::CWideString(const wchar_t* text) { PStringData = (void*)text; ++g_staticCtor; ++g_wides; }
CWideString::CWideString(const CWideString& other) { PStringData = other.PStringData; ++g_wides; }
CWideString::~CWideString() { --g_wides; }
CCharString::CCharString(const CCharString& other) { m_pData = other.m_pData; ++g_chars; }
CCharString::~CCharString() { --g_chars; }
static const char* g_entryText = "entry text";
CWideString CTextEntry::GetText() const { Log("GetText"); CWideString w(L"x"); w.PStringData = (void*)g_entryText; return w; }
static CTextEntry g_entry;
static bool g_returnEntry = true;
static unsigned long g_lookupId = 0; static void* g_group = 0; static long* g_index = (long*)1; static bool g_flag = false;
CTextEntry* CDataBank::GetTextEntryFromGroup(unsigned long entryID, const CCharString& group, bool* found, long* index, bool flag)
{ Log("Lookup"); g_lookupId = entryID; g_group = group.m_pData; g_index = index; g_flag = flag; *found = g_returnEntry; return g_returnEntry ? &g_entry : 0; }
static CDataBank g_bank;
static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
static unsigned long g_spoken = 0; static long g_layer = -1; static bool g_a = false, g_b = false; static CTCTalk* g_speaker = 0;
long CTCTalk::PlaySpeech(unsigned long textId, EDialogueLayer layer, bool a, bool b)
{ Log("PlaySpeech"); g_spoken = textId; g_layer = layer; g_a = a; g_b = b; g_speaker = this; return 1; }
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{ SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }

int main()
{
    g_mgc.PDataBank = &g_bank;
    static unsigned char talkBuf[sizeof(CTCTalk)]; CTCTalk& talk = *(CTCTalk*)talkBuf; talk.DialogueVoice.m_pData = (void*)"VOICE";
    static CThing thing; memset(&thing, 0, sizeof thing);
    g_entries[0].type = 0x4; g_entries[0].component = 0;
    g_entries[1].type = 0xf; g_entries[1].component = (CTCBase*)&talk;
    thing.m_interfaces.first = g_entries; thing.m_interfaces.last = g_entries + 2;
    thing.m_presentBits[0] = 1UL << 15;
    CTCOracleMinigame game; memset(&game, 0xcd, sizeof game);
    game.SpeechMarker = &thing;
    // 1: entry found: the talk speaks it, the entry's text comes back
    {
        CWideString r = game.GetTextWithNarrator(1234);
        CHECK(strcmp(g_log, "1:PlaySpeech;2:Lookup;3:GetText;") == 0);
        CHECK(g_speaker == &talk && g_spoken == 1234 && g_layer == 0 && g_a && g_b);
        CHECK(g_lookupId == 1234 && strcmp((const char*)g_group, "VOICE") == 0 && g_index == 0 && g_flag);
        CHECK(r.PStringData == (void*)g_entryText);
        CHECK(g_chars == 0 && g_wides == 2 && g_staticCtor == 2);
    }
    CHECK(g_wides == 1);
    // 2: no entry: the static placeholder comes back (constructed once)
    {
        g_log[0] = 0; g_step = 0; g_returnEntry = false;
        CWideString r = game.GetTextWithNarrator(77);
        CHECK(strcmp(g_log, "1:PlaySpeech;2:Lookup;") == 0);
        CHECK(g_spoken == 77 && g_lookupId == 77);
        CHECK(r.PStringData != (void*)g_entryText && r.PStringData != 0);
        CHECK(g_staticCtor == 2 && g_chars == 0 && g_wides == 2);
    }
    if (fails == 0) printf("007e5ff0_TEST PASS\n");
    return fails;
}