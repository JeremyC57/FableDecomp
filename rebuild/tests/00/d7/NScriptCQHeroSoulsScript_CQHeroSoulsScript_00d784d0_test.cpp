// behaviour test for 0x00D784D0 NScript::CQ_HeroSoulsScript::CQ_HeroSoulsScript(CScriptDataBase*, CGameScriptInterfaceBase&): the base is
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
class CQ_HeroSoulsScript : public CLevelScriptBase {
public:
    CQ_HeroSoulsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain() {}
    virtual void Main() {}
};
CQ_HeroSoulsScript::CQ_HeroSoulsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    CQ_HeroSoulsScript s(&data, game);
    if (gBaseCtors != 1 || s.Game != &game || s.MasterScriptData != &data) { std::printf("00d784d0_TEST FAIL\n"); return 1; }
    std::printf("00d784d0_TEST PASS\n");
    return 0;
}