// CGameScriptThing::MsgIsHitBy(const CCharString& attackerScriptName) const -- whether the live thing got a "hit by" entity event (TC 0x42) since the running quest script's last frame: from anyone for an empty name, else from a thing with that script name
// Leaf: MsgIsHitBy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D0FB0, length 381 bytes (`ret 4`, bool in al).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the const component-map LowerBound @ 0x004365B0,
//   CBasicString<char>::operator== @ 0x004115A0 ("" @ 0x0122D70E), CCharString copy ctor/dtor @ 0x0099EC30 / 0x0099EAE0,
//   CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn,
//   CIsHitEventHitByThingWithScriptName>> @ 0x008D4840 and GetEvent<CEventHitBy, CFilter_Bind2<CIsEventCreatedAfter,
//   CIsEventCreatedBeforeOrOn>> @ 0x008D47A0; the quest manager @ 0x013B89FC (running quest frames).
// Layout: engine/CGameScriptThing.h (PThing +0xC), engine/CSpawnedFuncBase.h; retail CThing UsingComponents +0x20,
//   component map +0x44, dead +0x91 (the landed MsgIsHitByWithFlourish 0x008D1330 / MsgReceivedInventoryItem 0x008D2540
//   models).
// Shape: the empty-name test is the inline operator==("") written as two tests that jump to the empty arm; the script-name
//   filter copies the name into the dead name slot and the GetEvent result is kept in bl across its destructor.
#include <string.h>
#include "engine/CGameScriptThing.h"
#include "engine/CSpawnedFuncBase.h"
#pragma pack(push, 1)
template <class T> class CBasicString { public:                               // CBasicString<char>, 13 bytes
    char*         PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    bool operator==(const char* text) const;            // 0x004115A0
};
class CCharStringData { public:                                // 17 bytes
    CBasicString<char> Data;                              // +0x00
    long             NoRefs;                            // +0x0d
};
#pragma pack(pop)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
struct ComponentEntry { long Type; void* Component; };
struct ComponentMap {
    ComponentEntry* Start;
    ComponentEntry* Finish;
    const ComponentEntry* LowerBound(const long& key) const;   // 0x004365B0
};
struct CThingNative {
    void*          VTable;
    unsigned char  Base[0x1c];
    unsigned int   UsingComponents[9];                  // +0x20
    ComponentMap   Components;                          // +0x44
    unsigned char  Metadata[0x45];
    unsigned char  LifeFlags;                           // +0x91 (bit 0 = dead)
};
struct PointerView {                                    // CIntelligentPointer<CThing>
    void*            VTable;
    void*            Info;
    CThingNative* GetPItem() const;                     // 0x00A01B50
};
struct ScriptBaseNative { unsigned char Containers[0x2c]; CSpawnedFuncBase* PActiveThread; };
struct ActiveQuest { unsigned char State[8]; ScriptBaseNative* Script; };
struct QuestManagerNative { unsigned char State[0x88]; ActiveQuest* PQuestCurrentlyRunning; };
extern QuestManagerNative* g_pQuestManager;             // 0x013B89FC
__forceinline long LastFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->LastScriptFrame : 0; }
__forceinline long CurrentFrame(ActiveQuest* q) { return q && q->Script ? q->Script->PActiveThread->CurrentScriptFrame : 0; }
struct CEventHitBy;                                     // NEntityEvents::CEventHitBy
struct CIsEventCreatedAfter { long Frame; CIsEventCreatedAfter(long f) : Frame(f) {} };
struct CIsEventCreatedBeforeOrOn { long Frame; CIsEventCreatedBeforeOrOn(long f) : Frame(f) {} };
struct CIsHitEventHitByThingWithScriptName {
    CCharString ScriptName;
    CIsHitEventHitByThingWithScriptName(const CCharString& s) : ScriptName(s) {}
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* First; const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) {}
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* First; const B* Second; const C* Third;
    CFilter_Bind3(const A& a, const B& b, const C& c) : First(&a), Second(&b), Third(&c) {}
};
typedef CFilter_Bind2<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn> CInFramesFilter;
struct CTCEntityEventsNative {
    bool GetEvent(CEventHitBy** out, const CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>& filter) const;   // 0x008D4840
    bool GetEvent(CEventHitBy** out, const CInFramesFilter& filter) const;   // 0x008D47A0
};
template <class F> __forceinline bool HasHitEvent(const CTCEntityEventsNative* events, const F& filter)
{
    CEventHitBy* event;
    return events->GetEvent(&event, filter);
}

struct CGameScriptThingMethods : CGameScriptThing {
    bool MsgIsHitBy(const CCharString& attackerScriptName) const;
};

extern "C" int StandInSink(int, int);
#pragma auto_inline(off)
bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CFilter_Bind3<CIsEventCreatedAfter, CIsEventCreatedBeforeOrOn, CIsHitEventHitByThingWithScriptName>& filter) const
{
    *out = (CEventHitBy*)StandInSink((int)this, (int)filter.First);
    return *out != 0;
}
bool CTCEntityEventsNative::GetEvent(CEventHitBy** out, const CInFramesFilter& filter) const
{
    *out = (CEventHitBy*)StandInSink((int)this, *(const int*)&filter);
    return *out != 0;
}
#pragma auto_inline(on)
bool CGameScriptThingMethods::MsgIsHitBy(const CCharString& attackerScriptName) const
{
    const PointerView* p = reinterpret_cast<const PointerView*>(PThing);
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
            } else if (attackerScriptName.PStringData->Data == "")
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