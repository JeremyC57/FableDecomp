// Behaviour test for NScript::CGameflowAssistanceScript::Main (0x00CEFAC0): four spawned functions registered in order,
// each named after its tutorial, with no start delay, bound to the script's own member and an empty tag.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
class CCharString { public:
    CCharString(const char* text, long length = -1) : PStringData(strdup(text)) { (void)length; }
    CCharString(const CCharString& o) : PStringData(strdup(o.PStringData)) {}
    ~CCharString() { free(PStringData); }
    char* PStringData;
};
class CSpawnedFuncBase { public: CCharString name; long delay; CSpawnedFuncBase(const char* n, long d) : name(n), delay(d) {} virtual ~CSpawnedFuncBase() {} virtual void Run() = 0; };
template<class T> struct CSpawnedFunc : CSpawnedFuncBase {
    T* owner; void (T::*func)();
    CSpawnedFunc(const char* n, long d, T* o, void (T::*f)()) : CSpawnedFuncBase(n, d), owner(o), func(f) {}
    void Run() { (owner->*func)(); }
};
class CScriptBase { public:
    CSpawnedFuncBase* funcs[8]; char* tags[8]; int count;
    CScriptBase() : count(0) {}
    ~CScriptBase() { for (int i = 0; i < count; ++i) { delete funcs[i]; free(tags[i]); } }
    void AddSpawnedFunction(CSpawnedFuncBase* f, const CCharString& tag) { funcs[count] = f; tags[count] = strdup(tag.PStringData); ++count; }
};
namespace NScript {
class CGameflowAssistanceScript : public CScriptBase {
public:
    int ran[4];
    CGameflowAssistanceScript() { ran[0] = ran[1] = ran[2] = ran[3] = 0; }
    virtual void Main();
    void Tutorial_CombatMultiplier() { ran[0]++; }
    void Tutorial_LowHealth() { ran[1]++; }
    void Tutorial_LowMagic() { ran[2]++; }
    void Tutorial_RenownLevelIncrease() { ran[3]++; }
};
void CGameflowAssistanceScript::Main()
{
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_CombatMultiplier", 0, this, &CGameflowAssistanceScript::Tutorial_CombatMultiplier), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_LowHealth", 0, this, &CGameflowAssistanceScript::Tutorial_LowHealth), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_LowMagic", 0, this, &CGameflowAssistanceScript::Tutorial_LowMagic), CCharString(""));
    AddSpawnedFunction(new CSpawnedFunc<CGameflowAssistanceScript>("Tutorial_RenownLevelIncrease", 0, this, &CGameflowAssistanceScript::Tutorial_RenownLevelIncrease), CCharString(""));
}
}
int main()
{
    static const char* names[4] = { "Tutorial_CombatMultiplier", "Tutorial_LowHealth", "Tutorial_LowMagic", "Tutorial_RenownLevelIncrease" };
    NScript::CGameflowAssistanceScript s;
    s.Main();
    if (s.count != 4) { printf("00cefac0_TEST FAIL count=%d\n", s.count); return 1; }
    for (int i = 0; i < 4; ++i) {
        if (strcmp(s.funcs[i]->name.PStringData, names[i]) != 0 || s.funcs[i]->delay != 0 || s.tags[i][0] != 0) { printf("00cefac0_TEST FAIL [%d]\n", i); return 1; }
        s.funcs[i]->Run();
        if (s.ran[i] != 1) { printf("00cefac0_TEST FAIL run [%d]\n", i); return 1; }
    }
    printf("00cefac0_TEST PASS\n");
    return 0;
}