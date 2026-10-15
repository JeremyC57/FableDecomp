// CTCPhysicsBase::ResetPosition(const C3DVector& pos) -- SetPosition(pos) through the vtable, then OldPosition = pos (no interpolation), flag the thing frame-updated and set Moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B01D0, 54 bytes, CTCPhysicsBase vtable slot 31.
// Callees: virtual SetPosition (slot 30, +0x78; CTCPhysicsBase 0x006B0C10), CThing::SetAsFrameUpdated @ 0x004C7C40.
// Layout: engine/CTCPhysicsBase.h (OldPosition +0x18, the parent thing +0x04, Moving = bit 0 of +0x3c through a bit-field view).
// Shape: the argument kept in edi across the virtual call; the flag set as mov al / or al,1 / mov around the epilogue pops.
// Leaf: ResetPosition
#include "engine/CTCPhysicsBase.h"

struct C3DVector {
    float X, Y, Z;
};

struct SPhysicsBaseFlags {                  // CTCPhysicsBase +0x3c bool:1 group
    bool Moving : 1;                        // bit 0
    bool OldMoving : 1;                     // bit 1
};

struct CThing {
    void SetAsFrameUpdated();               // 0x004C7C40
};

struct CTCPhysicsBaseVirtuals {                    // vtable-shaped view of `this` (slot = offset / 4)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void SetPosition(const C3DVector& pos);   // slot 30 (+0x78)
};

struct CTCPhysicsBase_Methods : CTCPhysicsBase {
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    C3DVector& OldPositionVector() { return *reinterpret_cast<C3DVector*>(&OldPosition_X); }
    SPhysicsBaseFlags& Flags()     { return *reinterpret_cast<SPhysicsBaseFlags*>(OldMoving); }
    void ResetPosition(const C3DVector& pos);
};

void CTCPhysicsBase_Methods::ResetPosition(const C3DVector& pos)
{
    reinterpret_cast<CTCPhysicsBaseVirtuals*>(this)->SetPosition(pos);
    OldPositionVector() = pos;
    GetThing()->SetAsFrameUpdated();
    Flags().Moving = true;
}
