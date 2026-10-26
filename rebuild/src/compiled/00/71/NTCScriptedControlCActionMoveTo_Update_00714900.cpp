// NTCScriptedControl::CActionMoveTo::Update() -- move the scripted creature towards its position or target thing: refresh both handles, stop ignoring anything for navigation, fail when a claimed action point's thing is dead or carried, then (when the creature is idle) step along the nav route, facing or looking where it goes; on a blocked route fall back to the backup proximity or cancel the script, and on arrival or failure make the hero stop looking and finish
// Leaf: ?Update@CActionMoveTo@NTCScriptedControl@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00714900, 960 bytes (+ int3 padding), plain `ret`. Ego_r.pdb
//   ?Update@CActionMoveTo@NTCScriptedControl@@UAEXXZ (CActionBase slot 1).
// Callees: the script thing's slots 71/76/77 Refresh/IsDead/IsValid (+0x11c/+0x130/+0x134) and 11 GetThing (+0x2c);
//   CVectorMap::LowerBound @ 0x0040F020 (the inlined GetInterface lookups: 7 CTCCreatureNavigation = sign of byte +0x20,
//   0x43 CTCLook = bit 3 of byte +0x28) and its const twin @ 0x004365B0 (0x47 CTCCarryable = sign of byte +0x28);
//   CTCCreatureNavigation::SetThingToIgnore @ 0x006CBE30 / GetNextNavPosOnRoute @ 0x006CBAC0 / SetNavigationTarget
//   @ 0x006CB960 (as the landed CActionFollowCreature::Update 0x00713EE0); CThingCreatureBase::IsPerformingAnyAction
//   @ 0x006622A0; CTCLook::LookAtPos @ 0x00753E90 / IsPosInViewRange @ 0x007534B0 / StopLooking @ 0x00751B80; the
//   physics component's slot 33 (+0x84); CTCScriptedControl's out-of-line action cancel @ 0x00713630 and the action
//   list's clear() @ 0x00719280 (the inline ClearActions of the landed CActionStopScripting::Update 0x00713660).
// Data: 0.0f @ 0x0122DEDC, epsilon 0.0001f @ 0x0129BA3C (squared at run time), 0.41666666f pushed as an immediate.
// Layout: CActionMoveTo per Ego_r.pdb (Pos +0x0c, MovementType +0x18, FaceMovement +0x1c, Proximity +0x20, PTargetThing
//   +0x24, UseTargetThing +0x30, PRouteThing +0x38, AvoidDynamicObstacles +0x45, UseActionPoint +0x47,
//   GotActionPointIndex +0x4c, UseBackupProximityForActionPointIfRouteBlocked +0x4d, CancelAllActionsOnFailure +0x4e,
//   IgnorePathPreferability +0x4f; the landed OnCleanup 0x00714730 agrees); CTCCarryable PCarryingTC +0x10 (PDB);
//   CNavigationInput / CCreatureNavOutput per the PDB (the default constructor zeroes Proximity, MovementType and Speed,
//   as the landed CBrainOutput ctor 0x00835620); retail CThing local model (thing type tag +0x10, 2 = the hero).
// Shape: the parent thing is a local for the navigation lookup and the idle test; the target thing's position is an
//   inline physics accessor (the copy goes through the returned reference); the action-point proximity ternary sits in
//   a float local (x87 `fld`/`fstp`, not integer moves); the nav result is read before the Active store; the squared
//   length is written X*X + Y*Y + Z*Z over the output member (emitted Z, Y, X); the step target is `offset + position`
//   through an inline operator+.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    __forceinline C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
class CTCPhysicsBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032();
    virtual void SetFacingPos(const C3DVector& pos);   // slot 33 (+0x84; as the landed 0x00713EE0)
    unsigned char _pad04[0xc - 0x4];
    C3DVector Position;                         // +0x0C
    const C3DVector& GetPosition() const { return Position; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing { public:
    void*         __vftable;
    unsigned char _pad04[0x10 - 0x04];
    unsigned char ThingType;                    // +0x10 (2 = the hero; descriptive)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long UsingComponents[9];           // +0x20
    CInterfaceMap Components;                   // +0x44
    unsigned char _pad50[0x60 - 0x50];
    CTCPhysicsBase* PPhysics;                   // +0x60
    bool IsPerformingAnyAction();               // 0x006622A0 (CThingCreatureBase)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetConstInterface(const T** ppOut, long interfaceType) const
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            const CInterfaceMap* map = &Components;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (const T*)it->component;
            return true;
        }
        return false;
    }
};
class CTCCarrying;
class CTCCarryable { public:                           // interface 0x47
    unsigned char _pad00[0x10];
    CTCCarrying*  PCarryingTC;                  // +0x10
};
class CNavigationInput { public:
    C3DVector Destination;                      // +0x00
    float     Proximity;                        // +0x0c
    long      MovementType;                     // +0x10
    float     Speed;                            // +0x14
    CNavigationInput() : Proximity(0), MovementType(0), Speed(0) { }
};
struct CCreatureNavOutput {
    long      NavResult;                        // +0x00 (0 arrived, 1 failed, 2 moving)
    C3DVector MovementVector;                   // +0x04
};
class CTCCreatureNavigation { public:                  // interface 7
    unsigned char _pad[0x56];
    bool Active;                                // +0x56
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool b1, bool b2, bool b3, bool b4, bool b5);   // 0x006CBAC0
    void SetNavigationTarget(const CNavigationInput& input);  // 0x006CB960 (PDB ?SetNavigationTarget@CTCCreatureNavigation@@QAEXABVCNavigationInput@@@Z)
};
class CTCLook { public:                                // interface 0x43
    void LookAtPos(const C3DVector* position, long priority);   // 0x00753E90
    bool IsPosInViewRange(const C3DVector& pos, float angle, bool a, bool b);   // 0x007534B0
    void StopLooking(long timeToStop);          // 0x00751B80
};
extern float g_navEpsilon;                      // 0x0129BA3C (0.0001f)
class CScriptThing { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010();
    virtual CThing* GetThing() const;   // slot 11 (+0x2c)
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043();
    virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051();
    virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069(); virtual void v070();
    virtual void Refresh();             // slot 71 (+0x11c)
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075();
    virtual bool IsDead() const;        // slot 76 (+0x130)
    virtual bool IsValid() const;       // slot 77 (+0x134)
    long _a, _b;
};

