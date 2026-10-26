// Test for 0x0084F1B0 NCameraModes::CBase::GetObservedThing: returns the camera manager's observed thing.
#include <stdio.h>
class CBaseObject { public: int Id; };
class CThingGameObject : public CBaseObject { };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* GetPItem() const { return PItem; }         // stand-in
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* Get() const { return (T*)GetPItem(); }
};
class CGameCameraManager {
public:
    unsigned char _before_PObservedThing[0x100];
    CIntelligentPointer<const CThingGameObject> PObservedThing;
    const CThingGameObject* GetObservedThing() const { return PObservedThing.Get(); }
};
namespace NCameraModes {
class CBase {
public:
    virtual ~CBase() { }
    unsigned char _before_CameraManager[0x3c];
    CGameCameraManager* CameraManager;
    const CThingGameObject* GetObservedThing() const;
};
}
const CThingGameObject* NCameraModes::CBase::GetObservedThing() const
{
    return CameraManager->GetObservedThing();
}
int main()
{
    CThingGameObject thing; thing.Id = 7;
    CGameCameraManager mgr; mgr.PObservedThing.PItem = &thing;
    NCameraModes::CBase mode; mode.CameraManager = &mgr;
    bool ok = (char*)&mode.CameraManager - (char*)&mode == 0x40 && (char*)&mgr.PObservedThing - (char*)&mgr == 0x100;
    ok = ok && mode.GetObservedThing() == &thing && mode.GetObservedThing()->Id == 7;
    mgr.PObservedThing.PItem = 0;
    ok = ok && mode.GetObservedThing() == 0;
    if (!ok) { printf("0084f1b0_TEST FAIL\n"); return 1; }
    printf("0084f1b0_TEST PASS\n");
    return 0;
}
