// Behaviour test for 008ad050: src/008ad050.cpp copied verbatim (the function body and its same-TU stand-ins), the remaining
// declared callees stubbed below so calls and arguments are observable. CMessageEventManager::ForEachMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween>,CGameScriptInterface_MsgOnLevelLoaded_Callback>(const Filter& filter, Callback& callback)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CMessageEventManager::ForEachMessage<CFilter_Bind2<CMessageEventFilter_IsOfType,CMessageEventFilter_OccuredBetween>,CGameScriptInterface_MsgOnLevelLoaded_Callback>(const Filter& filter, Callback& callback) -- run the callback on every queued message that passes both filters and count them; the callback appends each loaded level's script name
// Leaf: ForEachMessage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AD050, length 202 bytes (`ret 8`).
// Callees: CMessageEventFilter_IsOfType::operator() @ 0x006AF1C0 (labelled CMessageEventFilter_HasExtraData), CMessageEventFilter_OccuredBetween::
// operator() @ 0x006AF180, CWorld::PeekWorldMap (vtable slot 13), CWorldMap::GetLevelScriptName(long) @ 0x004FCFC0, malloc @ 0x00BFEA0E
// (the 12-byte list node), CCharString copy ctor @ 0x0099EC30.
// Layout: Ego_r.pdb CMessageEventManager World +0, Messages +4 (retail one-pointer list; node next +0, prev +4, message +8);
// CMessageEvent::ExtraData +0x3C (the level number for this message type); CFilter_Bind2 {F1, F2} pointers.
// Shape: the count, the cursor and `this` live in the frame; the callback's level-name argument is pushed before the PeekWorldMap virtual
// call (`World->PeekWorldMap()->GetLevelScriptName(message.ExtraData)`); the name list has no size word (push_back links before the head).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct CCharString {
    void* PStringData;
    CCharString(const CCharString& other);      // 0x0099EC30
};
struct CMessageEvent {
    unsigned char _pad00[0x3c];
    long          ExtraData;                    // +0x3c
};
struct CMessageEventFilter_IsOfType {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF1C0
};
struct CMessageEventFilter_OccuredBetween {
    bool operator()(const CMessageEvent& message) const;   // 0x006AF180
};
template<class F1, class F2> struct CFilter_Bind2 {
    F1* PF1;
    F2* PF2;
    bool operator()(const CMessageEvent& message) const { return (*PF1)(message) && (*PF2)(message); }
};
struct CWorldMap {
    const CCharString& GetLevelScriptName(long level) const;   // 0x004FCFC0
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* PeekWorldMap();          // slot 13 (+0x34)
};
template<class T> struct CList {                // retail one-pointer list (no size), STLport shape
    struct node { node* next; node* prev; T value; };
    struct iterator {
        node* m_node;
        iterator(node* n) : m_node(n) {}
        iterator(const iterator& o) : m_node(o.m_node) {}
        T& operator*() const { return m_node->value; }
        iterator& operator++() { m_node = m_node->next; return *this; }
        bool operator!=(const iterator& o) const { return m_node != o.m_node; }
    };
    struct SProxy { node* _M_data; SProxy(node* p) : _M_data(p) {} };
    SProxy head;
    iterator begin() { return iterator(head._M_data->next); }
    iterator end() { return iterator(head._M_data); }
    node* create_node(const T& value)
    {
        node* n = (node*)malloc(sizeof(node));
        new (&n->value) T(value);
        return n;
    }
    void insert(iterator position, const T& value)
    {
        node* n = create_node(value);
        node* pos = position.m_node;
        node* prev = pos->prev;
        n->next = pos;
        n->prev = prev;
        prev->next = n;
        pos->prev = n;
    }
    void push_back(const T& value) { insert(end(), value); }
};
struct CGameScriptInterface_MsgOnLevelLoaded_Callback {
    CWorld*             World;
    CList<CCharString>* LevelNames;
    void operator()(const CMessageEvent& message) { LevelNames->push_back(World->PeekWorldMap()->GetLevelScriptName(message.ExtraData)); }
};
struct CMessageEventManager {
    CWorld*              World;                 // +0x00
    CList<CMessageEvent> Messages;              // +0x04
    template<class Filter, class Callback> long ForEachMessage(const Filter& filter, Callback& callback);
};