class CCPPointerInfo;
namespace NTCScriptedControl { class CActionBase; }
struct SActionNode {
    SActionNode*                     _M_next;      // +0x00
    SActionNode*                     _M_prev;      // +0x04
    NTCScriptedControl::CActionBase* Data;         // +0x08
    CCPPointerInfo*                  Info;         // +0x0c
};
struct CActionList {                               // STLport list<CCountedPointer<CActionBase>>
    SActionNode* _M_node;
    bool empty() const { return _M_node->_M_next == _M_node; }
    void clear();                                  // 0x00719280
};
class CTCScriptedControl { public:
    void*        __vftable;
    CThing*      ParentThing;                      // +0x04
    unsigned char _pad08[0x14 - 0x08];
    CActionList  PActionList;                      // +0x14
    NTCScriptedControl::CActionBase* GetCurrentAction();
    void ClearActions();
    void CleanupAllActions();                      // 0x00713630 (PDB name ?CleanupAllActions@CTCScriptedControl@@QAEXXZ)
};

namespace NTCScriptedControl {
class CActionBase {
public:
    virtual ~CActionBase();
    virtual void Update();                         // slot 1
    virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnCleanup();                      // slot 5 (+0x14)
    CTCScriptedControl* PTCScript;                 // +0x04
    bool                Activated;                 // +0x08
    bool                Terminated;                // +0x09
};

class CActionMoveTo : public CActionBase {
public:
    virtual void Update();
    C3DVector    Pos;                              // +0x0C
    long         MovementType;                     // +0x18
    bool         FaceMovement;                     // +0x1C
    float        Proximity;                        // +0x20
    CScriptThing PTargetThing;                     // +0x24
    bool         UseTargetThing;                   // +0x30
    CTCBase*     PTCOldPhysics;                    // +0x34
    CScriptThing PRouteThing;                      // +0x38
    bool         UseRouteThing;                    // +0x44
    bool         AvoidDynamicObstacles;            // +0x45
    bool         PreviousPushableByHero;           // +0x46
    bool         UseActionPoint;                   // +0x47
    long         ActionPointIndex;                 // +0x48
    bool         GotActionPointIndex;              // +0x4C
    bool         UseBackupProximityForActionPointIfRouteBlocked;   // +0x4D
    bool         CancelAllActionsOnFailure;        // +0x4E
    bool         IgnorePathPreferability;          // +0x4F
};
}

