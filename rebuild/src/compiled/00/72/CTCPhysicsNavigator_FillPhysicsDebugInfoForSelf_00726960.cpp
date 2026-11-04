// CTCPhysicsNavigator::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const -- count this physics object and its moving category.
// Leaf: ?FillPhysicsDebugInfoForSelf@CTCPhysicsNavigator@@UBEXAAVCPhysicsDebugInfo@@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Identity: proven; RTTI CTCPhysicsNavigator vtable slot89; PDB virtual const FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&); Moving bit0+0x3C and own stats MovingNavigatorObjects offset match PDB.
// Layout: generated physics headers flatten vptr and omit the named Moving bit (some use the wrong struct key); local real polymorphic base/member chain preserves retail flag+0x3C.
#define CTCPhysicsNavigator CTCPhysicsNavigator_HeaderLayout
#include "engine/CTCPhysicsNavigator.h"
#undef CTCPhysicsNavigator
class CPhysicsDebugInfo { public: long MovingLightObjects,MovingStandardObjects,MovingNavigatorObjects,MovingControlledObjects,TotalNoPhysicsObjects; };
class CThing; class CPhysicsDef;
#include "engine/C3DVector.h"
class CTCBase { public: virtual ~CTCBase() {} CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
class CTCPhysicsBase:public CTCBase { public: C3DVector Position,OldPosition; long OldPositionLastFrameSet; C3DVector Velocity; float Radius; const CPhysicsDef* PhysicsDef; bool Moving:1; bool OldMoving:1; };
class CTCPhysicsStandard:public CTCPhysicsBase { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsControlled:public CTCPhysicsStandard { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
class CTCPhysicsNavigator:public CTCPhysicsControlled { public: virtual void FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo&) const; };
void CTCPhysicsNavigator::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo& info) const { if(Moving) ++info.MovingNavigatorObjects; ++info.TotalNoPhysicsObjects; }