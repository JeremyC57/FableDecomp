// CTCPhysicsControlled::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const -- count this physics object and its moving category.
// Leaf: ?FillPhysicsDebugInfoForSelf@CTCPhysicsControlled@@UBEXAAVCPhysicsDebugInfo@@@Z
// Identity: proven; RTTI CTCPhysicsControlled vtable slot89; PDB virtual const FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&); Moving bit0+0x3C and own stats MovingControlledObjects offset match PDB.
// Layout: generated physics headers flatten vptr and omit the named Moving bit (some use the wrong struct key); local real polymorphic base/member chain preserves retail flag+0x3C.
class CPhysicsDebugInfo { public: long MovingLightObjects,MovingStandardObjects,MovingNavigatorObjects,MovingControlledObjects,TotalNoPhysicsObjects; };
class CThing; class CPhysicsDef;
class C3DVector { public: float X,Y,Z; };
class CTCBase { public: virtual ~CTCBase() {} CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCPhysicsBase:public CTCBase { public: C3DVector Position,OldPosition; long OldPositionLastFrameSet; C3DVector Velocity; float Radius; const CPhysicsDef* PhysicsDef; bool Moving:1; bool OldMoving:1; };
class CTCPhysicsStandard:public CTCPhysicsBase { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsControlled:public CTCPhysicsStandard { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsNavigator:public CTCPhysicsControlled { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
void CTCPhysicsControlled::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const { if(Moving) ++info.MovingControlledObjects; ++info.TotalNoPhysicsObjects; }