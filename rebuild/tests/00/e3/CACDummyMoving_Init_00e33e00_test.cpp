// Behaviour test for 0x00E33E00 CAC_DummyMoving::Init
#include <cstdio>
#include <cstring>
static int g_thingsAlive = 0;
class CCharString { public: char t[32]; CCharString(const char* s) { std::strcpy(t, s); } };
class CScriptThing { public:
    int id;
    CScriptThing(int i) : id(i) { ++g_thingsAlive; }
    CScriptThing(const CScriptThing& o) : id(o.id) { ++g_thingsAlive; }
    ~CScriptThing() { --g_thingsAlive; }
};
class CGameScriptInterface { public:
    const CScriptThing* facer; int target; char lookedUp[32];
    virtual CScriptThing GetThingWithScriptName(const CCharString& name, long) const
    { std::strcpy(const_cast<CGameScriptInterface*>(this)->lookedUp, name.t); return CScriptThing(std::strcmp(name.t, "AC_PlayerRing") == 0 ? 42 : -1); }
    virtual void EntitySetFacingAngleTowardsThing(const CScriptThing& entity, const CScriptThing& target) const
    { CGameScriptInterface* self = const_cast<CGameScriptInterface*>(this); self->facer = &entity; self->target = target.id; }
    virtual ~CGameScriptInterface() {}
};
struct CAC_DummyMoving {
    CGameScriptInterface* m_pGame; CScriptThing m_thing; long m_1c; long m_20;
    CAC_DummyMoving() : m_thing(7) {}
    void Init() { m_1c = 0; m_20 = 0; CCharString name("AC_PlayerRing"); m_pGame->EntitySetFacingAngleTowardsThing(m_thing, m_pGame->GetThingWithScriptName(name, 0)); }
};
int main()
{
    CGameScriptInterface game; game.facer = 0; game.target = 0;
    CAC_DummyMoving o; o.m_pGame = &game; o.m_1c = 5; o.m_20 = 6;
    o.Init();
    bool ok = o.m_1c == 0 && o.m_20 == 0 && game.facer == &o.m_thing && game.target == 42 && std::strcmp(game.lookedUp, "AC_PlayerRing") == 0 && g_thingsAlive == 1;
    if (!ok) { std::printf("00e33e00_TEST FAIL\n"); return 1; }
    std::printf("00e33e00_TEST PASS\n");
    return 0;
}