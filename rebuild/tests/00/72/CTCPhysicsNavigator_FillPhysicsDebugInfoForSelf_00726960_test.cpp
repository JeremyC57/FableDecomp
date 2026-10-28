#include <stdio.h>
class CPhysicsDebugInfo { public: long MovingLightObjects,MovingStandardObjects,MovingNavigatorObjects,MovingControlledObjects,TotalNoPhysicsObjects; };
class CThing; class CPhysicsDef;
class C3DVector { public: float X,Y,Z; };
class CTCBase { public: virtual ~CTCBase() {} CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCPhysicsBase:public CTCBase { public: C3DVector Position,OldPosition; long OldPositionLastFrameSet; C3DVector Velocity; float Radius; const CPhysicsDef* PhysicsDef; bool Moving:1; bool OldMoving:1; };
class CTCPhysicsStandard:public CTCPhysicsBase { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsControlled:public CTCPhysicsStandard { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsNavigator:public CTCPhysicsControlled { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
void CTCPhysicsNavigator::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const { if(Moving) ++info.MovingNavigatorObjects; ++info.TotalNoPhysicsObjects; }
void CTCPhysicsStandard::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const {}
void CTCPhysicsControlled::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const {}
int main() { CTCPhysicsNavigator object; CPhysicsDebugInfo info; info.MovingLightObjects=10; info.MovingStandardObjects=20; info.MovingNavigatorObjects=30; info.MovingControlledObjects=40; info.TotalNoPhysicsObjects=50; CPhysicsDebugInfo expected=info;
 object.Moving=true; object.OldMoving=true; object.FillPhysicsDebugInfoForSelf(info); ++expected.MovingNavigatorObjects; ++expected.TotalNoPhysicsObjects;
 if(info.MovingLightObjects!=expected.MovingLightObjects || info.MovingStandardObjects!=expected.MovingStandardObjects || info.MovingNavigatorObjects!=expected.MovingNavigatorObjects || info.MovingControlledObjects!=expected.MovingControlledObjects || info.TotalNoPhysicsObjects!=expected.TotalNoPhysicsObjects) goto fail;
 object.Moving=false; object.OldMoving=true; object.FillPhysicsDebugInfoForSelf(info); ++expected.TotalNoPhysicsObjects;
 if(info.MovingNavigatorObjects!=expected.MovingNavigatorObjects || info.TotalNoPhysicsObjects!=expected.TotalNoPhysicsObjects) goto fail;
 puts("00726960_TEST PASS"); return 0; fail: puts("00726960_TEST FAIL"); return 1;
}