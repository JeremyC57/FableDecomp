#include <cstdio>
#include <cstring>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { virtual void v00() {} CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };

static const void* g_copied;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase() { std::memset(_body, 0, sizeof(_body)); }
    CCombatActionBase(const CCombatActionBase& o) { g_copied = &o; std::memcpy(_body, o._body, sizeof(_body)); } };
struct CCreatureAction_ScorpionChargeInto : CCombatActionBase { float ChargeDistance; CIntelligentPointer PTarget; virtual void v00() {}
    CCreatureAction_ScorpionChargeInto() : ChargeDistance(0), PTarget() { }
    CCreatureAction_ScorpionChargeInto(const CCreatureAction_ScorpionChargeInto& other) : CCombatActionBase(other), ChargeDistance(other.ChargeDistance), PTarget(other.PTarget) { } };
int main()
{
    CBaseObject t; CCreatureAction_ScorpionChargeInto a; a.ChargeDistance = 2.5f; a.PTarget.SetPItem(&t); a._body[7] = 9;
    CCreatureAction_ScorpionChargeInto b(a);
    if (g_copied != &a || b.ChargeDistance != 2.5f || b.PTarget.GetPItem() != &t || b._body[7] != 9 || (char*)&b.PTarget - (char*)&b != 0x118 || sizeof(b) != 0x120) { std::printf("008c5d40_TEST FAIL\n"); return 1; }
    std::printf("008c5d40_TEST PASS\n");
    return 0;
}