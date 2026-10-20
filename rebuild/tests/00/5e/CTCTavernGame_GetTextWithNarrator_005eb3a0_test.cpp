// Behaviour test for 0x005EB3A0 CTCTavernGame::GetTextWithNarrator (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
struct CTCBase { virtual ~CTCBase() {} };
class CWideString {
public:
    const char* text;                                  // narrow stand-in for the wide text
    CWideString(const char* t) : text(t) {}
    CWideString(const CWideString& o) : text(o.text) {}
};
struct CCharString {
    const char* text;
    CCharString(const char* t) : text(t) {}
    CCharString(const CCharString& o) : text(o.text) {}
};
struct CTextEntry { const char* t; CWideString GetText() const { return CWideString(t); } };
static CTextEntry g_entry = { "Pick a card" };
static const char* g_askedGroup = 0; static unsigned long g_askedId = 0; static bool g_entryExists = true;
struct CDataBank {
    CTextEntry* GetTextEntryFromGroup(unsigned long id, const CCharString& group, bool* found, long*, bool)
    { g_askedId = id; g_askedGroup = group.text; *found = g_entryExists; return g_entryExists ? &g_entry : 0; }
};
static CDataBank g_bank;
struct CMainGameComponent { CDataBank* PDataBank; };
static CMainGameComponent g_mgc = { &g_bank };
static CMainGameComponent* g_pMainGameComponent = &g_mgc;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    template <class T> bool GetInterface(long type, T** out)
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type; CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type) it = map->last;
            *out = (T*)it->component; return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
enum EDialogueLayer { DIALOGUE_LAYER_0 = 0 };
static unsigned long g_spoken = 0;
struct CTCTalk : CTCBase {
    CCharString DialogueVoice;
    CTCTalk() : DialogueVoice("GAME_MASTER_VOICE") {}
    long PlaySpeech(unsigned long id, EDialogueLayer, bool, bool) { g_spoken = id; return 0; }
    CCharString GetDialogueVoice() const { return DialogueVoice; }
};
struct CTCTavernGame {
    CBaseIntelligentPointer PGameMaster;
    CWideString GetTextWithNarrator(unsigned long textId);
};
CWideString CTCTavernGame::GetTextWithNarrator(unsigned long textId)
{
    CTCTalk* talk = 0;
    PGameMaster.GetPItem()->GetInterface(0xf, &talk);
    talk->PlaySpeech(textId, DIALOGUE_LAYER_0, true, true);
    static CWideString no_text_entry("Missing text entry");
    CDataBank* bank = g_pMainGameComponent->PDataBank;
    bool found;
    CTextEntry* entry = bank->GetTextEntryFromGroup(textId, talk->GetDialogueVoice(), &found, 0, true);
    return entry ? entry->GetText() : no_text_entry;
}
static int fail() { std::printf("005eb3a0_TEST FAIL\n"); return 1; }
int main()
{
    CTCTalk talk;
    CInterfaceMap::SEntry e[1] = { { 0xf, &talk } };
    CThing gm; std::memset(gm.m_presentBits, 0, sizeof gm.m_presentBits); gm.m_presentBits[0] = 1UL << 0xf;
    gm.m_interfaces.first = e; gm.m_interfaces.last = e + 1;
    CTCTavernGame game; game.PGameMaster.m_pItem = &gm;
    CWideString a = game.GetTextWithNarrator(1234);
    if (std::strcmp(a.text, "Pick a card") != 0 || g_spoken != 1234 || g_askedId != 1234 || std::strcmp(g_askedGroup, "GAME_MASTER_VOICE") != 0) return fail();
    g_entryExists = false;
    CWideString b = game.GetTextWithNarrator(99);
    if (std::strcmp(b.text, "Missing text entry") != 0 || g_spoken != 99) return fail();
    std::printf("005eb3a0_TEST PASS\n");
    return 0;
}