// Behaviour test for CGameScriptThing::MsgWhoKilledMe (0x008D0610): the filters carry type 9 and this thing's UID; a
// live originator's script thing is returned, a missing message / missing or dead originator gives the null thing.
#include <cstdio>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __cdecl
#define __forceinline inline
#define __declspec(x)
#endif
#ifndef _MSC_VER
#define __int64 long long
#endif
struct CMessageEventManager;
struct CWorld { CMessageEventManager* PMessageEventManager_Data; };            // test model of engine/CWorld.h (+0x60 there)
struct CGameScriptThing { unsigned __int64 ThingUID; CWorld* World; };       // test model of engine/CGameScriptThing.h
static int g_getpitem = 0;
struct CBaseObject;
struct CScriptThing {                              // 12 bytes (vptr + PImp {Data, Info}), returned by hidden pointer
    void* __vftable;
    void* PImp_Data;
    void* PImp_Info;
    CScriptThing() : __vftable(0), PImp_Data(0), PImp_Info(0) { }
    CScriptThing(const CScriptThing& other) : __vftable(other.__vftable), PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info) { }
};
struct CThingRetail {                              // retail CThing (0x10 below Ego_r.pdb from ThingTypeFlags on)
    enum { ThingTypeMask = 0 };                    // every thing matches the CThing type mask
    unsigned char _pad00[0x6c];
    unsigned long ThingTypeFlags;                  // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                        // +0x91 bit 0
    unsigned char _bits91 : 7;
    CScriptThing GetScriptThing() const { CScriptThing t; t.PImp_Data = (void*)this; return t; }
};
template <class T> inline bool IsThingOfType(const CThingRetail* const& thing)
{
    unsigned long mask = T::ThingTypeMask;
    return (thing->ThingTypeFlags & mask) == mask;
}
struct CBaseIntelligentPointer {
    void* __vftable;
    void* Info;
    CBaseObject* GetPItem() const { ++g_getpitem; return (CBaseObject*)Info; }
};
struct CMessageEvent {                             // 0x58 bytes
    unsigned char           _pad00[0x48];
    CBaseIntelligentPointer POriginator;           // +0x48 CIntelligentPointer<CThing>
    CBaseIntelligentPointer PTarget;               // +0x50
};
struct CMessageEventFilter_IsOfType {
    long Type;
    CMessageEventFilter_IsOfType(long type) : Type(type) { }
};
struct CMessageEventFilter_IsTargettedAtThing {
    unsigned __int64 UID;
    CMessageEventFilter_IsTargettedAtThing(unsigned __int64 uid) : UID(uid) { }
};
template <class A, class B> struct CFilter_Bind2 { // the bound filter holds POINTERS to its operands
    const A* m_pFirst;
    const B* m_pSecond;
    CFilter_Bind2(const A& a, const B& b) : m_pFirst(&a), m_pSecond(&b) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
typedef CFilter_Bind2<CMessageEventFilter_IsOfType, CMessageEventFilter_IsTargettedAtThing> CKilledMeFilter;
struct CMessageEventManager {
    CMessageEvent* m_result; long m_type; unsigned __int64 m_uid;
    CMessageEvent* FindMostRecentMessage(const CKilledMeFilter& filter) { m_type = filter.m_pFirst->Type; m_uid = filter.m_pSecond->UID; return m_result; }
};

struct CGameScriptThingManager {
    CScriptThing GetNullScriptThing() { CScriptThing t; t.PImp_Data = (void*)this; return t; }
};
struct CQuestManager {
    void*                    _vftable;
    CGameScriptThingManager* PGameScriptThingManager;   // +0x04 CCountedPointer<CGameScriptThingManager>::Data
    CGameScriptThingManager* GetGameScriptThingManager() const { return PGameScriptThingManager; }
};
CQuestManager* g_pQuestManager;

struct CGameScriptThingMethods : CGameScriptThing {
    CScriptThing MsgWhoKilledMe() const;
};

CScriptThing CGameScriptThingMethods::MsgWhoKilledMe() const
{
    CMessageEventManager* messages = World->PMessageEventManager_Data;
    CMessageEvent* message = messages->FindMostRecentMessage(
        CMessageEventFilter_IsOfType(9) && CMessageEventFilter_IsTargettedAtThing(ThingUID));
    if (message) {
        if (message->POriginator.GetPItem() &&
            IsThingOfType<CThingRetail>((CThingRetail*)message->POriginator.GetPItem())) {
            CThingRetail* killer = (CThingRetail*)message->POriginator.GetPItem();
            if (killer && !killer->Dead)
                return killer->GetScriptThing();
        }
    }
    return g_pQuestManager->GetGameScriptThingManager()->GetNullScriptThing();
}

int main()
{
    static CWorld world; static CMessageEventManager manager; static CMessageEvent message; static CThingRetail killer;
    static CGameScriptThingManager things; static CQuestManager quests;
    memset(&killer, 0, sizeof(killer)); memset(&message, 0, sizeof(message));
    quests.PGameScriptThingManager = &things; g_pQuestManager = &quests;
    world.PMessageEventManager_Data = &manager;
    static CGameScriptThingMethods gst; memset(&gst, 0, sizeof(gst));
    gst.World = &world; gst.ThingUID = 0x0000000500000007ULL;
    int bad = 0;
    manager.m_result = 0;
    if (gst.MsgWhoKilledMe().PImp_Data != (void*)&things || manager.m_type != 9 || manager.m_uid != 0x0000000500000007ULL) bad = 1;
    manager.m_result = &message; message.POriginator.Info = 0;
    if (gst.MsgWhoKilledMe().PImp_Data != (void*)&things) bad = 2;
    message.POriginator.Info = &killer; killer.Dead = 1;
    if (gst.MsgWhoKilledMe().PImp_Data != (void*)&things) bad = 3;
    killer.Dead = 0;
    if (gst.MsgWhoKilledMe().PImp_Data != (void*)&killer) bad = 4;
    if (bad) { std::printf("008d0610_TEST FAIL\n"); return 1; }
    std::printf("008d0610_TEST PASS\n");
    return 0;
}
