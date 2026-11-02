// Test: NCreatureAction::CActionEventRemoveObject::CActionEventRemoveObject() clears the base's Finished flag, default-constructs the Hand string once and installs this class's vtable.
#include <cstdio>
static int g_strings = 0;
class CCharString { public:
    void* PStringData;
    CCharString() : PStringData(0) { ++g_strings; }
};
class CBaseClass { public:
    CBaseClass() { }
    virtual ~CBaseClass() { }
    virtual int Kind() const { return 0; }
};
namespace NCreatureAction {
class CActionEventBase : public CBaseClass { public:
    bool  Finished;
    long  ActivationFrame;
    void* PThing;
    void* PCameraManager;
    CActionEventBase() : Finished(false) { }
};
class CActionEventRemoveObject : public CActionEventBase { public:
    CCharString Hand;
    CActionEventRemoveObject();
    virtual int Kind() const { return 1; }
};
CActionEventRemoveObject::CActionEventRemoveObject()
{
}
}
int main()
{
    NCreatureAction::CActionEventRemoveObject event;
    CBaseClass* base = &event;
    if (event.Finished != false || event.Hand.PStringData != 0 || g_strings != 1 || base->Kind() != 1) { std::printf("006922c0_TEST FAIL\n"); return 1; }
    std::printf("006922c0_TEST PASS\n");
    return 0;
}