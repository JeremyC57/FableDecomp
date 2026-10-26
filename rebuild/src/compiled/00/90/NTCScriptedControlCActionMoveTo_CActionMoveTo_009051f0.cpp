// NTCScriptedControl::CActionMoveTo::CActionMoveTo(CThing*, long, float, bool, bool, bool, bool, bool, bool) -- move-to-thing scripted action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009051F0, length 149 bytes (`ret 24h`).
// Callees: CBase::CBase @ 0x0099A2F0 (the action root), CThing::GetScriptThing @ 0x004C7E50 (by-value
// CScriptThing built straight into PTargetThing through the hidden pointer), CBaseObject::CBaseObject
// @ 0x0099A2D0 (the CScriptThing root). Vtables 0x0127091C = NTCScriptedControl::CActionMoveTo, 0x01238C8C =
// CScriptThing (rtti-vtables.tsv).
//
// Shape: the landed scripted-action shape (novtable CActionBase zeroing PTCScript/Activated/Terminated from the
// ebx zero register), the scalar initialisers MovementType/FaceMovement/Proximity scheduled around the vtable
// store, PTargetThing from the target's GetScriptThing, UseTargetThing = true, PTCOldPhysics = 0, PRouteThing
// default-constructed inline (out-of-line root constructor, CScriptThing vtable, zeroed counted pointer),
// then the remaining flags in member order with ActionPointIndex = -1; Pos is left uninitialised. Members
// carry the PDB names and offsets; the 4-argument twin is 0x00905290.
// Leaf: ??0CActionMoveTo

class CThing;

struct CBase {                                  // root of the scripted actions (vtable 0x0129A7C4)
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

namespace N_0099a2d0 { struct CBaseObject {                            // root of CScriptThing (vtable 0x01231710)
    virtual void v00();
    CBaseObject();                              // 0x0099A2D0
}; } using N_0099a2d0::CBaseObject;

struct SCountBlock;

struct CCountedPointer {
    void* m_pObject;                            // +0
    SCountBlock* m_pCount;                      // +4
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
};

class CScriptThing : public CBaseObject { public:             // vtable 0x01238C8C, 12 bytes (engine/CScriptThing.h layout)
    virtual void v00();
    CCountedPointer PImp;                       // +0x04
    CScriptThing() : CBaseObject() { }
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

struct __declspec(novtable) CActionBase : CBase {   // abstract: its own vtable store is dead
    void* PTCScript;                            // +0x04 CTCScriptedControl*
    bool  Activated;                            // +0x08
    bool  Terminated;                           // +0x09
    CActionBase() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
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
    virtual void v00();
    CActionMoveTo(CThing* target, long movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                  bool cancelAllActionsOnFailure, bool ignorePathPreferability);
    CActionMoveTo(CThing* routeThing, long movementType, bool faceMovement, bool avoidDynamicObstacles);
};

}

namespace NTCScriptedControl {

CActionMoveTo::CActionMoveTo(CThing* target, long movementType, float proximity, bool faceMovement,
                             bool avoidDynamicObstacles, bool useActionPoint, bool useBackupProximity,
                             bool cancelAllActionsOnFailure, bool ignorePathPreferability)
    : CActionBase()
    , MovementType(movementType)
    , FaceMovement(faceMovement)
    , Proximity(proximity)
    , PTargetThing(target->GetScriptThing())
    , UseTargetThing(true)
    , PTCOldPhysics(0)
    , PRouteThing()
    , UseRouteThing(false)
    , AvoidDynamicObstacles(avoidDynamicObstacles)
    , PreviousPushableByHero(false)
    , UseActionPoint(useActionPoint)
    , ActionPointIndex(-1)
    , GotActionPointIndex(false)
    , UseBackupProximityForActionPointIfRouteBlocked(useBackupProximity)
    , CancelAllActionsOnFailure(cancelAllActionsOnFailure)
    , IgnorePathPreferability(ignorePathPreferability)
{
}

}