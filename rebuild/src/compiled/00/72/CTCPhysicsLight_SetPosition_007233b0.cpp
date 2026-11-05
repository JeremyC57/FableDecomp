// CTCPhysicsLight::SetPosition(const C3DVector& pos) -- store the position, move the parent thing to the map number the world map reports there, and mark the body moving
// Retail: Fable.exe (TLC, VC7.1) @ 0x007233B0, 71 bytes, CTCPhysicsLight vtable 0x01265DFC slot 30.
// Callees: 0x006AF620 (CTCPhysicsBase member: GetWorld()->GetWorldMap(), out of line; manifest "CThingTrackNode::DrawIsDrawable",
//   propagated), the world map's virtual slot 16 (+0x40) = the map number at a position, CThing::SetCurrentMapNumber @ 0x004C7580
//   (stores a 16-bit map number at +0x8e), CTCPhysicsBase::SetAsMoving @ 0x006B0130.
// Layout: engine/CTCPhysicsBase.h (Position +0x0c viewed as a C3DVector, +0x04 the parent thing); AffectedByGravity at +0x40.
// Shape: the argument stays in edi; the position copy goes through `mov ecx,edi`; the thing is fetched into ebx between the
//   world-map call and the argument push (the object expression of the outer call). The virtual returns a LONG narrowed to the
//   16-bit parameter: that puts `push eax` before `mov ecx,ebx` (an unsigned short return schedules the mov first).
// Leaf: SetPosition
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
#include "engine/CTCPhysicsBase.h"

#include "engine/C3DVector.h"

class CWorldMap { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual long GetMapNumberAt(const C3DVector& pos);   // slot 16 (+0x40)
};

#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"

struct CTCPhysicsBase_Calls {
    CWorldMap* GetWorldMap();               // 0x006AF620
    void SetAsMoving(bool moving);          // 0x006B0130
};

struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;             // +0x40 bit 0
    CTCPhysicsBase_Calls* Base() { return reinterpret_cast<CTCPhysicsBase_Calls*>(this); }
    CThing* GetThing() { return reinterpret_cast<CThing*>(sub); }
    C3DVector& PositionVector() { return *reinterpret_cast<C3DVector*>(&Position_X); }
    void SetPosition(const C3DVector& pos);
};

void CTCPhysicsLight::SetPosition(const C3DVector& pos)
{
    PositionVector() = pos;
    GetThing()->SetCurrentMapNumber(Base()->GetWorldMap()->GetMapNumberAt(pos));
    Base()->SetAsMoving(true);
}