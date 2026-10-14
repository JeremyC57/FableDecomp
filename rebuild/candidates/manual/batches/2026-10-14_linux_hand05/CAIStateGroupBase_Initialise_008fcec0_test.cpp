#include <cstdio>
struct CCharString { void* m_pData; CCharString& operator=(const CCharString& o) { m_pData = o.m_pData; return *this; } };
struct CThingAICreature; struct CThingAIManager;
struct CThingManager { unsigned char _pad[0x24]; CThingAIManager* m_pAIManager; };
struct CAIBrain { void* __vftable; unsigned char _pad[0x18]; CThingManager* ThingManager; };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE = 0, SCRIPT_AI_PRIORITY_HIGH = 3 };
static CThingAIManager* g_initManager = 0; static CThingAICreature* g_initCreature = 0;
struct CAIStateGroupBase {
    CAIBrain* PBrain; EScriptAIPriority ScriptAIPriority; CCharString Name; bool Terminated; unsigned char Priority; bool Enabled;
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {} virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {} virtual void v12() {}
    virtual void InitStateGroup(CThingAIManager* manager, CThingAICreature& creature) { g_initManager = manager; g_initCreature = &creature; }
    void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
};
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    PBrain = &brain; Terminated = false; Priority = (unsigned char)priority; ScriptAIPriority = scriptPriority; Name = name; Enabled = true;
    InitStateGroup(brain.ThingManager->m_pAIManager, creature);
}
int main()
{
    int aiMgr, creatureRaw, nameData; CThingManager tm; tm.m_pAIManager = (CThingAIManager*)&aiMgr; CAIBrain brain; brain.ThingManager = &tm;
    CCharString name; name.m_pData = &nameData; CAIStateGroupBase g; g.Terminated = true; g.Enabled = false;
    g.Initialise(brain, *(CThingAICreature*)&creatureRaw, 0x155, SCRIPT_AI_PRIORITY_HIGH, name);
    if (g.PBrain != &brain || g.ScriptAIPriority != SCRIPT_AI_PRIORITY_HIGH || g.Name.m_pData != &nameData || g.Terminated || g.Priority != 0x55 || !g.Enabled
        || g_initManager != (CThingAIManager*)&aiMgr || g_initCreature != (CThingAICreature*)&creatureRaw) { std::printf("008fcec0_TEST FAIL\n"); return 1; }
    std::printf("008fcec0_TEST PASS\n");
    return 0;
}
