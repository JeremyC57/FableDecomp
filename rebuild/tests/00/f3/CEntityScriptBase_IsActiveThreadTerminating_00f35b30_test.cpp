// Behaviour test (0x00f35b30): CEntityScriptBase::IsActiveThreadTerminating asks the parent script (slot 3), which reports
// its active thread's terminating flag (false without an active thread)
#include <cstdio>
class CSpawnedFuncBase {
public:
    bool Terminating;
};
class CScriptBase {
public:
    CSpawnedFuncBase* PActiveThread;
    bool IsActiveThreadTerminating() { return PActiveThread ? PActiveThread->Terminating : false; }
};
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase() { }
    virtual void Main() = 0;
    virtual void Init() { }
    virtual CScriptBase* GetParentScript() const = 0;
    bool IsActiveThreadTerminating() const { return GetParentScript()->IsActiveThreadTerminating(); }
};
class CTestEntityScript : public CEntityScriptBase {
public:
    CScriptBase* ParentClass;
    int          ParentCalls;
    virtual void Main() { }
    virtual CScriptBase* GetParentScript() const { ++const_cast<CTestEntityScript*>(this)->ParentCalls; return ParentClass; }
};
int main()
{
    CSpawnedFuncBase thread; thread.Terminating = false;
    CScriptBase script; script.PActiveThread = 0;
    CTestEntityScript entity; entity.ParentClass = &script; entity.ParentCalls = 0;
    const CEntityScriptBase& e = entity;
    if (e.IsActiveThreadTerminating() || entity.ParentCalls != 1) { std::printf("00f35b30_TEST FAIL\n"); return 1; }
    script.PActiveThread = &thread;
    if (e.IsActiveThreadTerminating()) { std::printf("00f35b30_TEST FAIL\n"); return 1; }
    thread.Terminating = true;
    if (!e.IsActiveThreadTerminating() || entity.ParentCalls != 3) { std::printf("00f35b30_TEST FAIL\n"); return 1; }
    std::printf("00f35b30_TEST PASS\n");
    return 0;
}