template<class Filter, class Callback> long CMessageEventManager::ForEachMessage(const Filter& filter, Callback& callback)
{
    long count = 0;
    for (CList<CMessageEvent>::iterator it = Messages.begin(); it != Messages.end(); ++it) {
        const CMessageEvent* message = &*it;
        if (filter(*message)) {
            callback(*message);
            ++count;
        }
    }
    return count;
}
template long CMessageEventManager::ForEachMessage<CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>,
    CGameScriptInterface_MsgOnLevelLoaded_Callback>(const CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween>&,
    CGameScriptInterface_MsgOnLevelLoaded_Callback&);

// ---- stubs and driver ----

static int g_copies = 0;
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_copies; }
bool CMessageEventFilter_IsOfType::operator()(const CMessageEvent& message) const { return *(const long*)&message == 0x24; }
bool CMessageEventFilter_OccuredBetween::operator()(const CMessageEvent& message) const { long f = ((const long*)&message)[1]; return f >= 10 && f <= 20; }
static void* g_nameStore[4];
const CCharString& CWorldMap::GetLevelScriptName(long level) const { return *(const CCharString*)&g_nameStore[level & 3]; }
static CWorldMap g_map;
CWorldMap* CWorld::PeekWorldMap() { return &g_map; }
void CWorld::v00() {}
void CWorld::v01() {}
void CWorld::v02() {}
void CWorld::v03() {}
void CWorld::v04() {}
void CWorld::v05() {}
void CWorld::v06() {}
void CWorld::v07() {}
void CWorld::v08() {}
void CWorld::v09() {}
void CWorld::v10() {}
void CWorld::v11() {}
void CWorld::v12() {}

typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_OccuredBetween> TFilter;
static void Add(CList<CMessageEvent>& l, long type, long frame, long map)
{
    CMessageEvent e; memset(&e, 0, sizeof e); ((long*)&e)[0] = type; ((long*)&e)[1] = frame; e.ExtraData = map; l.push_back(e);
}
int main()
{
    for (int i = 0; i < 4; ++i) g_nameStore[i] = (void*)(0x200 + i);
    static unsigned char mhs[sizeof(CList<CMessageEvent>::node)], nhs[sizeof(CList<CCharString>::node)], ms[sizeof(CMessageEventManager)], ns[sizeof(CList<CCharString>)];
    CList<CMessageEvent>::node& mh = *(CList<CMessageEvent>::node*)mhs; mh.next = &mh; mh.prev = &mh;
    CList<CCharString>::node& nh = *(CList<CCharString>::node*)nhs; nh.next = &nh; nh.prev = &nh;
    CMessageEventManager& manager = *(CMessageEventManager*)ms; manager.Messages.head._M_data = &mh;
    CWorld world;
    Add(manager.Messages, 0x24, 15, 1); Add(manager.Messages, 0x23, 15, 2); Add(manager.Messages, 0x24, 30, 3); Add(manager.Messages, 0x24, 10, 2);
    CMessageEventFilter_IsOfType f1; CMessageEventFilter_OccuredBetween f2; TFilter filter; filter.PF1 = &f1; filter.PF2 = &f2;
    CList<CCharString>& names = *(CList<CCharString>*)ns; names.head._M_data = &nh;
    CGameScriptInterface_MsgOnLevelLoaded_Callback cb; cb.World = &world; cb.LevelNames = &names;
    long n = manager.ForEachMessage(filter, cb);
    CHECK(n == 2); CHECK(g_copies == 2);
    CList<CCharString>::node* a1 = nh.next; CList<CCharString>::node* a2 = a1->next;
    CHECK(a1->value.PStringData == (void*)0x201); CHECK(a2->value.PStringData == (void*)0x202); CHECK(a2->next == &nh); CHECK(nh.prev == a2);
    static unsigned char ehs[sizeof(CList<CMessageEvent>::node)], ems[sizeof(CMessageEventManager)];
    CList<CMessageEvent>::node& eh = *(CList<CMessageEvent>::node*)ehs; eh.next = &eh; eh.prev = &eh;
    CMessageEventManager& emptyManager = *(CMessageEventManager*)ems; emptyManager.Messages.head._M_data = &eh;
    CHECK(emptyManager.ForEachMessage(filter, cb) == 0); CHECK(g_copies == 2);
    if (fails == 0) printf("008ad050_TEST PASS\n"); else printf("008ad050_TEST FAIL\n");
    return fails;
}