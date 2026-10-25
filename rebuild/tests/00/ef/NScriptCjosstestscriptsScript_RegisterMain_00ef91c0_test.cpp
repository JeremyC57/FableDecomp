// behaviour test for 0x00EF91C0 NScript::Cjoss_test_scriptsScript::RegisterMain(): the spawned thread carries the name "Main", id 0, this, the
// Main member, region "", and every CCharString temporary is destroyed
#include <cstdio>
#include <cstring>
static int gLiveStrings = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { (void)length; ++gLiveStrings; }
    ~CCharString() { --gLiveStrings; }
};
class CSpawnedFuncBase {
public:
    virtual ~CSpawnedFuncBase() {}
    virtual void SuspendableProcess() = 0;
    char Name[64];
    unsigned long Id;
    CSpawnedFuncBase(const CCharString& name, unsigned long id) : Id(id) { std::strncpy(Name, name.PStringData, 63); Name[63] = 0; }
};
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {
public:
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) {}
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CScriptBase {
public:
    virtual ~CScriptBase() {}
protected:
    virtual void RegisterMain() {}
    virtual void Main() {}
public:
    CSpawnedFuncBase* LastFunc;
    char LastRegion[64];
    int Calls;
    CScriptBase() : LastFunc(0), Calls(0) { LastRegion[0] = 0; }
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName)
    {
        LastFunc = func; ++Calls;
        std::strncpy(LastRegion, regionName.PStringData, 63); LastRegion[63] = 0;
    }
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
class Cjoss_test_scriptsScript : public CScriptBase {
public:
    int Ran;
    Cjoss_test_scriptsScript() : Ran(0) {}
    virtual void RegisterMain();
    virtual void Main();
};
void Cjoss_test_scriptsScript::RegisterMain()
{
    SpawnFunction(new CSpawnedFunc<Cjoss_test_scriptsScript>("Main", 0, this, &Cjoss_test_scriptsScript::Main));
}
void Cjoss_test_scriptsScript::Main() { Ran = 1; }
int main()
{
    Cjoss_test_scriptsScript s;
    s.RegisterMain();
    CSpawnedFunc<Cjoss_test_scriptsScript>* f = (CSpawnedFunc<Cjoss_test_scriptsScript>*)s.LastFunc;
    if (s.Calls != 1 || f == 0 || std::strcmp(f->Name, "Main") != 0 || f->Id != 0 || f->PClass != &s
        || s.LastRegion[0] != 0 || gLiveStrings != 0) { std::printf("00ef91c0_TEST FAIL\n"); return 1; }
    f->SuspendableProcess();
    if (s.Ran != 1) { std::printf("00ef91c0_TEST FAIL\n"); return 1; }
    delete f;
    std::printf("00ef91c0_TEST PASS\n");
    return 0;
}