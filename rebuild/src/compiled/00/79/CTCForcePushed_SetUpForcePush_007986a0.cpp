// CTCForcePushed::SetUpForcePush(const C3DVector& direction, float distance, CThing& pusher) -- arm the push: remember where it started, how far and along which per-frame step it goes and who pushed; an AI creature being pushed has its animation suspended and its current action finished
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007986A0, length 202 bytes (+ int3 padding). __fastcall, `ret 0Ch`.
//         Ego_r.pdb: `?SetUpForcePush@CTCForcePushed@@QAEXABVC3DVector@@MAAVCThing@@@Z`.
// Callees: CThing::GetPosition @ 0x004C73D0; CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/
//          0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CThingAnimatable::SetSuspendAnimation(bool,
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

class C3DVector {
public:
    float X, Y, Z;
    float GetLength() const
    {
        float z = Z, y = Y, x = X;
        float s = x * x;
        s += y * y;
        s += z * z;
        return (float)sqrt(s);
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem((CBaseObject*)other.GetPItem());
        return *this;
    }
};

class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingClass;                       // +0x10 (1: AI creature)
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
class CThingAnimatable : public CThing {
public:
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
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
    PositionForcePushStarted = ParentThing->GetPosition();
    DistanceBeingPushed = distance;
    DirectionBeingPushed = direction;
    double length = direction.GetLength();
    DistanceMovingPerFrame = (float)length;
    PThingCausingForcePush = CIntelligentPointer<CThing>(&pusher);
    CThing* owner = ParentThing;
    if (owner->ThingClass == 1) {
        ((CThingAnimatable*)owner)->SetSuspendAnimation(true, 0);
        ((CThingCreatureBase*)owner)->FinishCurrentAction();
    }
    StartedMoving = false;
    Initialised = true;
}