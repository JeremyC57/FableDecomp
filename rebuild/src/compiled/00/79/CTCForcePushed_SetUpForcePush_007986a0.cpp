// CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher) -- arm the push: remember where it started, how far and along which per-frame step it goes and who pushed; an AI creature being pushed has its animation suspended and its current action finished
// Leaf: ?SetUpForcePush@CTCForcePushed@@QAEXABVC3DVector@@MAAVCThing@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Also: local GetLength respelled GetMagnitude (the header / FableWin.pdb method, same computation).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007986A0, length 202 bytes (+ int3 padding). __fastcall, `ret 0Ch`.
//         Ego_r.pdb: `?SetUpForcePush@CTCForcePushed@@QAEXABVC3DVector@@MAAVCThing@@@Z`.
// Callees: CThing::GetPos @ 0x004C73D0; CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/
//          0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CThingAICreature::SuspendAI(bool,
//          long) @ 0x00833A10; CThingCreatureBase::FinishCurrentAction @ 0x00662650.
// Shape: the two vector members are implicit C3DVector copies (destination through a `lea` register); the per-frame
//        distance is the step vector's length (x87: z, y, x loaded, then x*x + y*y + z*z and fsqrt); the pusher is the
//        converting temporary assigned through `SetPItem(GetPItem())`; the owner is read once into edi for the AI tail.
//        The length goes through a DOUBLE local narrowed into DistanceMovingPerFrame: retail moves the root under the
//        components (`fstp st(3)`), pops them, then stores; a direct float assignment stores before the pops.
// Layout: Ego_r.pdb CTCForcePushed (matches retail): PThingCausingForcePush +0x0C, PositionForcePushStarted +0x1C,
//         DistanceBeingPushed +0x28, DirectionBeingPushed +0x2C, DistanceMovingPerFrame +0x38, Initialised +0x3C,
//         StartedMoving +0x3D; retail CThing ThingClass +0x10 (1 = AI creature).
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(sqrt)

class CThing;
class CBaseObject;

#include "engine/C3DVector.h"

class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

#include "engine/CThing.h"
class CThingAICreature { public:
    void SuspendAI(bool suspend, void* suspender);   // 0x00833A10 (Ego_r.pdb CThingAICreature::SuspendAI)
};
class CThingAnimatable : public CThing {
};
class CThingCreatureBase : public CThingAnimatable {
public:
    void FinishCurrentAction();                     // 0x00662650
};

class CTCForcePushed {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    CIntelligentPointer<CThing> PThingCausingForcePush;     // +0x0C
    CIntelligentPointer<CThing> PParticle;          // +0x14
    C3DVector PositionForcePushStarted;             // +0x1C
    float DistanceBeingPushed;                      // +0x28
    C3DVector DirectionBeingPushed;                 // +0x2C
    float DistanceMovingPerFrame;                   // +0x38
    bool Initialised;                               // +0x3C
    bool StartedMoving;                             // +0x3D
    void SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher);
};

void CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher)
{
    PositionForcePushStarted = ParentThing->GetPos();
    DistanceBeingPushed = distance;
    DirectionBeingPushed = direction;
    double length = direction.GetMagnitude();
    DistanceMovingPerFrame = (float)length;
    PThingCausingForcePush = CIntelligentPointer<CThing>(&pusher);
    CThing* owner = ParentThing;
    if (owner->GetType() == 1) {
        ((CThingAICreature*)(((CThingAnimatable*)owner)))->SuspendAI(true, 0);
        ((CThingCreatureBase*)owner)->FinishCurrentAction();
    }
    StartedMoving = false;
    Initialised = true;
}