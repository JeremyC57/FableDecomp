// Behaviour test for 0x008D2D90 (RST2): model plus the body verbatim from final.cpp, the engine types stubbed.
// Checks that an absent or dead thing and a thing without the entity-events component answer false; otherwise
// an aggressive special-ability hit (type 27, Aggressive set) with WorldFrame in (last, current] matches an empty name, or a name equal to the live
// originator's script name; other types, frames, names and absent originators do not; and every by-value
// CScriptThing from GetScriptThing is released.
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#define _snprintf snprintf
#endif
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
static int g_live = 0;
class CCharString { public:
    const char* Text;
    explicit CCharString(const char* text) : Text(text) { }
    bool operator==(const char* text) const { return strcmp(Text, text) == 0; }
    bool operator==(const CCharString& other) const { return strcmp(Text, other.Text) == 0; }
};
class CScriptThing { public:
    const char* Name;
    explicit CScriptThing(const char* name) : Name(name) { ++g_live; }
    CScriptThing(const CScriptThing& o) : Name(o.Name) { ++g_live; }
    ~CScriptThing() { --g_live; }
    CCharString GetName() const { return CCharString(Name); }
};
class CTCEntityEvents { public: union { unsigned char Events[0xc]; void* Align; }; };
struct CThingNative {
    unsigned int UsingComponents[9];
    unsigned char LifeFlags;
    const char* ScriptName;
    CTCEntityEvents* Events;
    CScriptThing GetScriptThing() const { return CScriptThing(ScriptName); }
    CTCEntityEvents* GetEvents() { return Events; }
};
struct CBaseIntelligentPointer {
    void* VTable; void* Info;
    void* GetPItem() const { return Info; }
};
struct PointerView : CBaseIntelligentPointer {
    CThingNative* Get() const { return reinterpret_cast<CThingNative*>(GetPItem()); }
};
class CSpawnedFuncBase { public: long LastScriptFrame; long CurrentScriptFrame; };
struct ScriptBaseNative { CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { ScriptBaseNative* Script; };
struct QuestManagerNative { ActiveQuest* PQuestCurrentlyRunning; };
static QuestManagerNative* QuestManager;
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }

struct EventBase {
    long Type;
    long WorldFrame;
    PointerView POriginator;
    bool Aggressive;
    long GetType() const { return Type; }
};
__forceinline bool IsThingAlive(const CThingNative* thing) { return thing && !(thing->LifeFlags & 1); }
struct CSpecialAbilityEvent : EventBase {
    CThingNative* GetPPerformer() const { return POriginator.Get(); }
};
struct EventNode { EventNode* Next; EventNode* Previous; EventBase* Data; void* Info; };
struct EventList { EventNode* Head; };
class CGameScriptThing { public: union { unsigned char PThing[sizeof(void*) * 2]; void* Align; }; };   // 8 bytes on x86
struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& name) const;
};

bool CGameScriptThingMethods::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& name) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
    if (p->Get() && !(p->Get()->LifeFlags & 1)) {
        EventNode* n = 0;
        long last = LastFrame(QuestManager->PQuestCurrentlyRunning);
        long current = CurrentFrame(QuestManager->PQuestCurrentlyRunning);
        CThingNative* thing = p->Get();
        if (thing->UsingComponents[2] & 4) {
            CTCEntityEvents* events = thing->GetEvents();
            for (n = reinterpret_cast<EventList*>(events->Events)->Head->Next; n != reinterpret_cast<EventList*>(events->Events)->Head; n = n->Next) {
                EventBase* e = n->Data;
                if (e->GetType() == 27 && e->WorldFrame > last && e->WorldFrame <= current) {
                    CSpecialAbilityEvent* ability = static_cast<CSpecialAbilityEvent*>(e);
                    bool hit = name == "" || (IsThingAlive(ability->GetPPerformer()) && ability->GetPPerformer()->GetScriptThing().GetName() == name);
                    if (hit && ability->Aggressive)
                        return true;
                }
            }
        }
    }
    return false;
}

