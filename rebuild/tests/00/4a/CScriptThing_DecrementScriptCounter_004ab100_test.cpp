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
    virtual void DecrementScriptCounter();
};
class CScriptThingImp : public CScriptThing {
public:
    virtual void DecrementScriptCounter() { ++g_calls; g_this = this;  }
};

void CScriptThing::DecrementScriptCounter()
{
    if (PImp.IsValid())
        PImp->DecrementScriptCounter();
}

int main()
{
    CScriptThing handle; CScriptThingImp imp;
    handle.DecrementScriptCounter();
    bool ok = g_calls == 0;
    handle.PImp.Data = &imp;
    handle.DecrementScriptCounter();
    ok = ok && g_calls == 1 && g_this == &imp;
    
    printf(ok ? "004ab100_TEST PASS\n" : "004ab100_TEST FAIL\n");
    return ok ? 0 : 1;
}