#include <cstdio>
class CScriptThing { public: void* Data; void* Info; int id; CScriptThing() : Data(0), Info(0), id(0) { } CScriptThing(const CScriptThing& o) : Data(o.Data), Info(o.Info), id(o.id) { } };
class CScriptGameResourceObjectScriptedThingBase { public: void* Data; void* Info; CScriptGameResourceObjectScriptedThingBase() : Data(0), Info(0) { } };
struct CTimerManager { long m_next; long NewTimer() { return m_next++; } };
CTimerManager g_mgr; CTimerManager* g_pScriptTimerManager = &g_mgr;
class CTimer { public: long TimerIndex; CTimer() : TimerIndex(g_pScriptTimerManager->NewTimer()) { } };
struct CScriptOwner { void* m_pInterface; };
class CEntityScriptBase { public:
    void* Game; CScriptThing Me;
    CEntityScriptBase(const CScriptThing& thing, void* game) : Game(game), Me(thing) { }
};
struct CBriarRose : CEntityScriptBase {
    CScriptOwner* ParentClass; void* PMasterData; CScriptGameResourceObjectScriptedThingBase seh_me; CScriptThing MyDestination; CScriptThing Target; long NumHitsBetweenForcePushes; CTimer SpeechTimer;
    CBriarRose(CScriptOwner* parent, void* masterData, const CScriptThing& me)
        : CEntityScriptBase(me, parent->m_pInterface), ParentClass(parent), seh_me(), MyDestination(), Target(), SpeechTimer()
    {
        PMasterData = masterData;
    }
};
int main()
{
    g_mgr.m_next = 100;
    int iface = 0, data = 0; CScriptOwner owner; owner.m_pInterface = &iface;
    CScriptThing me; me.id = 9;
    CBriarRose o(&owner, &data, me);
    if (o.Game != &iface || o.Me.id != 9 || o.ParentClass != &owner || o.PMasterData != &data || o.SpeechTimer.TimerIndex != 100 || o.seh_me.Data != 0 || o.MyDestination.Data != 0 || o.Target.Info != 0 || g_mgr.m_next != 101)
    { std::printf("00d9e930_TEST FAIL\n"); return 1; }
    std::printf("00d9e930_TEST PASS\n");
    return 0;
}