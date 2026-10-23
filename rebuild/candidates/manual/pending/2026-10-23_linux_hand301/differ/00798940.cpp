// Grade: DIFFER(615v615) -- same size and flow; the expected-position sum loads the physics operand first where retail loads the push direction first (fadd operand order), and the frame packs the remaining distance, delta and move target into one slot where retail gives the remaining distance its own word (frame 0x138 vs 0x140)
// CTCForcePushed::FrameUpdate() -- keeps the creature in the being-pushed action, slides it along the push direction each frame and finishes the push when it has gone far enough or has been blocked (with a hit explosion when flagged)
#include <math.h>
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector operator+(const C2DVector& o) const { return C2DVector(X + o.X, Y + o.Y); }
    float MagSquared() const { return X * X + Y * Y; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    float MagSquared() const { return X * X + Y * Y + Z * Z; }
};
class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;
    mutable long RefCount;
};
template<class T> class CDefPointer {
public:
    T* Object;
    __forceinline CDefPointer() : Object(0) {}
    __forceinline ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CParticleEffectsDef : public CDefClassBase {
public:
    bool LookupParticleType(const CCharString& name, long* type) const;   // 0x00669D30
    };
class CSpecialAbilitiesForcePushDef : public CDefClassBase {
public:
    bool GetSubDef(CDefPointer<const CParticleEffectsDef>* out) const;   // 0x00669580
};
// Same-TU stand-in for the out-of-line GetSubDef (retail's link-time codegen saw that the out pointer does not escape).
#pragma auto_inline(off)
bool CSpecialAbilitiesForcePushDef::GetSubDef(CDefPointer<const CParticleEffectsDef>* out) const
{
    out->Object = (const CParticleEffectsDef*)this;
    return true;
}
#pragma auto_inline(on)
class CThing;
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* pos);   // 0x0072AB70
class CThingPhysics {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c(); virtual void v1d();
    virtual void v1e(); virtual void v1f(); virtual void v20(); virtual void v21();
    virtual void SetPosition(const C3DVector& pos, bool flag);   // slot 34 (+0x88)
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual void v29(); virtual void v2a(); virtual void v2b(); virtual void v2c(); virtual void v2d(); virtual void v2e();
    virtual void v2f(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v3a();
    virtual void v3b(); virtual void v3c(); virtual void v3d(); virtual void v3e(); virtual void v3f(); virtual void v40();
    virtual void v41();
    virtual void SetAngle(float angle);                 // slot 66 (+0x108)
    unsigned char _pad04[0x18 - 0x04];
    struct C2DVector Last;                              // +0x18
};
class CCreatureActionBase {
public:
    virtual void v00();
    unsigned char _pad04[0x10c];
    ~CCreatureActionBase();                             // 0x0062E900
};
class CThingCreatureBase;
class CCreatureAction_BeingForcePushed : public CCreatureActionBase {
public:
    CCreatureAction_BeingForcePushed(CThingCreatureBase* owner);   // 0x008BD0B0
};
class CThingCreatureBase {
public:
    unsigned char _pad00[0x60];
    CThingPhysics* Physics;                             // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char Flags6C;                              // +0x6C
    bool SetCurrentAction(CCreatureActionBase* action); // 0x006644F0
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CTCBase {
public:
    virtual void v00();
    CThingCreatureBase* Thing;                          // +0x04
    unsigned char _pad08[4];
};
struct CIntelligentPointerPad { void* vptr; void* item; };
class CTCForcePushed : public CTCBase {
public:
    CIntelligentPointerPad PThingCausingForcePush;      // +0x0C
    CIntelligentPointerPad PParticle;                   // +0x14
    C3DVector PositionForcePushStarted;                 // +0x1C
    float DistanceBeingPushed;                          // +0x28
    C3DVector DirectionBeingPushed;                     // +0x2C
    float DistanceMovingPerFrame;                       // +0x38
    bool Initialised;                                   // +0x3C
    bool StartedMoving;                                 // +0x3D
    CDefPointer<const CSpecialAbilitiesForcePushDef> PDef;   // +0x40
    void FinishBeingForcePushed(bool blocked, float remaining);   // 0x00798770
    void FrameUpdate();
};

void CTCForcePushed::FrameUpdate()
{
    CThingCreatureBase* thing = Thing;
    thing->SetCurrentAction(&CCreatureAction_BeingForcePushed(thing));
    C3DVector pos = thing->GetPosition();
    float distSq = (pos - PositionForcePushStarted).MagSquared();
    if (distSq < DistanceBeingPushed * DistanceBeingPushed) {
        CThingPhysics* physics = thing->Physics;
        if (StartedMoving) {
            C2DVector delta = C2DVector(physics->Last.X + DirectionBeingPushed.X, physics->Last.Y + DirectionBeingPushed.Y) - C2DVector(pos.X, pos.Y);
            float limit = DistanceMovingPerFrame * 0.2f;
            if (delta.MagSquared() > limit * limit) {
                float remaining = 0.0f;
                if (thing->Flags6C & 2) {
                    remaining = DistanceBeingPushed - sqrt(distSq);
                    CDefPointer<const CParticleEffectsDef> effects;
                    long type = 0;
                    if (PDef->GetSubDef(&effects) && effects->LookupParticleType("HIT_EXPLOSION", &type))
                        CreateParticleEffect(type, &thing->GetPosition());
                }
                FinishBeingForcePushed(true, remaining);
                return;
            }
        }
        physics->SetPosition(pos + DirectionBeingPushed, true);
        float angle = (float)atan2(-DirectionBeingPushed.X, -DirectionBeingPushed.Y) * 0.15915494f;
        if (angle < 0.0f || angle >= 1.0f) {
            angle = (float)fmod((double)angle, 1.0);
            if (angle < 0.0f)
                angle += 1.0f;
        }
        physics->SetAngle(angle);
        StartedMoving = true;
        return;
    }
    FinishBeingForcePushed(false, 0.0f);
}
