// CTCDExplosion::UpdateDynamicExplosion() -- advance a ring explosion: find the ring whose cumulative duration covers the frames elapsed; when it changes, spawn that ring's emitters evenly round the circle (on the ground) and remember each with its frame, then damage the band between the previous and current ring radii; finally, when the def has a secondary emitter, replace every live emitter older than the delay with a secondary one inserted in front of it
// Grade: DIFFER(1031v1097) -- 242 difflines (parked 999/334): the ring offset is (XAxis * sin + YAxis * cos) * radius with literal unit axes (retail's 0.0f-pool multiplies are the unit-vector products; cos evaluated first as the right operand). Residue: the ring-count loop (retail tests def first, then re-reads PExplosionDef->Rings each pass; writing the condition through PExplosionDef gives 1061/346), ebp/edi swap, the secondary-emitter insert temporary
// Leaf: UpdateDynamicExplosion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080D300, 1104 bytes, 8-aligned ebp frame, plain `ret`.
// Callees: CTCBase::GetWorld 0x00686A70, CWorld::GetWorldMap (slot 12) / GetWorldFrame 0x0049D870, __ftol2 0x00BFEA70,
// CThing::GetPosition 0x004C73D0, CWorldMap::GetHeightZBelowPos 0x0063E480, CExplosionDef::GetEmitterSpacing 0x0080C190,
// emitter creator 0x006E11F0 (__fastcall), CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem
// 0x00A01B50 / dtor 0x00A01C10, emitter vector insert-overflow 0x007893AE / insert 0x0080EF50, UpdateDynamicHitThings 0x0080C6D0.
// Layout: as the landed CreateStaticExplosion (0x0080D0B0) plus FrameCount +0x4c, CurrentRing +0x50; def rings vector +0x58
// (16-byte {radius, count, start angle, duration}), secondary emitter +0x44/+0x48, delay +0x4c.
#include <math.h>
#pragma intrinsic(sqrt, sin, cos)
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float MagnitudeSquared() const { return Z * Z + Y * Y + X * X; }
    float Magnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    void Normalise();                                        // 0x00A14440
};
inline C3DVector operator+(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X + b.X, a.Y + b.Y, a.Z + b.Z); }
inline C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
inline C3DVector operator*(const C3DVector& a, float s) { return C3DVector(a.X * s, a.Y * s, a.Z * s); }
struct CThing {
    unsigned char _pad00[0x91];
    unsigned char m_flags91;                                 // +0x91 bit 0: dead
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CThing* item);                             // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct SEmitter {                                            // pair<CIntelligentPointer<CThing>, long>
    CIntelligentPointer first;
    long                second;
    SEmitter(const CIntelligentPointer& a, const long& b) : first(a), second(b) { }
};
struct CEmitterVector {
    SEmitter* _M_start;
    SEmitter* _M_finish;
    SEmitter* _M_end_of_storage;
    void InsertOverflow(SEmitter* where, const SEmitter& value, const struct SScalarTag& tag, unsigned long count, bool atEnd);   // 0x007893AE
    SEmitter* insert(SEmitter* where, const SEmitter& value);   // 0x0080EF50
    void push_back(const SEmitter& value);
};
struct CExplosionDef {
    unsigned char _pad00[0x3c];
    long  EmitterDefIndex;                                   // +0x3c
    long  EmitterParam;                                      // +0x40
    long  SecondaryEmitterDefIndex;                          // +0x44
    long  SecondaryEmitterParam;                             // +0x48
    float SecondaryEmitterDelay;                             // +0x4c
    unsigned char _pad50[0x58 - 0x50];
    struct SRing { float Radius; long Count; float StartAngle; float Duration; };
    struct CRingVector {
        SRing* _M_start;
        SRing* _M_finish;
        SRing* _M_end_of_storage;
        unsigned long size() const { return _M_finish - _M_start; }
    } Rings;                                                 // +0x58
    unsigned char _pad64[0x66 - 0x64];
    bool  ShakeCamera;                                       // +0x66
    long  ShakeStrength;                                     // +0x68
    long  ShakeDuration;                                     // +0x6c
    float GetEmitterSpacing(float radius) const;             // 0x0080C190
};
struct CParticleEmitterManager {
    static CParticleEmitterManager* Get();                   // 0x00AEAAF0
    static CThing* __fastcall CreateEmitter(long defIndex, long param, float spacing, const C3DVector& pos, long flags);   // 0x006E11F0
};
void __fastcall CreateCameraShake(const C3DVector* position, long strength, long duration, long flags);   // 0x007F1B70
extern long g_FramesPerSecond;                               // 0x01375550
struct CWorldMap { float GetHeightZBelowPos(const C3DVector& pos) const; };   // 0x0063E480
struct CWorld {
#define VF(n) virtual void v##n();
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11)
#undef VF
    virtual CWorldMap* GetWorldMap();                        // slot 12 (+0x30)
    static long __fastcall GetWorldFrame();                  // 0x0049D870
};
struct CThingPos { const C3DVector& GetPosition() const; };   // CThing::GetPosition 0x004C73D0
struct CTCDExplosion {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    unsigned char _pad08[0x0c - 0x08];
    float DamageRadius;                                      // +0x0c
    float PhysicsRadius;                                     // +0x10
    float MaxDamage;                                         // +0x14
    long  FireDamage;                                        // +0x18
    long  ExplosionLifeTimer;                                // +0x1c
    C3DVector StartPos;                                      // +0x20
    C3DVector EndPos;                                        // +0x2c
    const CExplosionDef* PExplosionDef;                      // +0x38
    bool  CreatedEmitter;                                    // +0x3c
    bool  NeedToTriggerExplosion;                            // +0x3d
    CEmitterVector Emitters;                                 // +0x40
    long  FrameCount;                                        // +0x4c
    long  CurrentRing;                                       // +0x50
    CWorld* GetWorld();                                      // 0x00686A70
    void UpdateDynamicHitThings(float innerRadius, float outerRadius);   // 0x0080C6D0
    void UpdateDynamicExplosion();
};


