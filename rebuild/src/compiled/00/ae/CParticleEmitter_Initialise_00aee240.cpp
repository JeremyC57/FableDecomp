// NParticleEngine::CParticleEmitter::Initialise(CIEngine* engine) -- create the emitter's engine primitive group once: a 2D group (layer, the inverse of the emitter transform, the start position's X/Y, screen displacement, z-buffer read) for 2D emitters, else a 3D particle group (camera distance, bounding sphere, the draw flags and the water stage); then initialise every particle system against that group, the emitter staying initialised only if all of them succeed
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEE240, 791 bytes (+ int3 padding), `ret 4`.
// Callees: CEnginePrimitiveParticleGroup2D(long, const CMatrix3x4&, const C2DVector&, bool, bool) 0x00AF0880 (label
//   "C3DMeshFileBoneChunk::SetLocalizationMatrix" is wrong), CEnginePrimitiveParticleGroup(float, const C3DBoundingSphere*,
//   bool, bool, EEngineEffectWaterStage, bool, bool, bool) 0x00AF08D0, the engine's virtual slot 23 (+0x5c, AddPrimitive
//   group), CParticleSystem::Initialise 0x00AF4010. 1.0f 0x0122DED8; CMatrix3x4::SetInverse is inline.
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (landed constructor 0x00AF0380): ParticleSystems +0x08, UpdateParameters
//   +0x14, PrimitiveGroupHandle +0x38, flag word +0x40, flag bytes +0x44/+0x45, PositionInterpolator +0x94 (StartPosition
//   +0x98), Transform +0xb0, BSphere +0x144, DistanceToCam +0x154.
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) {}
};
struct C3DVector {
    float X, Y, Z;
};
struct C3DBoundingSphere {
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
struct CEnginePrimitive {
    unsigned long Type;                            // +0x00
    unsigned long _pad04;
    unsigned char _flags08[4];
};
struct CEnginePrimitiveParticleGroup : CEnginePrimitive {
    unsigned char _body0c[0x20];
    CEnginePrimitiveParticleGroup(float distance, const C3DBoundingSphere* sphere, bool screenDisplacement, bool readZBuffer, EEngineEffectWaterStage waterStage, bool zBufferWritable, bool clipToWeatherMask, bool enableDithering);   // 0x00AF08D0
};
struct CEnginePrimitiveParticleGroup2D : CEnginePrimitive {
    unsigned char _body0c[0x40];
    CEnginePrimitiveParticleGroup2D(long layer, const CMatrix3x4& transform, const C2DVector& position, bool screenDisplacement, bool readZBuffer);   // 0x00AF0880
};
struct CEnginePrimitiveHandle {
    void* __vftable;
    void* Data;
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void AddPrimitive(CEnginePrimitiveHandle* handle, const CEnginePrimitive* primitive, long flags);   // slot 23 (+0x5c)
};
namespace NParticleEngine {
struct CParticleUpdateParameters {
    void*         __vftable;                       // +0x00
    unsigned char _maps[0x20];                     // +0x04
};
struct CParticleSystem {
    bool Initialise(CIEngine* engine, CParticleUpdateParameters* params, CEnginePrimitiveHandle* group);   // 0x00AF4010
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
    bool Initialise(CIEngine* engine);
};

bool CParticleEmitter::Initialise(CIEngine* engine)
{
    float u0 = 0.0f;
    float u1 = 0.0f;
    float u2 = 0.0f;
    float u3 = 0.0f;
    float u4 = 0.0f;
    if (!Initialised) {
    if (engine) {
        if (Emitter2D) {
            CMatrix3x4 inverse;
            inverse.SetInverse(Transform);
            engine->AddPrimitive(&PrimitiveGroupHandle,
                &CEnginePrimitiveParticleGroup2D(Emitter2DLayer, inverse,
                    C2DVector(PositionInterpolator.StartPosition.X, PositionInterpolator.StartPosition.Y),
                    IsScreenDisplacement, ReadZBuffer), 0x42);
        } else {
            EEngineEffectWaterStage stage;
            if (PreWaterEffect)
                stage = WATER_STAGE_PRE;
            else
                stage = WaterEffect ? WATER_STAGE_WATER : WATER_STAGE_POST;
            engine->AddPrimitive(&PrimitiveGroupHandle,
                &CEnginePrimitiveParticleGroup(DistanceToCam, &BSphere, IsScreenDisplacement, ReadZBuffer, stage,
                    ZBufferWriteable, ClipEffectToWeatherMask, EnableDithering), 2);
        }
        Initialised = true;
    }
    for (CParticleSystemPtr* it = ParticleSystems._M_start; it != ParticleSystems._M_finish; ++it)
        Initialised &= it->Data->Initialise(engine, &UpdateParameters, &PrimitiveGroupHandle);
    }
    return Initialised;
}
}