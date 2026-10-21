// CGameScriptInterface::MsgOnLevelLoaded(std::list<CCharString>& levels) const -- append the script name of every level loaded since the previous message frame; true when the list is non-empty
// Leaf: ?MsgOnLevelLoaded@CGameScriptInterface@@UBE_NAAV?$list@VCCharString@@V?$allocator@VCCharString@@@std@@@std@@@Z
// Retail: Fable.exe @ 0x0089AC10, 109 bytes, `ret 4`; Ego_r.pdb same symbol.
// Body: frame = 0x006E7510, maxFrame = 0x006E7530 (both labelled GetMaxWorldFrameForMessages; the landed Msg family
// calls the first GetCurrentWorldFrameForMessages); World (+0x04)->PMessageEventManager (+0x60)->ForEachMessage(
// IsOfType(0x24) && OccuredBetween(frame, maxFrame), callback{World, &levels}) (0x008AD050, whose landed model has the
// callback push GetLevelScriptName(message.ExtraData) onto the list); return levels.begin() != levels.end().
// Frame: the IsOfType filter sits in the dead `levels` slot; OccuredBetween, callback and the Bind2 pair follow in the frame.
// Layout: CWorld +0x60 PMessageEventManager (engine/CWorld.h); the callback and list shapes are the landed 0x008AD050
// model (one-pointer STLport list, no size word).
// Shape: /O2 (16-aligned entry). Ego_r.pdb ??$ForEachMessage@...@CMessageEventManager@@QBE?BJABV...@@ABUCGameScriptInterface_MsgOnLevelLoaded_Callback@@@Z
// takes the callback by const reference: built as a temporary in the call, its stores land after the pushes as in retail;
// a named callback local stores World/&levels before them. World is bound to a local first.
struct CCharString {
    void* PStringData;
};
struct CMessageEvent;
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) {}
};
struct CMessageEventFilter_OccuredBetween {
    long StartFrame;
    long EndFrame;
    CMessageEventFilter_OccuredBetween(long start, long end) : StartFrame(start), EndFrame(end) {}
};
template<class F1, class F2> struct CFilter_Bind2 {
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
struct CMessageEventManager {
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