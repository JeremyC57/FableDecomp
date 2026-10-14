#include <cstdio>
struct CThing {};
struct CThingCreatureBase { int stops; void StopSpeaking() { ++stops; } };
static int g_live = 0;
struct CBaseIntelligentPointer { virtual void v00() {} void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { ++g_live; } CThing* GetPItem() const { return (CThing*)m_pItem; } void SetPItem(CThing* i) { m_pItem = i; } ~CBaseIntelligentPointer() { --g_live; } };
struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; } };
struct CAIBrain { unsigned char _pad[0x18]; CThingCreatureBase* Creature; };
struct CAIStateGroupBase { virtual ~CAIStateGroupBase() {} CAIBrain* PBrain; unsigned char _pad[0xc]; };
struct CAIStateGroup_MinionHawkingFromStall : CAIStateGroupBase { long m_hawkState; CIntelligentPointer PStall; CAIStateGroup_MinionHawkingFromStall() : PStall(0) {} virtual void OnReplace(); };
void CAIStateGroup_MinionHawkingFromStall::OnReplace() { PStall = 0; PBrain->Creature->StopSpeaking(); }
int main()
{
    CThing stall; CThingCreatureBase c = { 0 }; CAIBrain b; b.Creature = &c; CAIStateGroup_MinionHawkingFromStall g; g.PBrain = &b; g.PStall.SetPItem(&stall); int live = g_live;
    g.OnReplace();
    if (g.PStall.GetPItem() != 0 || c.stops != 1 || g_live != live) { std::printf("0094bad0_TEST FAIL\n"); return 1; }
    std::printf("0094bad0_TEST PASS\n"); return 0;
}