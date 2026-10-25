// behaviour test for 0x00E267E0 NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript(CScriptDataBase*, CGameScriptInterfaceBase&): the base is
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
class CQS_GuardianSisterInfo2_SisterInBanditCampScript : public CLevelScriptBase {
public:
    CQS_GuardianSisterInfo2_SisterInBanditCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual void RegisterMain() {}
    virtual void Main() {}
};
CQS_GuardianSisterInfo2_SisterInBanditCampScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase(data, game)
{
}
int main()
{
    CScriptDataBase data;
    CGameScriptInterfaceBase game;
    CQS_GuardianSisterInfo2_SisterInBanditCampScript s(&data, game);
    if (gBaseCtors != 1 || s.Game != &game || s.MasterScriptData != &data) { std::printf("00e267e0_TEST FAIL\n"); return 1; }
    std::printf("00e267e0_TEST PASS\n");
    return 0;
}