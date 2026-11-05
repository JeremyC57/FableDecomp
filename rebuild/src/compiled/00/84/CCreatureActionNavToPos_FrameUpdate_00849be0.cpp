// CCreatureAction_NavToPos::FrameUpdate() -- ask the creature's navigation component for the next step towards NavPos: on failure move to where it stands and finish; on a step move to position + step and, unless the player is Z-targeting, look at it (priority 0x11); on arrival move to where it stands and finish; then the base FrameUpdate
// Leaf: ?FrameUpdate@CCreatureAction_NavToPos@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00849BE0, 421 bytes, plain `ret` (virtual; Ego_r.pdb UAEXXZ).
// Callees (Ego_r spellings): CBaseIntelligentPointer::GetPItem() const 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*,
//   CKeyPairCompareLess<...> >::LowerBound 0x0040F020 (inside CThing::GetTC<CTCCreatureNavigation> / GetTC<CTCLook>),
//   CTCCreatureNavigation::GetNextNavPosOnRoute(const CNavigationInput&, bool, bool, bool, bool, bool) 0x006CBAC0 (returns
//   CTCCreatureNavigation::CCreatureNavOutput through the hidden pointer), CThing::GetPos() const 0x004C73D0, CThing::GetPlayer()
//   0x004C7A30 (CPlayer&; Ego_r donor order PeekPlayer 0x004C7A10, GetPlayer 0x004C7A30), CTCLook::LookAtPos(const
//   C3DVector&, long) 0x00753E90, CCreatureActionBase::SetMoveToPos (protected) 0x006929E0, the action's virtual FinishAction
//   (slot 4, +0x10; CCreatureAction_ReleaseTimeSpell::FinishAction 0x008CDD40 sits in that slot), the protected base virtual
//   CCreatureActionBase::FrameUpdate 0x00694020 (qualified call).
// Layout: retail CCreatureActionBase 0xa8 (engine/CCreatureActionBase.h; PCreature +0x08); CCreatureAction_NavToPos
//   PreviousPhysicsComponent +0xa8, NavPos +0xac (Ego_r +0xb4/+0xb8); CThing UsingComponents +0x20, Components +0x44;
//   CPlayer ZTargeting +0x20a (engine/CPlayer.h); CNavigationInput (Destination, Proximity, MovementType, Speed) and
//   CCreatureNavOutput (NavResult, MovementVector) from Ego_r. engine/CCreatureActionBase.h stores the vptr as a data member,
//   so the virtual chain (FinishAction, protected FrameUpdate) is modelled locally; slots 0..3 are unused filler declarations.
// Shape: the navigation input is a NAMED local built by the PDB's (destination, proximity, movement type) constructor with
//   the destination copied member by member (a temporary or an implicit vector copy shrinks the frame to 0x2c and loads
//   through `lea edx`); with the PDB names (GetTC<T>, GetPos, the reference-returning GetPlayer, the protected virtual base
//   FrameUpdate) VC7.1 keeps &PCreature in ebx as retail (the earlier stand-in model gave ebp); the GetTC lookup stores
//   `((it == last || type < it->first) ? last : it)->second` through the out pointer.
#define UNPROVEN_SLOT(n) virtual void UnprovenSlot##n();
#include "engine/C3DVector.h"
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CPlayer {
public:
    unsigned char _pad[0x20a];
    bool          ZTargeting;                         // +0x20a (engine/CPlayer.h)
};
enum EMovementType { MOVEMENT_TYPE_3 = 3 };           // descriptive
enum ENavigatorReturn { NAV_FAILED = 0, NAV_ARRIVED = 1, NAV_MOVING = 2 };   // descriptive
class CNavigationInput {
public:
    C3DVector     Destination;
    float         Proximity;
    EMovementType MovementType;
    float         Speed;
    CNavigationInput(const C3DVector& destination, float proximity, EMovementType type)
        : Proximity(proximity), MovementType(type), Speed(0.0f)
    {
        Destination.X = destination.X;
        Destination.Y = destination.Y;
        Destination.Z = destination.Z;
    }
};
class CTCCreatureNavigation {
public:
    class CCreatureNavOutput {
    public:
        ENavigatorReturn NavResult;
        C3DVector        MovementVector;
    };
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_NAVIGATION; }
    CCreatureNavOutput GetNextNavPosOnRoute(const CNavigationInput& input, bool a, bool b, bool c, bool d, bool e);   // 0x006CBAC0
};
class CTCLook {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_LOOK; }
    void LookAtPos(const C3DVector& position, long priority);   // 0x00753E90
};

class CThingCreatureBase : public CThing { };
class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CTCPhysicsBase;
class CCreatureActionBase {                           // retail sizeof 0xa8
public:
    UNPROVEN_SLOT(0) UNPROVEN_SLOT(1) UNPROVEN_SLOT(2) UNPROVEN_SLOT(3)
    virtual void FinishAction();                      // slot 4 (+0x10)
    unsigned char _pad04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;   // +0x08
    unsigned char _pad10[0xa8 - 0x10];
protected:
    void SetMoveToPos(const C3DVector& position);     // 0x006929E0
    virtual void FrameUpdate();                       // 0x00694020
};
class CCreatureAction_NavToPos : public CCreatureActionBase {
public:
    CTCPhysicsBase* PreviousPhysicsComponent;         // +0xa8
    C3DVector       NavPos;                           // +0xac
    virtual void FrameUpdate();
};

void CCreatureAction_NavToPos::FrameUpdate()
{
    CTCCreatureNavigation* navigation;
    PCreature.Get()->GetTC(&navigation);
    CNavigationInput input(NavPos, 0.0f, MOVEMENT_TYPE_3);
    CTCCreatureNavigation::CCreatureNavOutput output = navigation->GetNextNavPosOnRoute(input, false, true, true, false, false);
    if (output.NavResult == NAV_FAILED) {
        SetMoveToPos(PCreature.Get()->GetPos());
        FinishAction();
    } else if (output.NavResult == NAV_MOVING) {
        C3DVector position = PCreature.Get()->GetPos() + output.MovementVector;
        SetMoveToPos(position);
        CTCLook* look;
        if (PCreature.Get()->GetTC(&look) && !PCreature.Get()->GetPlayer().ZTargeting)
            look->LookAtPos(position, 0x11);
    } else if (output.NavResult == NAV_ARRIVED) {
        SetMoveToPos(PCreature.Get()->GetPos());
        FinishAction();
    }
    CCreatureActionBase::FrameUpdate();
}