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
    virtual void SetFriendsWithEverythingFlag(bool flag);
};
class CScriptThingImp : public CScriptThing {
public:
    virtual void SetFriendsWithEverythingFlag(bool flag) { ++g_calls; g_this = this; g_arg = flag; }
};

void CScriptThing::SetFriendsWithEverythingFlag(bool flag)
{
    if (PImp.IsValid())
        PImp->SetFriendsWithEverythingFlag(flag);
}

int main()
{
    CScriptThing handle; CScriptThingImp imp;
    handle.SetFriendsWithEverythingFlag(true);
    bool ok = g_calls == 0;
    handle.PImp.Data = &imp;
    handle.SetFriendsWithEverythingFlag(true);
    ok = ok && g_calls == 1 && g_this == &imp && g_arg == 1;
    handle.SetFriendsWithEverythingFlag(false); ok = ok && g_calls == 2 && g_arg == 0;
    printf(ok ? "004ab060_TEST PASS\n" : "004ab060_TEST FAIL\n");
    return ok ? 0 : 1;
}