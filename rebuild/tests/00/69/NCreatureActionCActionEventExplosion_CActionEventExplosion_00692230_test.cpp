// Test: NCreatureAction::CActionEventExplosion::CActionEventExplosion() clears the base's Finished flag, default-constructs the AttachmentObject string once and installs this class's vtable.
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
class CActionEventExplosion : public CActionEventBase { public:
    CCharString AttachmentObject; long ExplosionDefIndex; bool Dummy; bool UseActionHelper;
    CActionEventExplosion();
    virtual int Kind() const { return 1; }
};
CActionEventExplosion::CActionEventExplosion()
{
}
}
int main()
{
    NCreatureAction::CActionEventExplosion event;
    CBaseClass* base = &event;
    if (event.Finished != false || event.AttachmentObject.PStringData != 0 || g_strings != 1 || base->Kind() != 1) { std::printf("00692230_TEST FAIL\n"); return 1; }
    std::printf("00692230_TEST PASS\n");
    return 0;
}