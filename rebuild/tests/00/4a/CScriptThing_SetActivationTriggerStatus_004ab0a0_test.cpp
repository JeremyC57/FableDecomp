#include <stdio.h>
static int g_calls; static const void* g_this; static int g_arg = -1;
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info;
    T* operator->() const { return Data; } bool IsValid() const { return Data != 0; } };
class CScriptThing {
public:
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptThing() {}
    virtual void SetActivationTriggerStatus(bool status);
};
class CScriptThingImp : public CScriptThing {
public:
    virtual void SetActivationTriggerStatus(bool status) { ++g_calls; g_this = this; g_arg = status; }
};

void CScriptThing::SetActivationTriggerStatus(bool status)
{
    if (PImp.IsValid())
        PImp->SetActivationTriggerStatus(status);
}

int main()
{
    CScriptThing handle; CScriptThingImp imp;
    handle.SetActivationTriggerStatus(true);
    bool ok = g_calls == 0;
    handle.PImp.Data = &imp;
    handle.SetActivationTriggerStatus(true);
    ok = ok && g_calls == 1 && g_this == &imp && g_arg == 1;
    handle.SetActivationTriggerStatus(false); ok = ok && g_calls == 2 && g_arg == 0;
    printf(ok ? "004ab0a0_TEST PASS\n" : "004ab0a0_TEST FAIL\n");
    return ok ? 0 : 1;
}