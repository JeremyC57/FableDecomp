// CTCPhysicsNavigator::ResetPosition(const C3DVector& pos) -- CTCPhysicsBase::ResetPosition, then the navigation target and the forced-move position both become pos and ForcedMoveToPosSet is cleared
// Retail: Fable.exe (TLC, VC7.1) @ 0x00726700, 79 bytes, CTCPhysicsNavigator vtable 0x01266314 slot 31 (catalogued as vfunc_31;
//   slot 31 is ResetPosition in CTCPhysicsBase 0x006B01D0 and CTCPhysicsLight 0x00723320).
// Callees: CTCPhysicsBase::ResetPosition @ 0x006B01D0 (CTCPhysicsStandard does not override it).
// Layout: engine/CTCPhysicsNavigator.h (packed 1): PositionToNavigateTo +0x89, ForcedMoveToPos +0xa1, the PDB bool:1 group at +0xad
//   (ForcedMoveToPosSet bit 0, ForcedMoveToPosUsePhysics bit 1; the header names the opaque byte after the last bit) through a view.
// Shape: two whole-vector assignments from the argument kept in edi (the first copied through `mov ecx,edi`), the bit cleared as
//   `mov al ; and al,0FEh ; mov`.
// Leaf: ResetPosition
#include "engine/CTCPhysicsNavigator.h"

struct C3DVector {
    float X, Y, Z;
};

struct SNavigatorFlags {                    // CTCPhysicsNavigator +0xad bool:1 group
    bool ForcedMoveToPosSet : 1;            // bit 0
    bool ForcedMoveToPosUsePhysics : 1;     // bit 1
};

struct CTCPhysicsBase_Calls {
    void ResetPosition(const C3DVector& pos);   // CTCPhysicsBase::ResetPosition 0x006B01D0
};

struct CTCPhysicsNavigator_Methods : CTCPhysicsNavigator {
    C3DVector& PositionToNavigateToVector() { return *reinterpret_cast<C3DVector*>(&PositionToNavigateTo_X); }
    C3DVector& ForcedMoveToPosVector()      { return *reinterpret_cast<C3DVector*>(&ForcedMoveToPos_X); }
    SNavigatorFlags& Flags()                { return *reinterpret_cast<SNavigatorFlags*>(ForcedMoveToPosUsePhysics); }
    void ResetPosition(const C3DVector& pos);
};

void CTCPhysicsNavigator_Methods::ResetPosition(const C3DVector& pos)
{
    reinterpret_cast<CTCPhysicsBase_Calls*>(this)->ResetPosition(pos);
    PositionToNavigateToVector() = pos;
    ForcedMoveToPosVector() = pos;
    Flags().ForcedMoveToPosSet = false;
}
