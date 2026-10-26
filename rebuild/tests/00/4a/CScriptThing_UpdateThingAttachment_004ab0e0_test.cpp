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
    virtual void UpdateThingAttachment();
};
class CScriptThingImp : public CScriptThing {
public:
    virtual void UpdateThingAttachment() { ++g_calls; g_this = this;  }
};

void CScriptThing::UpdateThingAttachment()
{
    if (PImp.IsValid())
        PImp->UpdateThingAttachment();
}

int main()
{
    CScriptThing handle; CScriptThingImp imp;
    handle.UpdateThingAttachment();
    bool ok = g_calls == 0;
    handle.PImp.Data = &imp;
    handle.UpdateThingAttachment();
    ok = ok && g_calls == 1 && g_this == &imp;
    
    printf(ok ? "004ab0e0_TEST PASS\n" : "004ab0e0_TEST FAIL\n");
    return ok ? 0 : 1;
}