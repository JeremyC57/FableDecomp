#include <stdio.h>
static int g_calls; static const void* g_this; static int g_arg = -1;
struct CCPPointerInfo;
template <class T> struct CCountedPointer { T* Data; CCPPointerInfo* Info;
    T* operator->() const { return Data; } bool IsValid() const { return Data != 0; } };
class CScriptThing {
public:
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    virtual ~CScriptThing() {}
    virtual void IncrementScriptCounter();
};
class CScriptThingImp : public CScriptThing {
public:
    virtual void IncrementScriptCounter() { ++g_calls; g_this = this;  }
};

void CScriptThing::IncrementScriptCounter()
{
    if (PImp.IsValid())
        PImp->IncrementScriptCounter();
}

int main()
{
    CScriptThing handle; CScriptThingImp imp;
    handle.IncrementScriptCounter();
    bool ok = g_calls == 0;
    handle.PImp.Data = &imp;
    handle.IncrementScriptCounter();
    ok = ok && g_calls == 1 && g_this == &imp;
    
    printf(ok ? "004ab0f0_TEST PASS\n" : "004ab0f0_TEST FAIL\n");
    return ok ? 0 : 1;
}