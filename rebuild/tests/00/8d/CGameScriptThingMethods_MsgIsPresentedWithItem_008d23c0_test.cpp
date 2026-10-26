// 008d23c0 behaviour test: CGameScriptThing::MsgIsPresentedWithItem (match.cpp body: an implicit
// CDefString -> CCharString conversion at operator=), host stand-ins. C++03, g++ -w.
#include <stdio.h>
#include <string>
#include <vector>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)

class CCharString { public:
    std::string S;
    CCharString() { }
    CCharString(const std::string& s) : S(s) { }
};
static std::vector<std::string> g_names;
static int g_getStringCalls = 0;
class CDefString { public:
    long m_index;
    operator CCharString() const;
};
struct CDefStringTable_Retail {
    CCharString GetString(long index) { ++g_getStringCalls; return CCharString(g_names[index]); }
};
static CDefStringTable_Retail g_defStringTable;
inline CDefString::operator CCharString() const { return g_defStringTable.GetString(m_index); }

struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    std::vector<ComponentEntry> Entries;
    const ComponentEntry* Finish() const { return Entries.empty() ? 0 : &Entries[0] + Entries.size(); }
    const ComponentEntry* LowerBound(const long& key) const
    {
        const ComponentEntry* p = Entries.empty() ? 0 : &Entries[0];
        const ComponentEntry* e = Finish();
        while (p != e && p->Type < key) ++p;
        return p;
    }
};
struct CThingNative {
    unsigned int UsingComponents[9];
    ComponentMap Components;
    unsigned char LifeFlags;
    long DefIndex;
    CDefString GetDefName() const { CDefString d; d.m_index = DefIndex; return d; }
};
struct PointerView {
    CThingNative* Item;
    CThingNative* GetPItem() const { return Item; }
};
struct EventBase {
    long WorldFrame;
    int Type;
    virtual ~EventBase() { }
    virtual int GetType() const { return Type; }
};
struct ItemPresentedEvent : EventBase {
    PointerView PresentedBy;
    PointerView Item;
};
struct EventNode { EventNode* Next; EventNode* Previous; EventBase* Data; };
struct CTCEntityEventsNative { EventNode* Head; };
class CSpawnedFuncBase { public: long LastScriptFrame, CurrentScriptFrame; };
struct ScriptBaseNative { CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { ScriptBaseNative* Script; };
struct QuestManagerNative { ActiveQuest* PQuestCurrentlyRunning; };
static QuestManagerNative* g_pQuestManager;
static long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
static long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

class CGameScriptThing { public:
    PointerView PThing;
    bool MsgIsPresentedWithItem(CCharString& itemDefName) const;
};

bool CGameScriptThing::MsgIsPresentedWithItem(CCharString& itemDefName) const
{
    const PointerView* p = &PThing;
    if (p->GetPItem() && !(p->GetPItem()->LifeFlags & 1)) {
        long last = LastFrame(g_pQuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(g_pQuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = p->GetPItem();
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound(0x42);
            const ComponentEntry* end = map->Finish();
            CTCEntityEventsNative* events = (CTCEntityEventsNative*)((it == end || it->Type > 0x42) ? end : it)->Component;
            for (EventNode* node = events->Head->Next; node != events->Head; node = node->Next) {
                EventBase* event = node->Data;
                if (event->WorldFrame > last && event->WorldFrame <= current) {
                    switch (event->GetType()) {
                    case 10:
                        itemDefName = ((ItemPresentedEvent*)event)->Item.GetPItem()->GetDefName();
                        return true;
                    case 11:
                        itemDefName = ((ItemPresentedEvent*)event)->Item.GetPItem()->GetDefName();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int main()
{
    g_names.push_back("OBJECT_NONE");
    g_names.push_back("OBJECT_SWORD");
    g_names.push_back("OBJECT_APPLE");

    CSpawnedFuncBase thread = { 10, 20 };
    ScriptBaseNative script = { &thread };
    ActiveQuest quest = { &script };
    QuestManagerNative qm = { &quest };
    g_pQuestManager = &qm;

    CThingNative sword; for (int i = 0; i < 9; ++i) sword.UsingComponents[i] = 0; sword.LifeFlags = 0; sword.DefIndex = 1;
    CThingNative apple = sword; apple.DefIndex = 2;

    ItemPresentedEvent evOld; evOld.WorldFrame = 10; evOld.Type = 10; evOld.Item.Item = &apple;   // not after last
    ItemPresentedEvent evOther; evOther.WorldFrame = 15; evOther.Type = 3; evOther.Item.Item = &apple;
    ItemPresentedEvent evHit; evHit.WorldFrame = 18; evHit.Type = 11; evHit.Item.Item = &sword;
    ItemPresentedEvent evLate; evLate.WorldFrame = 21; evLate.Type = 10; evLate.Item.Item = &apple;  // after current

    EventNode head, n1, n2, n3, n4;
    head.Next = &n1; n1.Next = &n2; n2.Next = &n3; n3.Next = &n4; n4.Next = &head;
    n1.Data = &evOld; n2.Data = &evOther; n3.Data = &evHit; n4.Data = &evLate;
    CTCEntityEventsNative events = { &head };

    CThingNative me; for (int i = 0; i < 9; ++i) me.UsingComponents[i] = 0; me.LifeFlags = 0; me.DefIndex = 0;
    me.UsingComponents[2] = 4;
    ComponentEntry e1 = { 0x10, 0 }, e2 = { 0x42, &events }, e3 = { 0x50, 0 };
    me.Components.Entries.push_back(e1); me.Components.Entries.push_back(e2); me.Components.Entries.push_back(e3);

    CGameScriptThing gst; gst.PThing.Item = &me;
    CCharString name("unset");

    // type 11 inside (last, current] wins; the earlier non-item event is skipped
    CHECK(gst.MsgIsPresentedWithItem(name));
    CHECK(name.S == "OBJECT_SWORD");
    CHECK(g_getStringCalls == 1);

    // type 10 also counts
    evHit.Type = 10; evHit.Item.Item = &apple; name.S = "unset";
    CHECK(gst.MsgIsPresentedWithItem(name) && name.S == "OBJECT_APPLE");

    // no event in the window: false, name untouched
    evHit.WorldFrame = 25; name.S = "unset";
    CHECK(!gst.MsgIsPresentedWithItem(name) && name.S == "unset");
    evHit.WorldFrame = 18;

    // the frame window is (last, current]: frame == current counts, frame == last does not
    evHit.WorldFrame = 20; CHECK(gst.MsgIsPresentedWithItem(name));
    evHit.WorldFrame = 10; CHECK(!gst.MsgIsPresentedWithItem(name));
    evHit.WorldFrame = 18;

    // no running quest: window (0, 0] is empty
    qm.PQuestCurrentlyRunning = 0; CHECK(!gst.MsgIsPresentedWithItem(name)); qm.PQuestCurrentlyRunning = &quest;

    // no entity-events component bit, dead thing, no thing: false
    me.UsingComponents[2] = 0; CHECK(!gst.MsgIsPresentedWithItem(name)); me.UsingComponents[2] = 4;
    me.LifeFlags = 1; CHECK(!gst.MsgIsPresentedWithItem(name)); me.LifeFlags = 0;
    gst.PThing.Item = 0; CHECK(!gst.MsgIsPresentedWithItem(name)); gst.PThing.Item = &me;

    // sanity: back to a hit
    CHECK(gst.MsgIsPresentedWithItem(name) && name.S == "OBJECT_APPLE");

    if (!g_fail) printf("008d23c0_TEST PASS\n");
    return g_fail;
}