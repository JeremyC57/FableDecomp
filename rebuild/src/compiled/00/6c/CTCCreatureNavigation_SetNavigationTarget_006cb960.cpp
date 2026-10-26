// CTCCreatureNavigation::SetNavigationTarget(const CNavigationInput& target) -- NavigationTarget = target
// Leaf: SetNavigationTarget
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CB960, length 44 bytes. No callees.
// Callers: CThingAICreature::UpdateAI (0x006D5F34), CActionFollowCreature::Update (0x0071423E/0x00714B28),
// CCreatureAction_StealThingFromShop::PerformAction (0x00849BB5: a destination vector, proximity 0,
// movement type 3, speed 0 built on the frame) -- all on the creature's navigation component.
// Shape: the plain 24-byte record assigned as a whole (`add ecx,1Ch` + six dword moves through edx).
// Layout: PDB CTCCreatureNavigation has NavigationTarget (CNavigationInput {C3DVector Destination;
// float Proximity; EMovementType MovementType; float Speed;}) at +0x20; retail +0x1c, the
// PossibleNavigators vector before it being 12 bytes in retail (16 in the PDB). No engine header exists.
// The manifest label "CScriptModuleBase::SetCoordAxis" is propagated (byte-identical to 0x00851D30 apart
// from the member offset).
class C3DVector { public:
    float X, Y, Z;
};

enum EMovementType { MOVEMENT_TYPE_DEFAULT };

class CNavigationInput { public:                      // 24 bytes, plain record
    C3DVector     Destination;                 // +0x00
    float         Proximity;                   // +0x0c
    EMovementType MovementType;                // +0x10
    float         Speed;                       // +0x14
};

class CTCCreatureNavigation { public:
    unsigned char    _pad00[0x1c];
    CNavigationInput NavigationTarget;         // +0x1c
    void SetNavigationTarget(const CNavigationInput& target);
};

void CTCCreatureNavigation::SetNavigationTarget(const CNavigationInput& target)
{
    NavigationTarget = target;
}