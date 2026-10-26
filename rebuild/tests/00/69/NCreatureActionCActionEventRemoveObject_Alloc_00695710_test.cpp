#include <cstdio>
// Behaviour test for NCreatureAction::CActionEventRemoveObject::Alloc (retail 0x00695710): the clone factory builds a fresh event with
// the given activation frame, thing and camera manager, Finished cleared, and an empty Hand string.
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
class CCharString { public:
    const char* PStringData;
    CCharString() : PStringData("") { }
};
struct CActionEventRemoveObject : CActionEventBase {
    CCharString Hand;
    CActionEventRemoveObject(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera)
        : CActionEventBase(activationFrame, thing, camera), Hand() { }
    virtual CActionEventBase* Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const;
};
CActionEventBase* CActionEventRemoveObject::Alloc(long activationFrame, CThingCreatureBase& thing, CGameCameraManager& camera) const
{
    return new CActionEventRemoveObject(activationFrame, thing, camera);
}

int main()
{
    CThingCreatureBase thing = { 1 };
    CThingCreatureBase thing2 = { 2 };
    CGameCameraManager camera = { 3 };
    CGameCameraManager camera2 = { 4 };
    CActionEventRemoveObject proto(5, thing, camera);
    proto.Finished = true;
    CActionEventBase* made = proto.Alloc(42, thing2, camera2);
    CActionEventRemoveObject* e = (CActionEventRemoveObject*)made;
    if (made == 0 || made == &proto || e->baseBuilt != 1 || e->Finished || e->ActivationFrame != 42
        || e->PThing != &thing2 || e->PCameraManager != &camera2 || e->Hand.PStringData == 0 || e->Hand.PStringData[0] != 0) {
        std::printf("00695710_TEST FAIL\n");
        return 1;
    }
    delete made;
    std::printf("00695710_TEST PASS\n");
    return 0;
}