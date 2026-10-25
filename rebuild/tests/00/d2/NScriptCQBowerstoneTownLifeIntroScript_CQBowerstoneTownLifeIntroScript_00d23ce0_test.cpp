// behaviour test for 0x00D23CE0 NScript::CQ_BowerstoneTownLifeIntroScript::CQ_BowerstoneTownLifeIntroScript(CScriptDataBase*, CGameScriptInterfaceBase&): the base is
// constructed, Game points at the interface and MasterScriptData at the data
#include <cstdio>
static int gBaseCtors = 0;
class CScriptDataBase { public: virtual ~CScriptDataBase() {} };
class CGameScriptInterfaceBase { public: int Dummy; };
class CScriptBase {
public:
    CScriptBase() { ++gBaseCtors; }
    virtual ~CScriptBase() {}
protected:
    virtual void RegisterMain() {}
    virtual void Main() {}
};
class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    CScriptDataBase* MasterScriptData;
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game) : Game(&game), MasterScriptData(data) {}
};
class CQ_BowerstoneTownLifeIntroScript : public CLevelScriptBase {
public:
    CQ_BowerstoneTownLifeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain() {}
    virtual void Main() {}
};
CQ_BowerstoneTownLifeIntroScript::CQ_BowerstoneTownLifeIntroScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    CQ_BowerstoneTownLifeIntroScript s(&data, game);
    if (gBaseCtors != 1 || s.Game != &game || s.MasterScriptData != &data) { std::printf("00d23ce0_TEST FAIL\n"); return 1; }
    std::printf("00d23ce0_TEST PASS\n");
    return 0;
}