static CThingNative g_self, g_hero, g_dead;
static EventBase g_ev[3];
static EventNode g_nodes[4];
static CTCEntityEvents g_events;
static CSpawnedFuncBase g_thread;
static ScriptBaseNative g_script;
static ActiveQuest g_quest;
static QuestManagerNative g_qm;
static bool Ask(CThingNative* self, const char* name)
{
    CGameScriptThingMethods t;
    PointerView* p = reinterpret_cast<PointerView*>(t.PThing);
    p->VTable = 0; p->Info = self;
    g_live = 0;
    bool r = t.MsgIsHitByAnyAggressiveSpecialAbilityFrom(CCharString(name));
    if (g_live != 0) { g_fail++; printf("FAIL leaked CScriptThing\n"); }
    return r;
}
int main()
{
    g_hero.ScriptName = "HERO"; g_hero.LifeFlags = 0;
    g_self.ScriptName = "SELF"; g_self.LifeFlags = 0; g_self.UsingComponents[2] = 4; g_self.Events = &g_events;
    // circular list: sentinel g_nodes[0] -> 1 -> 2 -> 3 -> sentinel
    for (int k = 0; k < 4; ++k) g_nodes[k].Next = &g_nodes[(k + 1) % 4];
    for (int k = 1; k < 4; ++k) g_nodes[k].Data = &g_ev[k - 1];
    reinterpret_cast<EventList*>(g_events.Events)->Head = &g_nodes[0];
    g_ev[0].Type = 3; g_ev[0].WorldFrame = 5; g_ev[0].POriginator.Info = &g_hero;   // wrong type
    g_ev[1].Type = 27; g_ev[1].WorldFrame = 3; g_ev[1].POriginator.Info = &g_hero;   // frame 3: not after last
    g_ev[2].Type = 27; g_ev[2].WorldFrame = 6; g_ev[2].POriginator.Info = 0;         // no originator
    g_ev[0].Aggressive = g_ev[1].Aggressive = g_ev[2].Aggressive = true;
    g_thread.LastScriptFrame = 3; g_thread.CurrentScriptFrame = 10;
    g_script.PActiveThread = &g_thread; g_quest.Script = &g_script; g_qm.PQuestCurrentlyRunning = &g_quest; QuestManager = &g_qm;
    CHECK(!Ask(&g_self, "HERO"));
    CHECK(Ask(&g_self, ""));                       // the originator-less event at 6 matches an empty name
    g_ev[1].WorldFrame = 4;
    CHECK(Ask(&g_self, "HERO"));
    CHECK(!Ask(&g_self, "BANDIT"));
    g_ev[1].Aggressive = false; g_ev[2].Aggressive = false; CHECK(!Ask(&g_self, "HERO")); CHECK(!Ask(&g_self, "")); g_ev[1].Aggressive = true; g_ev[2].Aggressive = true;   // non-aggressive hits do not count
    g_hero.LifeFlags = 1; CHECK(!Ask(&g_self, "HERO")); g_hero.LifeFlags = 0;   // dead performer
    g_thread.LastScriptFrame = 4; CHECK(!Ask(&g_self, "HERO")); g_thread.LastScriptFrame = 3;
    g_thread.CurrentScriptFrame = 3; CHECK(!Ask(&g_self, "HERO")); g_thread.CurrentScriptFrame = 10;
    g_quest.Script = 0; CHECK(!Ask(&g_self, "")); g_quest.Script = &g_script;   // no running thread: frames 0..0
    g_self.LifeFlags = 1; CHECK(!Ask(&g_self, "")); g_self.LifeFlags = 0;
    g_self.UsingComponents[2] = 0; CHECK(!Ask(&g_self, "")); g_self.UsingComponents[2] = 4;
    CHECK(!Ask(0, ""));
    char line[64];
    _snprintf(line, sizeof(line), "008d2d90_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}