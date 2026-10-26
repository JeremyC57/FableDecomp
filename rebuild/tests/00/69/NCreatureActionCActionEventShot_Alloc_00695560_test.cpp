#include <cstdio>
// Behaviour test for NCreatureAction::CActionEventShot::Alloc (retail 0x00695560): the clone factory builds a fresh event with
// the given activation frame, thing and camera manager, Finished cleared.
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
struct CActionEventShot : CActionEventBase {
    CActionEventShot(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera) { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
CActionEventBase* CActionEventShot::Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventShot(activationFrame, thing, camera);
}

int main()
{
    CThingCreatureBase thing = { 1 };
    CThingCreatureBase thing2 = { 2 };
    CGameCameraManager camera = { 3 };
    CGameCameraManager camera2 = { 4 };
    CActionEventShot proto(5, thing, camera);
    proto.Finished = true;
    CActionEventBase* made = proto.Alloc(42, thing2, camera2);
    CActionEventShot* e = (CActionEventShot*)made;
    if (made == 0 || made == &proto || e->baseBuilt != 1 || e->Finished || e->ActivationFrame != 42
        || e->PThing != &thing2 || e->PCameraManager != &camera2) {
        std::printf("00695560_TEST FAIL\n");
        return 1;
    }
    delete made;
    std::printf("00695560_TEST PASS\n");
    return 0;
}