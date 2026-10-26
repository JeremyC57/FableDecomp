// NParticleEngine::CParticleEmitter::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info) -- unless sleeping, (re)submit the emitter's primitive group to the engine (a 2D group with the inverse emitter transform and the start position for 2D emitters, else the 3D particle group with the camera distance, bounding sphere, draw flags and water stage), let every visible particle system prepare its primitives into it, and clear the cleared-particles/decals flags
// Leaf: PreparePrimitives
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEE560, 854 bytes (+ int3 padding), `ret 8`.
// Callees: CEnginePrimitiveParticleGroup(...) 0x00AF08D0, the engine's virtual slot 23 (+0x5c) through info->Engine,
//   CParticleSystem::PreparePrimitives 0x00AF40B0. The 2D group constructor (out of line at 0x00AF0880) and
//   CMatrix3x4::SetInverse are inline here. 1.0f 0x0122DED8.
// Layout: as the Initialise row 0x00AEE240 (Ego_r.pdb CParticleEmitter minus 0x0c); CParticleSystem Visible is bit 5 of
//   +0x50; CEnginePrimitivePrepareInfo engine +0x34 (landed 0x00AEB2D0). CEnginePrimitiveParticleGroup2D (0x4c bytes):
//   type 0x20, layer +0x0c, screen displacement +0x10, z read +0x11, transform +0x14, position +0x44.
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}
};
class C3DVector { public:
    float X, Y, Z;
};
class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
};
class CMatrix3x4 {
public:
    float E11, E12, E13;
    float E21, E22, E23;
    float E31, E32, E33;
    float E41, E42, E43;
    CMatrix3x4() {}
    __forceinline void SetInverse(const CMatrix3x4& m)
    {
        float det = m.E11 * (m.E22 * m.E33 - m.E23 * m.E32) + m.E21 * (m.E32 * m.E13 - m.E33 * m.E12) + m.E31 * (m.E23 * m.E12 - m.E22 * m.E13);
        float inv = 1.0f / det;
        E11 = (m.E22 * m.E33 - m.E23 * m.E32) * inv;
        E21 = (m.E23 * m.E31 - m.E21 * m.E33) * inv;
        E31 = (m.E21 * m.E32 - m.E22 * m.E31) * inv;
        E12 = (m.E32 * m.E13 - m.E33 * m.E12) * inv;
        E22 = (m.E33 * m.E11 - m.E13 * m.E31) * inv;
        E32 = (m.E31 * m.E12 - m.E32 * m.E11) * inv;
        E13 = (m.E23 * m.E12 - m.E22 * m.E13) * inv;
        E23 = (m.E21 * m.E13 - m.E23 * m.E11) * inv;
        E33 = (m.E22 * m.E11 - m.E21 * m.E12) * inv;
        E41 = -(E11 * m.E41 + E21 * m.E42 + E31 * m.E43);
        E42 = -(E12 * m.E41 + E22 * m.E42 + E32 * m.E43);
        E43 = -(E13 * m.E41 + E23 * m.E42 + E33 * m.E43);
    }
};
enum EEngineEffectWaterStage { WATER_STAGE_PRE, WATER_STAGE_WATER, WATER_STAGE_POST };
class CEnginePrimitive { public:
    unsigned long Type;                            // +0x00
    unsigned long Unknown04;                       // +0x04
    bool          Flag08, Flag09, Flag0a;          // +0x08
    CEnginePrimitive(unsigned long type) : Type(type), Unknown04(0), Flag08(false), Flag09(false), Flag0a(false) {}
};
struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    unsigned char _body0c[0x20];
    CEnginePrimitiveParticleGroup(float distance, const C3DBoundingSphere* sphere, bool screenDisplacement, bool readZBuffer, EEngineEffectWaterStage waterStage, bool zBufferWritable, bool clipToWeatherMask, bool enableDithering);   // 0x00AF08D0
};
struct CEnginePrimitiveParticleGroup2D : CEnginePrimitive {
    long       Layer;                              // +0x0c
    bool       ScreenDisplacement;                 // +0x10
    bool       ReadZBuffer;                        // +0x11
    CMatrix3x4 Transform;                          // +0x14
    C2DVector  Position;                           // +0x44
    CEnginePrimitiveParticleGroup2D(long layer, const CMatrix3x4& transform, const C2DVector& position, bool screenDisplacement, bool readZBuffer)
        : CEnginePrimitive(0x20), Layer(layer), ScreenDisplacement(screenDisplacement), ReadZBuffer(readZBuffer), Transform(transform), Position(position) {}   // out of line at 0x00AF0880
};
class CEnginePrimitiveHandle { public:
    void* __vftable;
    void* Data;
};
class CIEngine { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive* primitive, long flags);   // slot 23 (+0x5c)
};
class CEnginePrimitivePrepareInfo { public:
    unsigned char _pad00[0x34];
    CIEngine*     Engine;                          // +0x34
};
namespace NParticleEngine {
struct CParticleUpdateParameters {
    void*         __vftable;                       // +0x00
    unsigned char _maps[0x20];                     // +0x04
};
class CParticleSystem { public:
    unsigned char _state00[0x50];
    bool Initialised : 1;                          // +0x50
    bool Enabled : 1;
    bool Dead : 1;
    bool NormalisedTimeEnabled : 1;
    bool UseAllAttractors : 1;
    bool Visible : 1;
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info, CEnginePrimitiveHandle* group);   // 0x00AF40B0
};
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
template <class T> struct CVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
};
struct C3DPositionInterpolator {
    void*     __vftable;                           // +0x00
    C3DVector StartPosition;                       // +0x04
    C3DVector EndPosition;                         // +0x10
};
class CParticleEmitter {
public:
    void*                       __vftable;             // +0x00
    void*                       m_pIntelligentPointer; // +0x04
    CVector<CParticleSystemPtr> ParticleSystems;       // +0x08
    CParticleUpdateParameters   UpdateParameters;      // +0x14
    CEnginePrimitiveHandle      PrimitiveGroupHandle;  // +0x38
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
    unsigned char               _state46[0x4e];        // +0x46 OldPos, Velocity, OrientationInterpolator
    C3DPositionInterpolator     PositionInterpolator;  // +0x94
    CMatrix3x4                  Transform;             // +0xb0
    CMatrix3x4                  InverseTransform;      // +0xe0
    CMatrix3x4                  EndTransform;          // +0x110
    void*                       Name;                  // +0x140
    C3DBoundingSphere           BSphere;               // +0x144
    float                       DistanceToCam;         // +0x154
    bool IsSleeping() const { return Sleeping; }
    bool IsEmitter2D() const { return Emitter2D; }
    void PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info);
};

