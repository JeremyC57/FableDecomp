// 008d0fb0 behaviour test: CGameScriptThing::MsgIsHitBy (match.cpp body, host-modelled types)
#include <stdio.h>
#include <string.h>
#include <string>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct CCharString { const char* PStringData; };            // 0 = null data
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start; ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const { const ComponentEntry* f = Start; while (f != Finish && f->Type < key) ++f; return f; }
};
struct CThingNative { unsigned int UsingComponents[9]; ComponentMap Components; unsigned char LifeFlags; };
struct PointerView { CThingNative* Item; CThingNative* GetPItem() const { return Item; } };
struct CSpawnedFuncBase { long LastScriptFrame, CurrentScriptFrame; };
struct ScriptBaseNative { CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { ScriptBaseNative* Script; };
struct QuestManagerNative { ActiveQuest* PQuestCurrentlyRunning; };
static QuestManagerNative g_qm; static QuestManagerNative* g_pQuestManager = &g_qm;
inline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
inline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }
struct CEventHitBy { long Frame; std::string Attacker; };
struct CIsEventCreatedAfter { long Frame; CIsEventCreatedAfter(long f) : Frame(f) {} };
struct CIsEventCreatedBeforeOrOn { long Frame; CIsEventCreatedBeforeOrOn(long f) : Frame(f) {} };
struct CIsHitEventHitByThingWithScriptName { std::string ScriptName; CIsHitEventHitByThingWithScriptName(const CCharString& s) : ScriptName(s.PStringData ? s.PStringData : "") {} };
template <class A, class B> struct CFilter_Bind2 { const A* First; const B* Second; CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {} };
template <class A, class B, class C> struct CFilter_Bind3 { const A* First; const B* Second; const C* Third; CFilter_Bind3(const A& a, const B& b, const C& c) : First(&a), Second(&b), Third(&c) {} };
typedef CFilter_Bind2<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn> CInFramesFilter;
struct CTCEntityEventsNative {
    CEventHitBy Events[4]; int Count;
    bool GetEvent(CEventHitBy** out, const CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>& f) const {
        for (int i = 0; i < Count; ++i) if (Events[i].Frame > f.First->Frame && Events[i].Frame <= f.Second->Frame && Events[i].Attacker == f.Third->ScriptName) { *out = (CEventHitBy*)&Events[i]; return true; }
        return false; }
    bool GetEvent(CEventHitBy** out, const CInFramesFilter& f) const {
        for (int i = 0; i < Count; ++i) if (Events[i].Frame > f.First->Frame && Events[i].Frame <= f.Second->Frame) { *out = (CEventHitBy*)&Events[i]; return true; }
        return false; }
};
struct CGameScriptThing { PointerView* PThing; bool MsgIsHitBy(const CCharString& attackerScriptName) const; };
bool CGameScriptThing::MsgIsHitBy(const CCharString& attackerScriptName) const
{
    const PointerView* p = PThing;
    if (p->GetPItem() && !(p->GetPItem()->LifeFlags & 1)) {
        long last = LastFrame(g_pQuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(g_pQuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = p->GetPItem();
        if (thing->UsingComponents[2] & 4) {
            const ComponentMap* map = &thing->Components;
            const ComponentEntry* it = map->LowerBound(0x42);
            const ComponentEntry* end = map->Finish;
            const CTCEntityEventsNative* events = (const CTCEntityEventsNative*)((it == end || it->Type > 0x42) ? end : it)->Component;
            CEventHitBy* event;
            if (attackerScriptName.PStringData == 0) {
                if (strcmp("", "") == 0)
                    goto anyone;
            } else if (strcmp(attackerScriptName.PStringData, "") == 0)
                goto anyone;
            {
                if (events->GetEvent(&event, CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>(
                        CIsEventCreatedAfter(last), CIsEventCreatedBeforeOrOn(current), CIsHitEventHitByThingWithScriptName(attackerScriptName))))
                    return true;
                goto none;
            }
anyone:
            {
                if (events->GetEvent(&event, CInFramesFilter(CIsEventCreatedAfter(last), CIsEventCreatedBeforeOrOn(current))))
                    return true;
            }
        }
    }
none:
    return false;
}
int main()
{
    CSpawnedFuncBase thread = { 10, 20 }; ScriptBaseNative sb = { &thread }; ActiveQuest q = { &sb }; g_qm.PQuestCurrentlyRunning = &q;
    CTCEntityEventsNative ev; ev.Count = 1; ev.Events[0].Frame = 15; ev.Events[0].Attacker = "Bandit";
    ComponentEntry entries[2] = { { 0x10, 0 }, { 0x42, &ev } };
    CThingNative t; memset(t.UsingComponents, 0, sizeof t.UsingComponents); t.UsingComponents[2] = 4; t.Components.Start = entries; t.Components.Finish = entries + 2; t.LifeFlags = 0;
    PointerView pv = { &t }; CGameScriptThing g = { &pv };
    CCharString none = { 0 }, empty = { "" }, bandit = { "Bandit" }, hero = { "Hero" };
    CHECK(g.MsgIsHitBy(none)); CHECK(g.MsgIsHitBy(empty)); CHECK(g.MsgIsHitBy(bandit)); CHECK(!g.MsgIsHitBy(hero));
    thread.LastScriptFrame = 15;                                    // event not after last frame
    CHECK(!g.MsgIsHitBy(none)); CHECK(!g.MsgIsHitBy(bandit));
    thread.LastScriptFrame = 10; t.LifeFlags = 1; CHECK(!g.MsgIsHitBy(none));   // dead
    t.LifeFlags = 0; t.UsingComponents[2] = 0; CHECK(!g.MsgIsHitBy(none));     // no event component
    t.UsingComponents[2] = 4; pv.Item = 0; CHECK(!g.MsgIsHitBy(none));         // no item
    pv.Item = &t; g_qm.PQuestCurrentlyRunning = 0; CHECK(!g.MsgIsHitBy(none)); // no quest: frames 0,0
    if (!g_fail) printf("008d0fb0_TEST PASS\n");
    return g_fail;
}