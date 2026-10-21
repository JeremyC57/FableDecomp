// Grade: DIFFER(351v349) -- the environment pointer is loaded straight into ecx in retail; the wind copy sits above the direction in the frame and the normalised components are multiplied by r from memory (fstp r; fxch; fmul [r]); ours multiplies through st(2) and stores the wind below
// CTCPerceiveThing::UpdateSmell() -- within smelling range, the perceived thing is smelt when the wind blows from it towards the owner (the unit direction from it to the owner lies within 70 degrees of the wind); a smelt, alive thing out of sight makes the owner directly aware of it
// Leaf: UpdateSmell
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00744B50, 349 bytes (plain `ret`, tail jump to SetLastAwareInfo).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x2); CTCBase::GetWorld() const 0x00686A80; the world's environment
//          object (+0x18) virtual slot 2 (the wind direction, const C3DVector&); CThing::GetPosition 0x004C73D0 (x2);
//          CTCPerceiveThing::SetLastAwareInfo 0x00744730.
// Globals: the reciprocal-square-root mantissa table 0x013CE558, 3.0f 0x0122DED4, 0.5f 0x0122F59C, the double 1.2217305
//          (70 degrees) 0x01268940 fed to the fcos intrinsic.
// Layout: rebuild/include/engine/CTCPerceiveThing.h (retail offsets): ParentThing +0x04, PerceivedThing +0x0c,
//          DistanceFromPerceivedThingSquared +0x14, FramesSinceDirectlyPerceivedThing +0x1c, LineOfSight +0x2d,
//          DirectAwareness +0x30, Smelt +0x31, SmellRadius +0x48.
// Shape: the wind is a named copy taken before the positions; the direction is C3DVector's inline fast Normalise (as the landed
//          CTCAssassinRush::SetupFacingDirection 0x007A6E20) and the test `dot > cos(70 deg)`.
#include <math.h>
#pragma intrinsic(cos)
extern unsigned long g_InvSqrtMantissaTable[128];   // 0x013CE558
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
    __forceinline void Normalise()
    {
        float x = X;
        float y = Y;
        float z = Z;
        float lengthSquared = x * x + y * y + z * z;
        unsigned long bits = *(unsigned long*)&lengthSquared;
        unsigned long estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                                 (g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
        float r = *(float*)&estimate;
        r = (3.0f - r * r * lengthSquared) * r * 0.5f;
        X *= r;
        Y *= r;
        Z *= r;
    }
};
class CThing {
public:
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
struct CEnvironment {
    virtual void v00();
    virtual void v01();
    virtual const C3DVector& GetWindDirection() const;   // slot 2
};
struct CWorld {
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
};
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCPerceiveThing {
    void*            __vftable;                 // +0x00
    CThing*          ParentThing;               // +0x04 (CTCBase)
    unsigned char    _pad_0x08[0x4];
    CBaseIntelligentPointer PerceivedThing;     // +0x0c CIntelligentPointer<CThing const>
    float            DistanceFromPerceivedThingSquared;   // +0x14
    float            AwarenessTimer;            // +0x18
    long             FramesSinceDirectlyPerceivedThing;   // +0x1c
    C3DVector        LastPerceivedPos;          // +0x20
    bool             Perceiving;                // +0x2c
    bool             LineOfSight;               // +0x2d
    bool             ExtendedLineOfSight;       // +0x2e
    bool             IgnoreFOVLineOfSight;      // +0x2f
    bool             DirectAwareness;           // +0x30
    bool             Smelt;                     // +0x31
    bool             Heard;                     // +0x32
    bool             SearchingForThingToPerceive;   // +0x33
    void*            PDef;                      // +0x34
    long             UpdateOffset;              // +0x38
    float            SoundRadius;               // +0x3c
    float            SightRadius;               // +0x40
    float            ExtendedSightRadius;       // +0x44
    float            SmellRadius;               // +0x48
    const CWorld* GetWorld() const;             // 0x00686A80 (CTCBase)
    void SetLastAwareInfo();                    // 0x00744730
    void UpdateSmell();
};

void CTCPerceiveThing::UpdateSmell()
{
    Smelt = false;
    if (DistanceFromPerceivedThingSquared < SmellRadius * SmellRadius) {
        CThing* perceived = PerceivedThing.GetPItem();
        C3DVector wind = GetWorld()->PEnvironment->GetWindDirection();
        CThing* owner = ParentThing;
        C3DVector direction = owner->GetPosition() - perceived->GetPosition();
        direction.Normalise();
        if (wind.Dot(direction) > cos(1.2217304763960306)) {
            Smelt = true;
            if (!LineOfSight && PerceivedThing.GetPItem()) {
                DirectAwareness = true;
                FramesSinceDirectlyPerceivedThing = 0;
                SetLastAwareInfo();
            }
        }
    }
}
