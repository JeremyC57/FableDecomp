#include <cstdio>
#include <cstring>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { virtual void v00() {} CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };
struct CSoundPair { long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

struct CThingCreatureBase { int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const void* g_copied;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { g_copied = &o; std::memcpy(_body, o._body, sizeof(_body)); } };
#pragma pack(push, 1)
struct CCreatureAction_BriarRoseConcentrateInto : CCombatActionBase { unsigned char Imposter; CIntelligentPointer PTarget; virtual void v00() {}
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter) : CCombatActionBase(owner, 50, CSoundPair()), Imposter(imposter), PTarget(target) { }
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other) : CCombatActionBase(other), Imposter(other.Imposter), PTarget(other.PTarget) { } };
#pragma pack(pop)

int main()
{
    CThingCreatureBase owner; CBaseObject t; CCreatureAction_BriarRoseConcentrateInto a(&owner, &t, true);
    if (g_owner != &owner || g_code != 50 || a.Imposter != 1 || a.PTarget.GetPItem() != &t || (char*)&a.PTarget - (char*)&a != 0x115 || sizeof(a) != 0x11d) { std::printf("008e8bb0_TEST FAIL\n"); return 1; }
    std::printf("008e8bb0_TEST PASS\n");
    return 0;
}