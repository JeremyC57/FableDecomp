// behaviour test for 0x00E6E2B0 NScript::CV_DemonDoorsScript::CV_DemonDoorsScript(CScriptDataBase*, CGameScriptInterfaceBase&): the base is
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
class CV_DemonDoorsScript : public CLevelScriptBase {
public:
    CV_DemonDoorsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain() {}
    virtual void Main() {}
};
CV_DemonDoorsScript::CV_DemonDoorsScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    CV_DemonDoorsScript s(&data, game);
    if (gBaseCtors != 1 || s.Game != &game || s.MasterScriptData != &data) { std::printf("00e6e2b0_TEST FAIL\n"); return 1; }
    std::printf("00e6e2b0_TEST PASS\n");
    return 0;
}