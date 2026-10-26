#include <cstdio>
class CCharString { public:
    void* PStringData;
};
class CMessageEvent;
class CMessageEventFilter_IsOfType { public:
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) {}
};
class CMessageEventFilter_OccuredBetween { public:
    long StartFrame;
    long EndFrame;
    CMessageEventFilter_OccuredBetween(long start, long end) : StartFrame(start), EndFrame(end) {}
};
template<class F1, class F2> class CFilter_Bind2 { public:
    const F1* PF1;
    const F2* PF2;
    CFilter_Bind2(const F1& a, const F2& b) : PF1(&a), PF2(&b) {}
};
template<class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template<class T> struct CList {                // retail one-pointer STLport list (no size)
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* m_node;
        iterator(node* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    struct SProxy { node* _M_data; SProxy(node* p) : _M_data(p) {} };
    SProxy head;
    iterator begin() { return iterator(head._M_data->next); }
    iterator end() { return iterator(head._M_data); }
};
class CWorld;
struct CGameScriptInterface_MsgOnLevelLoaded_Callback {
    CWorld*             World;
    CList<CCharString>* LevelNames;
    CGameScriptInterface_MsgOnLevelLoaded_Callback(CWorld* world, CList<CCharString>& names) : World(world), LevelNames(&names) {}
    void operator()(const CMessageEvent& message) const;
};
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> CLevelLoadedFilter;
class CMessageEventManager { public:
    template<class F, class C> const long ForEachMessage(const F& filter, const C& callback) const;   // <CLevelLoadedFilter, callback> 0x008AD050
};
class CWorld {
public:
    unsigned char         _pad00[0x60];
    CMessageEventManager* PMessageEventManager;  // +0x60
};
class CGameScriptInterface {
public:
    virtual bool MsgOnLevelLoaded(CList<CCharString>& levels) const;
    CWorld* World;                               // +0x04
    long GetCurrentWorldFrameForMessages() const;   // 0x006E7510 (labelled GetMaxWorldFrameForMessages)
    long GetMaxWorldFrameForMessages() const;       // 0x006E7530
};
bool CGameScriptInterface::MsgOnLevelLoaded(CList<CCharString>& levels) const
{
    long frame = GetCurrentWorldFrameForMessages();
    long maxFrame = GetMaxWorldFrameForMessages();
    CWorld* world = World;
    world->PMessageEventManager->ForEachMessage(CMessageEventFilter_IsOfType(0x24) && CMessageEventFilter_OccuredBetween(frame, maxFrame), CGameScriptInterface_MsgOnLevelLoaded_Callback(world, levels));
    return levels.begin() != levels.end();
}

static long g_type, g_start, g_end; static CWorld* g_world; static void* g_names; static int g_calls, g_hits;
long CGameScriptInterface::GetCurrentWorldFrameForMessages() const { return 100; }
long CGameScriptInterface::GetMaxWorldFrameForMessages() const { return 105; }
static CList<CCharString>::node g_extra;
void CGameScriptInterface_MsgOnLevelLoaded_Callback::operator()(const CMessageEvent&) const
{
    ++g_hits;   // link one node before the head (push_back)
    CList<CCharString>::node* h = LevelNames->head._M_data;
    g_extra.next = h; g_extra.prev = h->prev; h->prev->next = &g_extra; h->prev = &g_extra;
}
static int g_messages;
template<class F, class C> const long CMessageEventManager::ForEachMessage(const F& f, const C& c) const
{
    ++g_calls; g_type = f.PF1->Type; g_start = f.PF2->StartFrame; g_end = f.PF2->EndFrame; g_world = c.World; g_names = c.LevelNames;
    for (int i = 0; i < g_messages; ++i) c(*(const CMessageEvent*)0);
    return g_messages;
}
int main()
{
    CMessageEventManager mgr; static unsigned char raw[sizeof(CWorld)]; CWorld& w = *(CWorld*)raw; w.PMessageEventManager = &mgr;
    CGameScriptInterface gsi; gsi.World = &w;
    CList<CCharString>::node head; head.next = &head; head.prev = &head;
    CList<CCharString>& names = *(CList<CCharString>*)&raw[0x60 - 0x60 + 0]; (void)names;
    struct { CList<CCharString>::node* p; } lst; lst.p = &head;
    CList<CCharString>& l = *(CList<CCharString>*)&lst;
    g_messages = 0; bool ok = !gsi.MsgOnLevelLoaded(l);
    ok = ok && g_calls == 1 && g_type == 0x24 && g_start == 100 && g_end == 105 && g_world == &w && g_names == &l;
    g_messages = 1; ok = ok && gsi.MsgOnLevelLoaded(l) && g_hits == 1 && head.next == &g_extra;
    std::printf(ok ? "0089ac10_TEST PASS\n" : "0089ac10_TEST FAIL\n");
    return ok ? 0 : 1;
}