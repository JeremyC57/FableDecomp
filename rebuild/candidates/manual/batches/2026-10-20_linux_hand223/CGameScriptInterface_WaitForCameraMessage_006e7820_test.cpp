#include <cstdio>
#include <cstring>

struct CCharString { const char* p; };
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{ unsigned long mask = T::ThingTypeMask; return (thing->ThingTypeFlags & mask) == mask; }
struct CIntelligentPointer {
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
    CThing* GetPTypedItem() const { return (GetPItem() != 0 && IsThingOfType<CThing>(GetPItem())) ? GetPItem() : 0; }
};
struct CMessageEvent { long Type; CIntelligentPointer PTarget; };
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
    bool operator()(CMessageEvent* event) const { return event->Type == m_type; }
};
struct SMessageNode { SMessageNode* m_pNext; SMessageNode* m_pPrev; CMessageEvent m_value; };
static int g_removed;
struct CMessageEventManager {
    SMessageNode* m_pHead;
    template<class TFilter> CMessageEvent* FindMostRecentMessage(const TFilter& filter)
    {
        for (SMessageNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext) {
            CMessageEvent* event = &node->m_value;
            if (filter(event)) return event;
        }
        return 0;
    }
    void RemoveMessage(const CMessageEvent* message)
    {
        for (SMessageNode* node = m_pHead->m_pNext; node != m_pHead; node = node->m_pNext)
            if (&node->m_value == message) { node->m_pPrev->m_pNext = node->m_pNext; node->m_pNext->m_pPrev = node->m_pPrev; ++g_removed; return; }
    }
};
struct CWorld { CMessageEventManager* PMessageEventManager; };
struct CTCCameraPointScriptedSpline {
    const char* msg;
    bool HasMessage(const CCharString& message) { return std::strcmp(msg, message.p) == 0; }
};
static int g_frames, g_stopAfter = 1000;
static CMessageEventManager* g_mgr; static SMessageNode* g_pending;
struct CScriptRunner {
    bool m_bStopRequested;
    void YieldFrame()
    {
        ++g_frames;
        if (g_frames == 3 && g_pending) {          // the camera message arrives on the third frame
            SMessageNode* h = g_mgr->m_pHead; g_pending->m_pNext = h->m_pNext; g_pending->m_pPrev = h;
            h->m_pNext->m_pPrev = g_pending; h->m_pNext = g_pending;
        }
        if (g_frames >= g_stopAfter) m_bStopRequested = true;
    }
};
static CScriptRunner g_runner;
static CScriptRunner* g_pScriptRunner = &g_runner;
struct CGameScriptInterface {
    CWorld* World;
    void WaitForCameraMessage(const CCharString& message) const
    {
        do {
            CMessageEvent* event = World->PMessageEventManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2b));
            if (event) {
                CThing* thing = event->PTarget.GetPTypedItem();
                CTCCameraPointScriptedSpline* spline;
                if (thing && !(thing->m_dead & 1) && thing->GetInterface(&spline, 0x3f)) {
                    if (spline->HasMessage(message) == true) {
                        World->PMessageEventManager->RemoveMessage(event);
                        return;
                    }
                }
            }
            g_pScriptRunner->YieldFrame();
        } while (!g_pScriptRunner->m_bStopRequested);
    }
};
int main()
{
    static CTCCameraPointScriptedSpline spline = { "CAM_DONE" };
    CThing cam; std::memset(&cam, 0, sizeof(cam));
    CThing::CInterfaceMap_Retail::SEntry entries[1] = { { 0x3f, &spline } };
    cam.m_interfaces.first = entries; cam.m_interfaces.last = entries + 1; cam.m_presentBits[1] = 0x80000000;
    SMessageNode head; head.m_pNext = head.m_pPrev = &head;
    SMessageNode other; other.m_value.Type = 7; other.m_value.PTarget.m_pItem = &cam;      // wrong type: ignored
    other.m_pNext = head.m_pNext; other.m_pPrev = &head; head.m_pNext->m_pPrev = &other; head.m_pNext = &other;
    SMessageNode msg; msg.m_value.Type = 0x2b; msg.m_value.PTarget.m_pItem = &cam;
    CMessageEventManager mgr; mgr.m_pHead = &head; g_mgr = &mgr; g_pending = &msg;
    CWorld world; world.PMessageEventManager = &mgr;
    CGameScriptInterface gsi; gsi.World = &world;
    CCharString m = { "CAM_DONE" };
    gsi.WaitForCameraMessage(m);
    if (g_frames != 3 || g_removed != 1 || head.m_pNext != &other) { std::printf("006e7820_TEST FAIL\n"); return 1; }
    g_frames = 0; g_stopAfter = 5; g_pending = 0;   // nothing arrives: returns when the runner stops
    gsi.WaitForCameraMessage(m);
    if (g_frames != 5 || g_removed != 1) { std::printf("006e7820_TEST FAIL\n"); return 1; }
    std::printf("006e7820_TEST PASS\n");
    return 0;
}
