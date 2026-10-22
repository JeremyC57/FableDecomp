// Grade: DIFFER(778v778) -- same length; Emitter2D is held in dl across CalcFadeAlpha (the same-TU stand-in body preserves edx) where retail keeps it in bl with ebx pushed in the prologue; without the stand-in VC reloads the flag word after the call
// NParticleEngine::CParticleEmitter::IsVisible(float distance, const CEnginePrimitivePrepareInfo* info) -- decide whether the emitter draws this frame: work out the distance fade (2D emitters use distance 0), clear Visible, and unless sleeping either show a 2D emitter outright, or (within the draw range, past the fade-in start, with some alpha left) test the cached bounding sphere against the engine when it is valid, else rebuild it from every active system's world-space sphere (the emitter position with radius 0.1 when a system has none), test each one, and keep the new sphere when every system supplied one; each system gets the fade alpha and its own Visible flag
// Leaf: IsVisible
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AED2F0, 778 bytes (+ int3 padding), `ret 8`.
// Callees: CParticleEmitter::CalcFadeAlpha(float) 0x00AECD70 (label "IsVisible_CalcFadeAlpha"; same TU, its body is
//   compiled above as the stand-in so VC7.1 knows it writes no emitter flags and keeps the flag word in edi across the
//   call, as retail does), CParticleSystem::GetBoundingSphereWorldSpace 0x00AF3FB0, CParticleEmitter::GetPosition 0x006E1B10
//   (ICF-shared with CTCCameraPointScripted's getter; Transform translation +0xd4), C3DBoundingSphere::AddToBounds
//   0x00ADDD80, the engine's virtual slot 21 (+0x54, IsSphereVisible). Floats 0.0 0x0122DEDC, 0.1 immediate.
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (the landed constructor 0x00AF0380): ParticleSystems +0x08, UpdateParameters
//   +0x14, flag word +0x40 (Sleeping bit 10, Emitter2D bit 11), flag bytes +0x44 (Visible bit 3) / +0x45
//   (CalcBoundingSphereOnceOnly bit 1, BoundingSphereValid bit 2), BSphere +0x144, DistanceToCam +0x154, MaxDrawDistance
//   +0x15c, FadeOutStart +0x160, FadeInStart +0x164, FadeInEnd +0x168. CParticleSystem: flag byte +0x50 (Enabled bit 1,
//   Dead bit 2, Visible bit 5), GlobalAlpha +0x54, FadeAlpha +0x58 (landed 0x00AF4660). CEnginePrimitivePrepareInfo engine
//   +0x34 (landed 0x00AEB2D0).
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
struct C3DBoundingSphere {
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
    C3DBoundingSphere(const C3DVector& center, float radius) : Center(center), Radius(radius) {}
    void Set(const C3DVector& center, float radius) { Center = center; Radius = radius; }
    void AddToBounds(const C3DVector& center, float radius);   // 0x00ADDD80
};
struct CMatrix3x4 {
    C3DVector Right, Up, Forward, Translation;
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20();
    virtual bool IsSphereVisible(const C3DBoundingSphere& sphere);   // slot 21 (+0x54)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[4];
    float         AlwaysUpdateDistance;                // +0x04
    unsigned char _pad08[0x1c];
    float         Distance;                            // +0x24
    unsigned char _pad28[0xc];
    CIEngine*     Engine;                              // +0x34
};
namespace NParticleEngine {
struct CParticleUpdateParameters {
    void*         __vftable;                           // +0x00
    unsigned char _maps[0x20];                         // +0x04 VectorParams, UpdateParams
};
struct CParticleSystem {
    unsigned char _state00[0x50];
    bool Initialised : 1;                              // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    bool StayWithEmitter : 1;
    bool ScaleParticles : 1;
    float GlobalAlpha;                                 // +0x54
    float FadeAlpha;                                   // +0x58
    bool GetBoundingSphereWorldSpace(C3DBoundingSphere* sphere, float distance, CParticleUpdateParameters* params);   // 0x00AF3FB0
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
class CParticleEmitter {
public:
    void*                       __vftable;             // +0x00
    void*                       m_pIntelligentPointer; // +0x04
    CVector<CParticleSystemPtr> ParticleSystems;       // +0x08
    CParticleUpdateParameters   UpdateParameters;      // +0x14
    unsigned char               PrimitiveGroupHandle[8];   // +0x38
    unsigned short Priority : 3;                       // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;
    unsigned short Emitter2DLayer : 7;                 // +0x42
    unsigned char PreWaterEffect : 1;                  // +0x44
    unsigned char WaterEffect : 1;
    unsigned char ZBufferWriteable : 1;
    unsigned char Visible : 1;
    unsigned char AlwaysUpdate : 1;
    unsigned char DieIfOffsceen : 1;
    unsigned char OffscreenUpdate : 1;
    unsigned char ClipEffectToWeatherMask : 1;
    unsigned char EnableDithering : 1;                 // +0x45
    unsigned char CalcBoundingSphereOnceOnly : 1;
    unsigned char BoundingSphereValid : 1;
    unsigned char ClearedParticles : 1;
    unsigned char ClearedDecals : 1;
    unsigned char               _state46[0x6a];        // +0x46 motion, interpolators
    CMatrix3x4                  Transform;             // +0xb0
    CMatrix3x4                  InverseTransform;      // +0xe0
    CMatrix3x4                  EndTransform;          // +0x110
    void*                       Name;                  // +0x140 CCharString
    C3DBoundingSphere           BSphere;               // +0x144
    float                       DistanceToCam;         // +0x154
    float                       MaxSpawnDistance;      // +0x158
    float                       MaxDrawDistance;       // +0x15c
    float                       FadeOutStart;          // +0x160
    float                       FadeInStart;           // +0x164
    float                       FadeInEnd;             // +0x168
    float CalcFadeAlpha(float distance) const;         // 0x00AECD70
    bool IsSleeping() const { return Sleeping; }
    __forceinline void UpdateSystemsVisibility(float alpha)
    {
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
            CParticleSystem* system = it->Data;
            system->FadeAlpha = alpha;
            system->Visible = system->Enabled && !system->Dead && system->GlobalAlpha > 0.0f;
        }
    }
    __forceinline bool BuildBoundingSphere(float distance, const CEnginePrimitivePrepareInfo* info, float alpha)
    {
        bool allValid = true;
        C3DBoundingSphere sphere;
        for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
            CParticleSystem* system = it->Data;
            bool visible = false;
            if (system->Enabled && !system->Dead && system->GlobalAlpha > 0.0f) {
                if (!system->GetBoundingSphereWorldSpace(&sphere, distance, &UpdateParameters)) {
                    sphere.Center = GetPosition();
                    sphere.Radius = 0.1f;
                    allValid = false;
                }
                visible = info->Engine->IsSphereVisible(sphere);
            }
            Visible |= visible;
            system->Visible = visible;
            system->FadeAlpha = alpha;
            BSphere.AddToBounds(sphere.Center, sphere.Radius);
        }
        return allValid;
    }
    C3DVector GetPosition() const;                     // 0x006E1B10
    bool IsVisible(float distance, const CEnginePrimitivePrepareInfo* info);
};

