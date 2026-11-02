// Test: NCameraModes::CScriptedControl::Update() forwards to the script module's Update (slot 1 of the module).
#include <cstdio>
static int g_updates = 0;
namespace NCameraModes {
class CScriptModuleBase { public:
    virtual ~CScriptModuleBase() { }
    virtual void Update() = 0;
};
class CTestModule : public CScriptModuleBase { public:
    virtual void Update() { ++g_updates; }
};
template <class T> class CCountedPointer { public:
    T* Data; void* Info;
    T* operator->() const { return Data; }
};
class CBase { public:
    virtual ~CBase() { }
    virtual void Update() { }
};
class CScriptedControl : public CBase { public:
    CCountedPointer<CScriptModuleBase> PScriptModule;
    virtual void Update();
};
void CScriptedControl::Update()
{
    PScriptModule->Update();
}
}
int main()
{
    NCameraModes::CTestModule module;
    NCameraModes::CScriptedControl control;
    control.PScriptModule.Data = &module;
    control.PScriptModule.Info = 0;
    NCameraModes::CBase* base = &control;
    base->Update();
    base->Update();
    if (g_updates != 2) { std::printf("0087bbc0_TEST FAIL\n"); return 1; }
    std::printf("0087bbc0_TEST PASS\n");
    return 0;
}