#include <cstdio>
#include <cstring>

struct CSpawnedFuncNative { long LastScriptFrame; long CurrentScriptFrame; };
struct ScriptBaseNative { CSpawnedFuncNative* PActiveThread; };
struct ActiveQuest { ScriptBaseNative* Script; };
struct QuestManagerNative { ActiveQuest* PQuestCurrentlyRunning; };
static QuestManagerNative* QuestManager;
static long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
static long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }
struct CEventBase {
    long Type; long WorldFrame; unsigned long Group;
    long GetType() const { return Type; }
};
struct SEventNode { SEventNode* Next; SEventNode* Prev; CEventBase* Event; };
struct CTCEntityEventsNative { SEventNode* Events; };
struct CThingNative {
    unsigned long UsingComponents[9]; CTCEntityEventsNative* EventsTC; unsigned char LifeFlags;
    bool HasTC(long type) const { if (UsingComponents[type >> 5] & (1UL << (type & 31))) return true; return false; }
};
struct CULongVector {
    unsigned long data[8]; int n;
    void push_back(const unsigned long& value) { data[n++] = value; }
};
struct CGameScriptThing {
    CThingNative* PThing;
    bool MsgGetThingsKilled(CULongVector& groups) const
    {
        bool found = false;
        if (PThing && !(PThing->LifeFlags & 1)) {
            long last = LastFrame(QuestManager->PQuestCurrentlyRunning);
            long current = CurrentFrame(QuestManager->PQuestCurrentlyRunning);
            CThingNative* thing = PThing;
            if (thing->HasTC(0x42)) {
                const CTCEntityEventsNative* events = thing->EventsTC;
                for (SEventNode* node = events->Events->Next; node != events->Events; node = node->Next) {
                    CEventBase* event = node->Event;
                    if (event->GetType() == 0x22 && event->WorldFrame > last && event->WorldFrame <= current) {
                        groups.push_back(event->Group);
                        found = true;
                    }
                }
            }
        }
        return found;
    }
};
int main()
{
    CSpawnedFuncNative thread = { 10, 13 }; ScriptBaseNative script = { &thread }; ActiveQuest quest = { &script };
    QuestManagerNative qm = { &quest }; QuestManager = &qm;
    CEventBase e1 = { 0x22, 11, 5 }, e2 = { 0x22, 10, 6 }, e3 = { 0x21, 12, 7 }, e4 = { 0x22, 13, 8 }, e5 = { 0x22, 14, 9 };
    SEventNode head, n[5]; CEventBase* ev[5] = { &e1, &e2, &e3, &e4, &e5 };
    head.Next = &n[0]; head.Prev = &n[4];
    for (int i = 0; i < 5; ++i) { n[i].Event = ev[i]; n[i].Next = i < 4 ? &n[i + 1] : &head; n[i].Prev = i ? &n[i - 1] : &head; }
    CTCEntityEventsNative tc = { &head };
    CThingNative thing; std::memset(&thing, 0, sizeof(thing)); thing.EventsTC = &tc; thing.UsingComponents[2] = 4;
    CGameScriptThing me = { &thing };
    CULongVector v; v.n = 0;
    if (!me.MsgGetThingsKilled(v) || v.n != 2 || v.data[0] != 5 || v.data[1] != 8) { std::printf("008d3dd0_TEST FAIL\n"); return 1; }
    thing.UsingComponents[2] = 0; v.n = 0;
    if (me.MsgGetThingsKilled(v) || v.n != 0) { std::printf("008d3dd0_TEST FAIL\n"); return 1; }
    thing.UsingComponents[2] = 4; thing.LifeFlags = 1;
    if (me.MsgGetThingsKilled(v)) { std::printf("008d3dd0_TEST FAIL\n"); return 1; }
    thing.LifeFlags = 0; quest.Script = 0;          // no running script: the window is (0, 0]
    if (me.MsgGetThingsKilled(v)) { std::printf("008d3dd0_TEST FAIL\n"); return 1; }
    std::printf("008d3dd0_TEST PASS\n");
    return 0;
}