#pragma auto_inline(off)
__declspec(noinline) float CParticleEmitter::CalcFadeAlpha(float distance) const
{
    if (distance < FadeInStart || distance > MaxDrawDistance)
        return 0.0f;
    if (distance < FadeInEnd)
        return (distance - FadeInStart) / (FadeInEnd - FadeInStart);
    if (distance < FadeOutStart)
        return 1.0f;
    return 1.0f - (distance - FadeOutStart) / (MaxDrawDistance - FadeOutStart);
}
#pragma auto_inline(on)

bool CParticleEmitter::IsVisible(float distance, const CEnginePrimitivePrepareInfo* info)
{
    bool is2D = Emitter2D;
    float alpha = CalcFadeAlpha(is2D ? 0.0f : DistanceToCam);
    Visible = false;
    if (IsSleeping())
        return Visible;
    if (is2D) {
        Visible = true;
        UpdateSystemsVisibility(alpha);
        return Visible;
    }
    if (DistanceToCam <= MaxDrawDistance && DistanceToCam >= FadeInStart && alpha > 0.0f) {
        if (CalcBoundingSphereOnceOnly && BoundingSphereValid) {
            Visible = info->Engine->IsSphereVisible(BSphere);
            UpdateSystemsVisibility(alpha);
            return Visible;
        }
        BSphere.Set(C3DVector(0.0f, 0.0f, 0.0f), 0.0f);
        bool allValid = BuildBoundingSphere(distance, info, alpha);
        BoundingSphereValid = allValid & (BSphere.Radius > 0.0f);
    }
    return Visible;
}
}
