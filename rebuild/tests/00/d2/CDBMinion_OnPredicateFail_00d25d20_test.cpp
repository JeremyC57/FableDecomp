#include <cstdio>
struct CMinionRecord { unsigned char _pad[0x14]; long m_timer; };
static CMinionRecord g_record; static long g_lookedUp = 0, g_resetThing = 0; static long* g_resetSlot = 0;
struct CThingManager_Calls {
    virtual void ResetMinionTimer(long thingId, long* slot) { g_resetThing = thingId; g_resetSlot = slot; *slot = 0; }
    virtual CMinionRecord* GetMinionRecord(long thingId) { g_lookedUp = thingId; return &g_record; }
};
static CThingManager_Calls g_manager; static CThingManager_Calls* g_pThingManager_0143e8f8 = &g_manager;
class CScriptThing { public: long m_thingId; const long& GetThingId() const { return m_thingId; } };
struct CQ_DragonBossFightScript { unsigned char _pad[0x70]; long m_minionCount; unsigned char _pad2[0x90 - 0x70 - 4]; CScriptThing m_minion; };
struct CDBMinion { unsigned char _pad[0x14]; CQ_DragonBossFightScript* ParentClass;
    void OnPredicateFail() {
        --ParentClass->m_minionCount;
        const CScriptThing& minion = ParentClass->m_minion;
        CMinionRecord* record = g_pThingManager_0143e8f8->GetMinionRecord(minion.GetThingId());
        g_pThingManager_0143e8f8->ResetMinionTimer(ParentClass->m_minion.m_thingId, &record->m_timer); } };
int main()
{
    CQ_DragonBossFightScript script; script.m_minionCount = 3; script.m_minion.m_thingId = 0x1234; g_record.m_timer = 99;
    CDBMinion m; m.ParentClass = &script; m.OnPredicateFail();
    if (script.m_minionCount != 2 || g_lookedUp != 0x1234 || g_resetThing != 0x1234 || g_resetSlot != &g_record.m_timer || g_record.m_timer != 0) { std::printf("00d25d20_TEST FAIL\n"); return 1; }
    std::printf("00d25d20_TEST PASS\n"); return 0;
}