void CParticleEmitter::PreparePrimitives(float distance, const CEnginePrimitivePrepareInfo* info)
{
    if (IsSleeping())
        return;
    if (IsEmitter2D()) {
        CMatrix3x4 inverse;
        inverse.SetInverse(Transform);
        info->Engine->AddPrimitive(&PrimitiveGroupHandle,
            &CEnginePrimitiveParticleGroup2D(Emitter2DLayer, inverse,
                C2DVector(PositionInterpolator.StartPosition.X, PositionInterpolator.StartPosition.Y),
                IsScreenDisplacement, ReadZBuffer), 0x42);
    } else {
        EEngineEffectWaterStage stage;
        if (PreWaterEffect)
            stage = WATER_STAGE_PRE;
        else
            stage = WaterEffect ? WATER_STAGE_WATER : WATER_STAGE_POST;
        info->Engine->AddPrimitive(&PrimitiveGroupHandle,
            &CEnginePrimitiveParticleGroup(DistanceToCam, &BSphere, IsScreenDisplacement, ReadZBuffer, stage,
                ZBufferWriteable, ClipEffectToWeatherMask, EnableDithering), 2);
    }
    for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it) {
        CParticleSystem* system = it->Data;
        if (system->Visible)
            system->PreparePrimitives(distance, info, &PrimitiveGroupHandle);
    }
    ClearedParticles = false;
    ClearedDecals = false;
}
}