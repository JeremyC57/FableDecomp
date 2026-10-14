#include <cstdio>
#include <cstring>
struct CBaseObject { int id; };
struct CBaseIntelligentPointer { virtual void v00() {} CBaseObject* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } CBaseObject* GetPItem() const { return m_pItem; } void SetPItem(CBaseObject* item) { m_pItem = item; } };
struct CIntelligentPointer : CBaseIntelligentPointer { virtual void v00() {} CIntelligentPointer() : CBaseIntelligentPointer() { } CIntelligentPointer(CBaseObject* item) : CBaseIntelligentPointer() { SetPItem(item); } CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); } };
static long g_code;
struct CCombatActionBase { virtual void v00() {} unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode) { g_code = actionCode; std::memset(_body, 0, sizeof(_body)); } };

struct CCreatureAction_SummonerFlameSlice : CCombatActionBase { CIntelligentPointer PTarget; virtual void v00() {} CCreatureAction_SummonerFlameSlice(); };
CCreatureAction_SummonerFlameSlice::CCreatureAction_SummonerFlameSlice() : CCombatActionBase(150), PTarget() { }
int main()
{
    CCreatureAction_SummonerFlameSlice a;
    if (g_code != 150 || a.PTarget.GetPItem() != 0 || (char*)&a.PTarget - (char*)&a != 0x114) { std::printf("006f5b50_TEST FAIL\n"); return 1; }
    std::printf("006f5b50_TEST PASS\n");
    return 0;
}