#include <cstdio>
#include <cstring>
class CBaseObject { public: int id; };
struct CBaseIntelligentPointer { virtual void v00() {} CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };
class CSoundPair { public: long FirstSound; long LastSound; CSoundPair() : FirstSound(0), LastSound(0) { } };

class CThingCreatureBase { public: int id; };
static CThingCreatureBase* g_owner; static long g_code; static long g_s0, g_s1; static const void* g_copied;
class CCombatActionBase { public: virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound) { g_owner = owner; g_code = actionCode; g_s0 = sound.FirstSound; g_s1 = sound.LastSound; std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { g_copied = &o; std::memcpy(_body, o._body, sizeof(_body)); } };
#pragma pack(push, 1)
class CCreatureAction_BriarRoseConcentrateInto : public CCombatActionBase { public: unsigned char Imposter; CIntelligentPointer PTarget; virtual void v00() {}
    CCreatureAction_BriarRoseConcentrateInto(CThingCreatureBase* owner, CBaseObject* target, bool imposter) : CCombatActionBase(owner, 50, CSoundPair()), Imposter(imposter), PTarget(target) { }
    CCreatureAction_BriarRoseConcentrateInto(const CCreatureAction_BriarRoseConcentrateInto& other) : CCombatActionBase(other), Imposter(other.Imposter), PTarget(other.PTarget) { } };
#pragma pack(pop)

int main()
{
    CThingCreatureBase owner; CBaseObject t; CCreatureAction_BriarRoseConcentrateInto a(&owner, &t, true); a._body[5] = 0x42;
    CCreatureAction_BriarRoseConcentrateInto b(a);
    if (g_copied != &a || b.Imposter != 1 || b.PTarget.GetPItem() != &t || b._body[5] != 0x42 || (char*)&b.PTarget - (char*)&b != 0x115) { std::printf("008e8d60_TEST FAIL\n"); return 1; }
    std::printf("008e8d60_TEST PASS\n");
    return 0;
}