inline NTCScriptedControl::CActionBase* CTCScriptedControl::GetCurrentAction()
{
    if (PActionList.empty())
        return 0;
    return PActionList._M_node->_M_next->Data;
}

inline void CTCScriptedControl::ClearActions()
{
    NTCScriptedControl::CActionBase* action = GetCurrentAction();
    if (action && action->Activated)
        action->OnCleanup();
    PActionList.clear();
}

namespace NTCScriptedControl {
void CActionMoveTo::Update()
{
    if (!PTargetThing.IsValid())
        PTargetThing.Refresh();
    if (!PRouteThing.IsValid())
        PRouteThing.Refresh();
    CTCCreatureNavigation* ignoring;
    if (PTCScript->ParentThing->GetInterface(&ignoring, 7))
        ignoring->SetThingToIgnore(0);
    if (GotActionPointIndex) {
        const CTCCarryable* carryable;
        if (PTargetThing.IsDead()
            || (PTargetThing.GetThing()->GetConstInterface(&carryable, 0x47) && carryable->PCarryingTC)) {
            Terminated = true;
            if (CancelAllActionsOnFailure)
                PTCScript->ClearActions();
            return;
        }
    }
    CThing* parent = PTCScript->ParentThing;
    CTCCreatureNavigation* nav;
    if (!parent->GetInterface(&nav, 7))
        return;
    if (parent->IsPerformingAnyAction())
        return;
    CNavigationInput input;
    if (UseTargetThing) {
        if (PTargetThing.IsDead()) {
            Terminated = true;
            return;
        }
        input.Destination = PTargetThing.GetThing()->PPhysics->GetPosition();
        input.Proximity = Proximity;
        input.MovementType = MovementType;
    } else {
        float proximity = UseActionPoint ? 0.0f : Proximity;
        input.Proximity = proximity;
        input.Destination = Pos;
        input.MovementType = MovementType;
    }
    CCreatureNavOutput output = nav->GetNextNavPosOnRoute(input, false, AvoidDynamicObstacles, true, false, IgnorePathPreferability);
    nav->SetNavigationTarget(input);
    long result = output.NavResult;
    nav->Active = true;
    if (result == 2) {
        if (!(output.MovementVector.X * output.MovementVector.X + output.MovementVector.Y * output.MovementVector.Y + output.MovementVector.Z * output.MovementVector.Z > g_navEpsilon * g_navEpsilon))
            return;
        C3DVector target = output.MovementVector + PTCScript->ParentThing->PPhysics->Position;
        if (!FaceMovement) {
            PTCScript->ParentThing->PPhysics->SetFacingPos(target);
            return;
        }
        CTCLook* look;
        if (PTCScript->ParentThing->GetInterface(&look, 0x43)) {
            look->LookAtPos(&target, 0x21);
            if (look->IsPosInViewRange(target, 0.41666666f, false, false))
                PTCScript->ParentThing->PPhysics->SetFacingPos(target);
        }
        return;
    }
    if (result == 1) {
        if (UseActionPoint && UseBackupProximityForActionPointIfRouteBlocked) {
            UseActionPoint = false;
            return;
        }
        if (GotActionPointIndex && CancelAllActionsOnFailure)
            PTCScript->CleanupAllActions();
    }
    CThing* thing = PTCScript->ParentThing;
    CTCLook* look;
    if (thing->ThingType == 2 && thing->GetInterface(&look, 0x43))
        look->StopLooking(0);
    Terminated = true;
}
}