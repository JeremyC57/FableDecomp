#include <cstdio>
// Behaviour test for NCreatureAction::CActionEventCameraShake::Alloc (retail 0x00695480): the clone factory builds a fresh event with
// the given activation frame, thing and camera manager, Finished cleared, Global false.
class CThingCreatureBase { public: int id; };
class CGameCameraManager { public: int id; };
struct CBase {
    int baseBuilt;
    CBase() : baseBuilt(1) { }
    virtual ~CBase() { }
};
struct CActionEventBase : CBase {
    bool Finished;
    long ActivationFrame;
    CThingCreatureBase* PThing;
    CGameCameraManager* PCameraManager;
    CActionEventBase(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CBase(), Finished(false), ActivationFrame(activationFrame), PThing(&thing), PCameraManager(&camera) { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const = 0;
};
struct CActionEventCameraShake : CActionEventBase {
    long Strength;
    long Length;
    bool Global;
    CActionEventCameraShake(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera)
    {
        Global = false;
    }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
CActionEventBase* CActionEventCameraShake::Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventCameraShake(activationFrame, thing, camera);
}

int main()
{
    CThingCreatureBase thing = { 1 };
    CThingCreatureBase thing2 = { 2 };
    CGameCameraManager camera = { 3 };
    CGameCameraManager camera2 = { 4 };
    CActionEventCameraShake proto(5, thing, camera);
    proto.Finished = true;
    CActionEventBase* made = proto.Alloc(42, thing2, camera2);
    CActionEventCameraShake* e = (CActionEventCameraShake*)made;
    if (made == 0 || made == &proto || e->baseBuilt != 1 || e->Finished || e->ActivationFrame != 42
        || e->PThing != &thing2 || e->PCameraManager != &camera2 || e->Global) {
        std::printf("00695480_TEST FAIL\n");
        return 1;
    }
    delete made;
    std::printf("00695480_TEST PASS\n");
    return 0;
}