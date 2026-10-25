// behaviour test for 0x00EEFFA0 NScript::CPersonalScript_EmptyScript::CPersonalScript_EmptyScript(CScriptDataBase*, CGameScriptInterfaceBase&): the base is
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
class CPersonalScript_EmptyScript : public CLevelScriptBase {
public:
    CPersonalScript_EmptyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain() {}
    virtual void Main() {}
};
CPersonalScript_EmptyScript::CPersonalScript_EmptyScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    CPersonalScript_EmptyScript s(&data, game);
    if (gBaseCtors != 1 || s.Game != &game || s.MasterScriptData != &data) { std::printf("00eeffa0_TEST FAIL\n"); return 1; }
    std::printf("00eeffa0_TEST PASS\n");
    return 0;
}