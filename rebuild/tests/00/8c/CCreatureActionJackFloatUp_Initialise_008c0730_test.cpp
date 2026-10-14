#include <cstdio>
#include <cstring>
static const char* g_anim; static int g_baseInit; static int g_order;
struct CCharString { const char* m_text; CCharString(const char* t, long) : m_text(t) { } ~CCharString() { } };
struct CCombatActionBase { virtual void v00() { } virtual void Initialise() { ++g_baseInit; g_order = g_order * 10 + 2; }
    void SetAnimationName(const CCharString& name) { g_anim = name.m_text; g_order = g_order * 10 + 1; }
    unsigned char _pad04[4]; struct CBaseIntelligentPointer* PCreature_; unsigned char _pad[0x114 - 0x0c];
};

struct CPosition { long X, Y, Z; };
struct CThingCreatureBase_Calls { CPosition m_pos; const CPosition& GetPosition() const { return m_pos; } };
struct CBaseIntelligentPointer { virtual void v00() { } CThingCreatureBase_Calls* m_pItem; CThingCreatureBase_Calls* GetPItem() const { return m_pItem; } };
struct CCreatureAction_JackFloatUp : CCombatActionBase { long FloatHeight; long FloatSpeed; unsigned char Moving; long StartHeight; CBaseIntelligentPointer PCreature; virtual void Initialise(); };
void CCreatureAction_JackFloatUp::Initialise()
{
    { CCharString name("RISE_TO_FLOAT", -1); SetAnimationName(name); }
    CCombatActionBase::Initialise();
    Moving = 0; FloatSpeed = 0; StartHeight = PCreature.GetPItem()->GetPosition().Z;
}
int main()
{
    CThingCreatureBase_Calls creature; creature.m_pos.X = 1; creature.m_pos.Y = 2; creature.m_pos.Z = 0x42c80000;
    CCreatureAction_JackFloatUp a; a.PCreature.m_pItem = &creature; a.Moving = 1; a.FloatSpeed = 5; a.StartHeight = 0; a.Initialise();
    if (g_baseInit != 1 || g_order != 12 || std::strcmp(g_anim, "RISE_TO_FLOAT") != 0 || a.Moving != 0 || a.FloatSpeed != 0 || a.StartHeight != 0x42c80000
        || (char*)&a.StartHeight - (char*)&a != 0x120) { std::printf("008c0730_TEST FAIL\n"); return 1; }
    std::printf("008c0730_TEST PASS\n");
    return 0;
}