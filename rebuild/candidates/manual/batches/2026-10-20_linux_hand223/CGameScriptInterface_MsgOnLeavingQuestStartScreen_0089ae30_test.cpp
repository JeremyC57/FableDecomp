#include <cstdio>
#include <cstring>

struct CCharStringData { const char* m_pData; unsigned long m_length; };
static long Compare_00411570(const char* a, const char* b) { int r = std::strcmp(a, b); return r < 0 ? -1 : (r > 0 ? 1 : 0); }
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData) return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
struct CThing {
    enum { ThingTypeMask = 0 };
    unsigned long m_presentBits[9];
    struct CInterfaceMap_Retail {
        struct SEntry { long type; void* component; };
        SEntry* first; SEntry* last;
        SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
    } m_interfaces;
    unsigned long ThingTypeFlags;
    unsigned char m_dead;
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap_Retail* map = &m_interfaces;
            CInterfaceMap_Retail::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type) it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CTarget { CThing* p; CThing* GetPTypedItem() const { return p; } };
struct CMessageEvent { long type; long frame; CTarget PTarget; };
static CMessageEvent* g_messages[4]; static int g_count; static long g_seenFrame, g_seenMax;
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(long type, long frame, long maxFrame)
    {
        g_seenFrame = frame; g_seenMax = maxFrame;
        for (int i = g_count - 1; i >= 0; --i)
            if (g_messages[i]->type == type && g_messages[i]->frame >= frame && g_messages[i]->frame <= maxFrame) return g_messages[i];
        return 0;
    }
};
struct CTCQuestStartTarget { CCharString QuestName; };
struct CGameScriptInterface {
    CMessageEventManager* Manager; long Frame;
    long GetCurrentWorldFrameForMessages() const { return Frame; }
    long GetMaxWorldFrameForMessages() const { return Frame + 2; }
    bool MsgOnLeavingQuestStartScreen(CCharString& questName) const
    {
        long frame = GetCurrentWorldFrameForMessages();
        long maxFrame = GetMaxWorldFrameForMessages();
        CMessageEventManager* messages = Manager;
        CMessageEvent* message = messages->FindMostRecentMessage(0x49, frame, maxFrame);
        if (message) {
            CThing* thing = message->PTarget.GetPTypedItem();
            CTCQuestStartTarget* target;
            if (thing && !(thing->m_dead & 1) && thing->GetInterface(&target, 0xfc)) {
                if (target->QuestName == questName) return true;
            }
        }
        return false;
    }
};
int main()
{
    CCharStringData q1 = { "Q_ARENA", 7 }, q1b = { "Q_ARENA", 7 }, q2 = { "Q_BANDIT", 8 };
    static CTCQuestStartTarget card; card.QuestName.PStringData = &q1;
    CThing thing; std::memset(&thing, 0, sizeof(thing));
    CThing::CInterfaceMap_Retail::SEntry entries[1] = { { 0xfc, &card } };
    thing.m_interfaces.first = entries; thing.m_interfaces.last = entries + 1; thing.m_presentBits[7] = 0x10000000;
    CMessageEvent m = { 0x49, 10, { &thing } }; g_messages[0] = &m; g_count = 1;
    CMessageEventManager mgr; CGameScriptInterface gsi; gsi.Manager = &mgr; gsi.Frame = 9;
    CCharString same = { &q1 }, equal = { &q1b }, other = { &q2 };
    if (!gsi.MsgOnLeavingQuestStartScreen(same) || !gsi.MsgOnLeavingQuestStartScreen(equal) || gsi.MsgOnLeavingQuestStartScreen(other) ||
        g_seenFrame != 9 || g_seenMax != 11) { std::printf("0089ae30_TEST FAIL\n"); return 1; }
    thing.m_dead = 1;
    if (gsi.MsgOnLeavingQuestStartScreen(same)) { std::printf("0089ae30_TEST FAIL\n"); return 1; }
    thing.m_dead = 0; gsi.Frame = 20;                 // the message is too old
    if (gsi.MsgOnLeavingQuestStartScreen(same)) { std::printf("0089ae30_TEST FAIL\n"); return 1; }
    std::printf("0089ae30_TEST PASS\n");
    return 0;
}
