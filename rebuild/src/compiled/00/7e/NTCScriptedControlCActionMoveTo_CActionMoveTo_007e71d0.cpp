// NTCScriptedControl::CActionMoveTo::CActionMoveTo(const C3DVector& pos, EMovementType movementType, float proximity, bool faceMovement, bool avoidDynamicObstacles) -- constructor: the common action members zeroed, own vtable, the move parameters, two empty CScriptThings (target / route), the flags cleared and no action point (-1).
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- CScriptThing::PImp modelled as the PDB's CCountedPointer<CScriptThing> record (one inline ctor), not two raw words: the second CScriptThing's vtable store stays above its PImp zeros
// Leaf: ??0CActionMoveTo
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E71D0, length 154 bytes (+ int3 padding), `ret 14h`.
// Callees: CBase::CBase @ 0x0099A2F0 (the action root) and the CScriptThing root constructor @ 0x0099A2D0 (each
// CScriptThing; also labelled CBase::CBase, a separate byte twin, so it is a separate symbol here). Vtables 0x0127091C = NTCScriptedControl::CActionMoveTo (rtti-vtables.tsv),
// 0x01238C8C = CScriptThing.
// Ego_r.pdb CActionMoveTo (0x50): Pos +0x0c, MovementType +0x18, FaceMovement +0x1c, Proximity +0x20, PTargetThing
// +0x24, UseTargetThing +0x30, PTCOldPhysics +0x34, PRouteThing +0x38, UseRouteThing +0x44, AvoidDynamicObstacles
// +0x45, PreviousPushableByHero +0x46, UseActionPoint +0x47, ActionPointIndex +0x48, GotActionPointIndex +0x4c,
// UseBackupProximityForActionPointIfRouteBlocked +0x4d, CancelAllActionsOnFailure +0x4e, IgnorePathPreferability +0x4f.
// Shape: the landed CActionFireProjectileWeapon (0x00716790) model: novtable CAction intermediate, zero stores
// sharing ebx; Pos through C3DVector's user copy constructor (member-wise, no address formed); the CScriptThing members' inline default constructors (out-of-line CBase, vtable, zeroed PImp).
struct CBase {                                  // the action root
    virtual void v00();
    CBase();                                    // 0x0099A2F0
};

struct CScriptBase {                            // CScriptThing's root (the manifest also calls it CBase::CBase)
    virtual void v00();
    CScriptBase();                              // 0x0099A2D0
};

struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};
typedef long EMovementType;
struct CTCBase;

template <class T> struct CCountedPointer {      // the counted pointer record with its own inline constructor
    T*    Object;
    void* Count;
    CCountedPointer() : Object(0), Count(0) { }
};

struct CScriptThing : CScriptBase {             // vtable 0x01238C8C, 12 bytes
    virtual void v00();
    CCountedPointer<CScriptThing> PImp;         // +0x04 (Ego_r.pdb: CCountedPointer<CScriptThing> PImp -- object, count block)
    CScriptThing() : CScriptBase(), PImp() { }
};

struct __declspec(novtable) CAction : CBase {   // NTCScriptedControl::CActionBase: abstract, its own vtable store is dead
    void*   PTCScript;                          // +0x04 CTCScriptedControl*
    bool    Activated;                          // +0x08
    bool    Terminated;                         // +0x09
    CAction() : CBase(), PTCScript(0), Activated(false), Terminated(false) { }
};

struct CActionMoveTo : CAction {                // vtable 0x0127091C
    C3DVector     Pos;                          // +0x0c
    EMovementType MovementType;                 // +0x18
    bool          FaceMovement;                 // +0x1c
    float         Proximity;                    // +0x20
    CScriptThing  PTargetThing;                 // +0x24
    bool          UseTargetThing;               // +0x30
    CTCBase*      PTCOldPhysics;                // +0x34
    CScriptThing  PRouteThing;                  // +0x38
    bool          UseRouteThing;                // +0x44
    bool          AvoidDynamicObstacles;        // +0x45
    bool          PreviousPushableByHero;       // +0x46
    bool          UseActionPoint;               // +0x47
    long          ActionPointIndex;             // +0x48
    bool          GotActionPointIndex;          // +0x4c
    bool          UseBackupProximityForActionPointIfRouteBlocked;   // +0x4d
    bool          CancelAllActionsOnFailure;    // +0x4e
    bool          IgnorePathPreferability;      // +0x4f
    virtual void v00();
    CActionMoveTo(const C3DVector& pos, EMovementType movementType, float proximity, bool faceMovement,
                  bool avoidDynamicObstacles);
};

CActionMoveTo::CActionMoveTo(const C3DVector& pos, EMovementType movementType, float proximity, bool faceMovement,
                             bool avoidDynamicObstacles)
    : CAction()
    , Pos(pos)
    , MovementType(movementType)
    , FaceMovement(faceMovement)
    , Proximity(proximity)
    , PTargetThing()
    , UseTargetThing(false)
    , PTCOldPhysics(0)
    , PRouteThing()
    , UseRouteThing(false)
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