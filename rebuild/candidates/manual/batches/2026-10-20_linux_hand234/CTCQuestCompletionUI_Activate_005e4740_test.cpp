// Behaviour test for 0x005E4740 CTCQuestCompletionUI::Activate (copy of the source logic over spy callees).
#include <cstdio>
#include <cstring>
struct CTCBase { long tag; };
struct CCharString {
    const char* text;
    CCharString(const char* t) : text(t) {}
    CCharString(const CCharString& o) : text(o.text) {}
    CCharString& operator=(const CCharString& o) { text = o.text; return *this; }
};
struct CWideString {
    const wchar_t* text;
    CWideString& operator=(const CWideString& o) { text = o.text; return *this; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* p = first; while (p != last && p->type < *type) ++p; return p; }
};
class CThing {
public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> bool GetInterface(long type, T** out)
    {
        if (HasTC(type)) { *out = GetTC<T>(type); return true; }
        return false;
    }
};
static int g_deactivated = 0;
struct CTCBoastUI : CTCBase { void Deactivate() { ++g_deactivated; } };
struct CTCQuestCard : CTCBase {};
static CThing* g_cardThing = 0;
static const char* g_removedFor = 0;
struct CQuestManager {
    CThing* GetActiveQuestCardFromScriptName(const CCharString&) { return g_cardThing; }
    void RemoveAcceptedBoastsForScript(CCharString name) { g_removedFor = name.text; }
};
static CQuestManager g_qm;
static CQuestManager* g_pQuestManager = &g_qm;
enum CompletionType { COMPLETION_0, COMPLETION_1, COMPLETION_2, COMPLETION_3 };
static int g_actType = -1; static CTCQuestCard* g_actCard = (CTCQuestCard*)1; static bool g_actArg = false;
struct CTCQuestCompletionUI {
    CThing*     ParentThing;
    CCharString ScriptName;
    CWideString FailureReason;
    bool        CanTeleportToGuildMember;
    CTCQuestCompletionUI() : ScriptName("") {}
    void Activate(CompletionType type, CTCQuestCard* card, bool arg) { g_actType = type; g_actCard = card; g_actArg = arg; }
    void Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport);
};
void CTCQuestCompletionUI::Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport)
{
    if (type == COMPLETION_3) {
        CTCBoastUI* boast;
        if (ParentThing->GetInterface(0xe8, &boast))
            boast->Deactivate();
    }
    CTCQuestCard* card = 0;
    ScriptName = scriptName;
    FailureReason = failureReason;
    CanTeleportToGuildMember = canTeleport;
    if (type == COMPLETION_3) {
        Activate(COMPLETION_3, card, arg);
        return;
    }
    CThing* cardThing = g_pQuestManager->GetActiveQuestCardFromScriptName(scriptName);
    if (cardThing) {
        if (cardThing->HasTC(0x6c))
            card = cardThing->GetTC<CTCQuestCard>(0x6c);
        Activate(type, card, arg);
    } else {
        Activate(type, 0, arg);
    }
    if (type == COMPLETION_0) {
        CQuestManager* questManager = g_pQuestManager;
        questManager->RemoveAcceptedBoastsForScript(ScriptName);
    }
}
static int fail() { std::printf("005e4740_TEST FAIL\n"); return 1; }
int main()
{
    CTCBoastUI boast; CTCQuestCard qc;
    CInterfaceMap::SEntry heroEntries[2] = { { 0x6c, 0 }, { 0xe8, &boast } };
    CThing hero; std::memset(hero.m_presentBits, 0, sizeof hero.m_presentBits);
    hero.m_presentBits[0xe8 >> 5] |= 1UL << (0xe8 & 31);
    hero.m_interfaces.first = heroEntries; hero.m_interfaces.last = heroEntries + 2;
    CInterfaceMap::SEntry cardEntries[1] = { { 0x6c, &qc } };
    CThing cardThing; std::memset(cardThing.m_presentBits, 0, sizeof cardThing.m_presentBits);
    cardThing.m_presentBits[0x6c >> 5] |= 1UL << (0x6c & 31);
    cardThing.m_interfaces.first = cardEntries; cardThing.m_interfaces.last = cardEntries + 1;

    CTCQuestCompletionUI ui; ui.ParentThing = &hero; ui.CanTeleportToGuildMember = false;
    CCharString name("Q_Test"); CWideString why; why.text = L"x";
    // type 3: boast UI closed, card null, no boast removal
    ui.Activate(COMPLETION_3, name, true, why, true);
    if (g_deactivated != 1 || g_actType != 3 || g_actCard != 0 || !g_actArg || !ui.CanTeleportToGuildMember
        || std::strcmp(ui.ScriptName.text, "Q_Test") != 0 || ui.FailureReason.text != why.text || g_removedFor) return fail();
    // type 0 with an active card: card component passed, boasts dropped for the script
    g_cardThing = &cardThing;
    ui.Activate(COMPLETION_0, name, false, why, false);
    if (g_deactivated != 1 || g_actType != 0 || g_actCard != &qc || g_actArg || ui.CanTeleportToGuildMember
        || !g_removedFor || std::strcmp(g_removedFor, "Q_Test") != 0) return fail();
    // type 1 without a card thing: zero card, no removal
    g_cardThing = 0; g_removedFor = 0;
    ui.Activate(COMPLETION_1, name, true, why, false);
    if (g_actType != 1 || g_actCard != 0 || g_removedFor) return fail();
    // card thing without the component: zero card
    std::memset(cardThing.m_presentBits, 0, sizeof cardThing.m_presentBits); g_cardThing = &cardThing; g_actCard = &qc;
    ui.Activate(COMPLETION_2, name, true, why, false);
    if (g_actType != 2 || g_actCard != 0) return fail();
    std::printf("005e4740_TEST PASS\n");
    return 0;
}
