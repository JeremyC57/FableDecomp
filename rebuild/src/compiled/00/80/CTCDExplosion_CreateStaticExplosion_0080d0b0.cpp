// CTCDExplosion::CreateStaticExplosion() -- when the explosion def has an emitter: place emitters along the explosion line
// (one per damage radius, plus one) and remember each live one with its creation frame; then, when the def asks for it,
// shake the camera at the middle of the line
// Leaf: CreateStaticExplosion
//
// Retail 0x0080D0B0, 580 bytes, plain `ret` (8-aligned frame).
// Callees: CParticleEmitterManager::Get @ 0x00AEAAF0 (static), __ftol2 @ 0x00BFEA70, C3DVector::AccurateNormalise @ 0x00A14440, the
// explosion def's emitter spacing @ 0x0080C190, the emitter creator @ 0x006E11F0 (__fastcall), CTCBase::GetWorld
// @ 0x00686A70, GetWorldFrame @ 0x0049D870 (static), CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 /
// GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10 (vtable 0x01238C6C), the emitter vector's push_back @ 0x00789515,
// CreateCameraShake @ 0x007F1B70 (__fastcall). Constants: 1.0f, 1e-4f, 0.5f.
// Layout: Ego_r.pdb CTCDExplosion as the landed FrameUpdate (0x0080D750): DamageRadius +0x0c, StartPos +0x20, EndPos +0x2c,
// PExplosionDef +0x38, CreatedEmitter +0x3c, Emitters +0x40; CExplosionDef emitter def +0x3c / +0x40, camera shake flag
// +0x66, shake strength +0x68 / +0x6c (descriptive).
#include <math.h>
#pragma intrinsic(sqrt)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    float MagnitudeSquared() const { return Z * Z + Y * Y + X * X; }
    float Magnitude() const { return (float)sqrt(X * X + Y * Y + Z * Z); }
    C3DVector& AccurateNormalise();                                        // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
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
    void push_back(const SEmitter& value);                   // 0x00789515
};
struct CExplosionDef {
    unsigned char _pad00[0x3c];
    long  EmitterDefIndex;                                   // +0x3c
    long  EmitterParam;                                      // +0x40
    unsigned char _pad44[0x66 - 0x44];
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
extern const float g_epsilon_0129ba3c;                       // 0x0129BA3C (1e-4f)
struct CWorld { static long __fastcall GetWorldFrame(); };   // 0x0049D870
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
    CWorld* GetWorld();                                      // 0x00686A70
    void CreateStaticExplosion();
};

void CTCDExplosion::CreateStaticExplosion()
{
    float radius = DamageRadius;
    CParticleEmitterManager* manager = CParticleEmitterManager::Get();
    if (PExplosionDef->EmitterDefIndex >= 0) {
        long count = (long)((EndPos - StartPos).Magnitude() / radius + 1.0f);
        C3DVector direction = EndPos - StartPos;
        if (direction.MagnitudeSquared() > g_epsilon_0129ba3c * g_epsilon_0129ba3c)
            direction.AccurateNormalise();
        float spacing = PExplosionDef->GetEmitterSpacing(radius);
        for (long i = 0; i != count; ++i) {
            C3DVector pos = StartPos + direction * (float)i * radius;
            CThing* emitter = manager->CreateEmitter(PExplosionDef->EmitterDefIndex, PExplosionDef->EmitterParam, spacing, pos, 0);
            if (emitter && !(emitter->m_flags91 & 1)) {
                long frame = GetWorld()->GetWorldFrame();
                Emitters.push_back(SEmitter(CIntelligentPointer(emitter), frame));
            }
        }
        CreatedEmitter = true;
    }
    if (PExplosionDef->ShakeCamera) {
        C3DVector middle = (StartPos + EndPos) * 0.5f;
        CreateCameraShake(&middle, PExplosionDef->ShakeStrength, PExplosionDef->ShakeDuration, 0);
    }
}