struct SScalarTag { SScalarTag() { } };
inline void* operator new(unsigned int, void* p) { return p; }
inline void CEmitterVector::push_back(const SEmitter& value)
{
    if (_M_finish != _M_end_of_storage) {
        if (_M_finish)
            new (_M_finish) SEmitter(value);
        ++_M_finish;
    } else
        InsertOverflow(_M_finish, value, SScalarTag(), 1, true);
}
inline SEmitter MakeEmitter(const CIntelligentPointer& a, const long& b) { return SEmitter(a, b); }

void CTCDExplosion::UpdateDynamicExplosion()
{
    float radius = DamageRadius;
    CWorldMap* map = GetWorld()->GetWorldMap();
    ++FrameCount;
    const CExplosionDef* def = PExplosionDef;
    long framesPerSecond = g_FramesPerSecond;
    long frames = 0;
    unsigned long ring = 0;
    for (; ring < def->Rings.size() && frames < FrameCount; ++ring)
        frames += (long)(framesPerSecond * def->Rings._M_start[ring].Duration);
    --ring;
    if ((long)ring != CurrentRing) {
        CurrentRing = ring;
        CreatedEmitter = true;
        const CExplosionDef::SRing& current = def->Rings._M_start[ring];
        long count = current.Count;
        float step = 1.0f / count;
        float ringRadius = current.Radius;
        float angle = current.StartAngle;
        for (long n = count; n > 0; --n) {
            double a = angle * 6.283185307179586;
            C3DVector offset = (C3DVector(1.0f, 0.0f, 0.0f) * (float)sin(a) + C3DVector(0.0f, 1.0f, 0.0f) * (float)cos(a)) * ringRadius;
            C3DVector pos = offset + ((CThingPos*)ParentThing)->GetPosition();
            pos.Z = map->GetHeightZBelowPos(pos);
            CThing* emitter = CParticleEmitterManager::CreateEmitter(PExplosionDef->EmitterDefIndex, PExplosionDef->EmitterParam,
                                                                     PExplosionDef->GetEmitterSpacing(radius), pos, 0);
            if (emitter && !(emitter->m_flags91 & 1)) {
                long frame = GetWorld()->GetWorldFrame();
                Emitters.push_back(MakeEmitter(CIntelligentPointer(emitter), frame));
            }
            angle += step;
        }
        float inner = (long)ring > 0 ? radius + def->Rings._M_start[ring - 1].Radius : 0.0f;
        UpdateDynamicHitThings(inner, ringRadius + radius);
    }
    if (def->SecondaryEmitterParam > 0 && def->SecondaryEmitterDefIndex > 0) {
        for (SEmitter* it = Emitters._M_start; it != Emitters._M_finish; ) {
            CThing* thing = it->first.GetPItem();
            long created = it->second;
            if (thing && !(thing->m_flags91 & 1) && created > 0
                && GetWorld()->GetWorldFrame() > (long)(framesPerSecond * def->SecondaryEmitterDelay + created)) {
                it->second = -1;
                CThing* emitter = CParticleEmitterManager::CreateEmitter(def->SecondaryEmitterDefIndex, def->SecondaryEmitterParam,
                                                                         def->GetEmitterSpacing(radius),
                                                                         ((CThingPos*)thing)->GetPosition(), 0);
                if (emitter && !(emitter->m_flags91 & 1)) {
                    it = Emitters.insert(it, MakeEmitter(CIntelligentPointer(emitter), -1));
                    continue;
                }
            }
            ++it;
        }
    }
}
