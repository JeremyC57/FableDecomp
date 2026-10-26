#include <cstdio>
#include <cstring>
static int g_step = 0; static int g_anim = 0, g_get = 0, g_base = 0;
class CThingCreatureBase { public: int id; };
class CCharString { public: char* m_pData; CCharString(const char* text, long length = -1) : m_pData((char*)text) { (void)length; } ~CCharString() { m_pData = 0; } };
struct CBaseIntelligentPointer { virtual void v00() {} CThingCreatureBase* m_pItem; CThingCreatureBase* GetPItem() const { g_get = ++g_step; return m_pItem; } };
class CCombatActionBase { public: virtual void Initialise() { g_base = ++g_step; } unsigned char _pad04[4]; CBaseIntelligentPointer PCreature;
    void SetAnimationName(const CCharString& name) { if (std::strcmp(name.m_pData, "ST_JUMP") == 0) g_anim = ++g_step; } };
class CCreatureActionHeroJump : public CCombatActionBase { public: virtual void Initialise(); };
void CCreatureActionHeroJump::Initialise() { { CCharString name("ST_JUMP"); SetAnimationName(name); } PCreature.GetPItem(); CCombatActionBase::Initialise(); }
int main()
{
    CCreatureActionHeroJump a; CThingCreatureBase c; a.PCreature.m_pItem = &c; a.Initialise();
    if (g_anim != 1 || g_get != 2 || g_base != 3) { std::printf("0085ba90_TEST FAIL\n"); return 1; }
    std::printf("0085ba90_TEST PASS\n");
    return 0;
}