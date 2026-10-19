// Behaviour test for 0x006E7920 CGameScriptInterface::CheckForCameraMessage: no event, dead/absent target, target
// without the spline TC, spline without the message, and the consumed match.
#include <cstdio>
#include <cstring>
class CCharString { public: const char* text; };
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
struct CThing {
    enum { ThingTypeMask = 0 };
    unsigned long ThingTypeFlags;
    virtual ~CThing() {}
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char m_dead : 1;
    unsigned char m_91_1 : 7;
    CInterfaceMap::SEntry m_entries[8];
    CThing() : m_dead(0), m_91_1(0) { for (int i = 0; i < 9; ++i) m_presentBits[i] = 0; m_interfaces.first = m_interfaces.last = m_entries; }
    void Add(long type, void* tc)   // keep sorted
    {
        CInterfaceMap::SEntry* it = m_interfaces.last;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type; it->component = (CTCBase*)tc; ++m_interfaces.last;
        m_presentBits[type >> 5] |= 1UL << (type & 31);
    }
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> T* GetTC(long interfaceType)
    {
        T* tc = 0;
        GetInterface(&tc, interfaceType);
        return tc;
    }
};
template <class T> inline bool IsThingOfType(const CThing* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CBaseIntelligentPointer { CThing* item; CThing* GetPItem() const { return item; } };
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPTypedItem() const { return (GetPItem() != 0 && IsThingOfType<T>((T*)GetPItem())) ? (T*)GetPItem() : 0; }
};
struct CMessageEvent { long type; CIntelligentPointer<CThing> PTarget; };
struct CMessageEventFilter_IsOfType { long m_type; CMessageEventFilter_IsOfType(long type) : m_type(type) {} };
struct CMessageEventManager {
    CMessageEvent* event; int removed;
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& f) { return event && event->type == f.m_type ? event : 0; }
    void RemoveMessage(const CMessageEvent* m) { if (m == event) { event = 0; ++removed; } }
};
template<class T> struct CCountedPointer { T* Data; T* operator->() const { return Data; } };
struct CWorld { CCountedPointer<CMessageEventManager> PMessageEventManager; };
struct CTCCameraPointScriptedSpline {
    const char* msg;
    bool HasMessage(const CCharString& m) { return std::strcmp(msg, m.text) == 0; }
};
struct CGameScriptInterface {
    CWorld* World;
    bool CheckForCameraMessage(const CCharString& message) const;
};
bool CGameScriptInterface::CheckForCameraMessage(const CCharString& message) const
{
    CMessageEvent* event = World->PMessageEventManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x2b));
    if (event) {
        CThing* thing = event->PTarget.GetPTypedItem();
        CTCCameraPointScriptedSpline* spline;
        if (thing && !thing->m_dead && thing->GetInterface(&spline, 0x3f)) {
            if (spline->HasMessage(message) == true) {
                World->PMessageEventManager->RemoveMessage(event);
                return true;
            }
        }
    }
    return false;
}
int main()
{
    CMessageEventManager mm = { 0, 0 }; CWorld world; world.PMessageEventManager.Data = &mm;
    CGameScriptInterface gsi = { &world };
    CCharString msg = { "CAM_DONE" };
    CThing cam; cam.ThingTypeFlags = 0x40;
    CTCCameraPointScriptedSpline spline = { "OTHER" };
    CMessageEvent ev; ev.type = 0x2b; ev.PTarget.item = 0;
    int fail = 0;
    if (gsi.CheckForCameraMessage(msg)) fail |= 1;            // no event
    mm.event = &ev;
    if (gsi.CheckForCameraMessage(msg)) fail |= 2;            // no target
    ev.PTarget.item = &cam;
    if (gsi.CheckForCameraMessage(msg)) fail |= 4;            // no spline TC
    cam.Add(0x3f, &spline); cam.Add(0x10, 0);
    if (gsi.CheckForCameraMessage(msg) || mm.removed) fail |= 8;   // other message
    spline.msg = "CAM_DONE"; cam.m_dead = 1;
    if (gsi.CheckForCameraMessage(msg)) fail |= 16;           // dead target
    cam.m_dead = 0; ev.type = 0x2c;
    if (gsi.CheckForCameraMessage(msg)) fail |= 32;           // wrong event type
    ev.type = 0x2b;
    if (!gsi.CheckForCameraMessage(msg) || mm.removed != 1 || mm.event) fail |= 64;
    if (fail) { std::printf("006e7920_TEST FAIL\n"); return 1; }
    std::printf("006e7920_TEST PASS\n");
    return 0;
}