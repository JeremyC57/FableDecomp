// CTCPhysicsStandard::~CTCPhysicsStandard() -- delete the owned physics mesh info, then (the implicit CTCPhysicsBase destructor) release PhysicsDef and run the CTCBase base destructor
// Leaf: ??1CTCPhysicsStandard@@UAE@XZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00723F90, 57 bytes, thiscall, tail `jmp ~CTCBase`.
// Identity (proven): stores vtable 0x01265F84 first -- RTTI .?AVCTCPhysicsStandard@@ : CTCPhysicsBase : CTCBase
//   (rtti-vtables.tsv / rtti_bases.tsv; its own slot-0 `??_G` 0x00724DE0 expands this body). The two callers are the slot-0
//   `??_G`s of the derived CTCPhysicsControlled (vtable 0x0126616C, 0x00725EF0) and CTCPhysicsNavigator (0x01266314,
//   0x00727200), whose implicit destructors add nothing: siblings calling one distant dtor call the base's (chunk 660).
//   Ego_r.pdb (0x0073AF80) and FableWin.pdb (0x02524EB0) spell ??1CTCPhysicsStandard@@UAE@XZ. The catalog label
//   "~CEngineInternalPrimitiveMeshBase" is propagated.
// Callees: operator delete 0x00BFE9BC, the PhysicsDef pointee's slot-1 Delete through the vtable, CTCBase::~CTCBase 0x00686830.
// Shape: user destructor `if (PPhysicsMeshInfo) delete PPhysicsMeshInfo;` (CThingPhysicsMeshInfo is trivially destructible, so
//   the test/je is the source's own `if`, chunk 664; the pointer is loaded before the vtable store). CTCPhysicsBase's
//   destructor is implicit (no CTCPhysicsBase vtable store), so its PhysicsDef CDefPointer release (engine/CDefPointer.h,
//   Object copied to a local, `dec [obj+4]`, Delete at zero, zeroed) is expanded before the tail jump.
// Layout: Ego_r.pdb CTCPhysicsBase (0x40): CTCBase 0xC, Position +0xC, OldPosition +0x18, OldPositionLastFrameSet +0x24,
//   Velocity +0x28, Radius +0x34, PhysicsDef CDefPointer<const CPhysicsDef> +0x38, Moving/OldMoving bits +0x3C;
//   CTCPhysicsStandard (0x88): PPhysicsMeshInfo +0x40, OffsetVector3D +0x44, RHSet +0x50, OldRHSet +0x68, RHSetLastFrameSet
//   +0x80, flag bits +0x84. Local CTCBase / CTCPhysicsBase / CTCPhysicsStandard models: engine/CTCBase.h and
//   engine/CTCPhysicsStandard.h flatten the vptr into a data member (header debt).
void __cdecl operator delete(void* block);              // 0x00BFE9BC
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"
#include "engine/CBaseClassNonCopyable.h"

class CThing;
class CPhysicsDef;

#include "engine/C3DVector.h"

class CRightHandedSet {
public:
    C3DVector Up;                                        // +0x00
    C3DVector Forward;                                   // +0x0C
};

class CThingPhysicsMeshInfo {                            // Ego_r 0x1C, trivially destructible
public:
    unsigned short BankIndex;                            // +0x00
    float RenderSizeX;                                   // +0x04
    C3DVector ToCentre;                                  // +0x08
    float BoundingRadius;                                // +0x14
    float BoundingCircleRadius;                          // +0x18
};

class CTCBase : public CBaseClassNonCopyable {
public:
    virtual ~CTCBase();                                  // 0x00686830
    CThing* ParentThing;                                 // +0x04
    bool ToBeRemoved;                                    // +0x08
    bool IsInGlobalUpdateTCs;                            // +0x09
};

class CTCPhysicsBase : public CTCBase {
public:
    C3DVector Position;                                  // +0x0C
    C3DVector OldPosition;                               // +0x18
    long OldPositionLastFrameSet;                        // +0x24
    C3DVector Velocity;                                  // +0x28
    float Radius;                                        // +0x34
    CDefPointer<const CPhysicsDef> PhysicsDef;           // +0x38
    bool Moving : 1;                                     // +0x3C
    bool OldMoving : 1;
};

class CTCPhysicsStandard : public CTCPhysicsBase {       // vtable 0x01265F84, 0x88 bytes
public:
    virtual ~CTCPhysicsStandard();
    CThingPhysicsMeshInfo* PPhysicsMeshInfo;             // +0x40
    C3DVector OffsetVector3D;                            // +0x44
    CRightHandedSet RHSet;                               // +0x50
    CRightHandedSet OldRHSet;                            // +0x68
    long RHSetLastFrameSet;                              // +0x80
    bool AlwaysFacingMovementDirection : 1;                     // +0x84
    bool AngledWithGround : 1;
    bool FeelsGravity : 1;
    bool FeelsAirResistance : 1;
    bool OnGround : 1;
    bool OnMesh : 1;
    bool InteractsWithThings : 1;
    bool CollidesWithThings : 1;
    bool CollidesWithThingsOneWay : 1;                     // +0x85
    bool CollidesWithLandscape : 1;
    bool PhysicsEnabled : 1;
    bool Immoveable : 1;
    bool NavigationLayers : 1;
    bool PushableByHero : 1;
    bool ExcludingCreatures : 1;
    bool Impassable : 1;
};

CTCPhysicsStandard::~CTCPhysicsStandard()
{
    if (PPhysicsMeshInfo)
        delete PPhysicsMeshInfo;
}