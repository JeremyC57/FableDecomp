// NTCScriptedControl::CActionMoveTo::CActionMoveTo(CThing*, long, bool, bool) -- follow-route scripted move action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00905290, length 129 bytes (`ret 10h`).
// Callees: CBaseClassNonCopyable::CBaseClassNonCopyable @ 0x0099A2F0 (the action root), CBaseClass::CBaseClass @ 0x0099A2D0 (the CScriptThing
// root), CThing::GetScriptThing @ 0x004C7E50 (by-value CScriptThing built straight into PRouteThing). Vtables
// 0x0127091C = NTCScriptedControl::CActionMoveTo, 0x01238C8C = CScriptThing (rtti-vtables.tsv).
//
// Shape: the 4-argument twin of 0x009051F0: here the thing is the ROUTE thing -- PTargetThing is default-
// constructed inline, PRouteThing comes from GetScriptThing, UseTargetThing = false, UseRouteThing = true,
// Proximity 0 (stored from the ebx zero register), every other flag false and ActionPointIndex = -1; the
// argument flags are FaceMovement and AvoidDynamicObstacles. Pos is left uninitialised.
// Leaf: ??0CActionMoveTo

class CThing;

class CBaseClassNonCopyable {
protected:                                   // root of the scripted actions (vtable 0x0129A7C4)
    virtual ~CBaseClassNonCopyable();   // 0x0099A300
    CBaseClassNonCopyable();                                    // 0x0099A2F0
};

class CBaseClass {
public:                             // root of CScriptThing (vtable 0x01231710)
    virtual void v00();
    CBaseClass();                              // 0x0099A2D0
};

struct SCountBlock;

struct CCountedPointer {
    void* m_pObject;                            // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CScriptThing : public CBaseClass { public:             // vtable 0x01238C8C, 12 bytes (engine/CScriptThing.h layout)
    virtual void v00();
    CCountedPointer PImp;                       // +0x04
    CScriptThing() : CBaseClass() { }
    CScriptThing(const CScriptThing& other);    // non-POD: returned through a hidden pointer
};

class CThing { public:
    CScriptThing GetScriptThing();              // 0x004C7E50
};

class C3DVector { public:
    float X, Y, Z;
};

class CTCBase;

namespace NTCScriptedControl {

struct __declspec(novtable) CActionBase : CBaseClassNonCopyable {   // abstract: its own vtable store is dead
    void* PTCScript;                            // +0x04 CTCScriptedControl*
    bool  Activated;                            // +0x08
    bool  Terminated;                           // +0x09
    CActionBase() : CBaseClassNonCopyable(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionMoveTo : CActionBase {            // vtable 0x0127091C, 0x50 bytes
    C3DVector    Pos;                           // +0x0c
    long         MovementType;                  // +0x18 EMovementType
    bool         FaceMovement;                  // +0x1c
    float        Proximity;                     // +0x20
    CScriptThing PTargetThing;                  // +0x24
    bool         UseTargetThing;                // +0x30
    CTCBase*     PTCOldPhysics;                 // +0x34
    CScriptThing PRouteThing;                   // +0x38
    bool         UseRouteThing;                 // +0x44
    bool         AvoidDynamicObstacles;         // +0x45
    bool         PreviousPushableByHero;        // +0x46
    bool         UseActionPoint;                // +0x47
    long         ActionPointIndex;              // +0x48
    bool         GotActionPointIndex;           // +0x4c
    bool         UseBackupProximityForActionPointIfRouteBlocked;   // +0x4d
    bool         CancelAllActionsOnFailure;     // +0x4e
    bool         IgnorePathPreferability;       // +0x4f
    CActionMoveTo(CThing* target, long movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                  bool cancelAllActionsOnFailure, bool ignorePathPreferability);
    CActionMoveTo(CThing* routeThing, long movementType, bool faceMovement, bool avoidDynamicObstacles);
};

}

namespace NTCScriptedControl {

CActionMoveTo::CActionMoveTo(CThing* routeThing, long movementType, bool faceMovement, bool avoidDynamicObstacles)
    : CActionBase()
    , MovementType(movementType)
    , FaceMovement(faceMovement)
    , Proximity(0.0f)
    , PTargetThing()
    , UseTargetThing(false)
    , PTCOldPhysics(0)
    , PRouteThing(routeThing->GetScriptThing())
    , UseRouteThing(true)
    , AvoidDynamicObstacles(avoidDynamicObstacles)
    , PreviousPushableByHero(false)
    , UseActionPoint(false)
    , ActionPointIndex(-1)
    , GotActionPointIndex(false)
    , UseBackupProximityForActionPointIfRouteBlocked(false)
    , CancelAllActionsOnFailure(false)
    , IgnorePathPreferability(false)
{
}

}