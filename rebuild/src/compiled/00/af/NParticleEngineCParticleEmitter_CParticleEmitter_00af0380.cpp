// NParticleEngine::CParticleEmitter::CParticleEmitter() -- default emitter: empty particle-system list and update parameters, null primitive handle, identity transforms, default orientation and zero position interpolation, the default spawn/draw distances and fades, and its flags (ReadZBuffer, DieIfOffscreen, EnableDithering, ClearedParticles, ClearedDecals set; the rest clear, layer 74)
// Leaf: CParticleEmitter
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AF0380, 656 bytes, plain `ret` (returns this).
// Callees: CBaseObject() 0x0099A380, CBaseClass() 0x0099A2D0, CCharString() 0x0099E4B0, CRightHandedSet::SetDefault 0x00AF07A0,
//   the default-distance getters 0x00AEAA60 / 0x00AEAA50 / 0x00AEAA20 / 0x00AEAA30 / 0x00AEAA40 (static, float in st0).
// Layout: Ego_r.pdb CParticleEmitter minus 0x0c (retail STLport 12-byte vectors, 16-byte CVectorMaps):
//   ParticleSystems +0x08, UpdateParameters +0x14 (vtable 0x0125FFDC), PrimitiveGroupHandle +0x38 (vtable 0x0122F598),
//   flag words +0x40 / +0x42 and bytes +0x44 / +0x45, OldPos +0x46, Velocity +0x52, OrientationInterpolator +0x60 (vtable
//   0x01239824), PositionInterpolator +0x94 (vtable 0x0123981C), Transform +0xb0, InverseTransform +0xe0, EndTransform
//   +0x110, Name +0x140, BSphere +0x144, distances +0x154..+0x168. The emitter's own vtable is 0x0129F348.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CMatrix3x4 { public:
    C3DVector Right, Up, Forward, Translation;
    __forceinline void SetIdentity()
    {
        Right.Y = Right.Z = Up.X = Up.Z = Forward.X = Forward.Y = Translation.X = Translation.Y = Translation.Z = 0.0f;
        Right.X = Up.Y = Forward.Z = 1.0f;
    }
};
class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    void SetDefault();                                 // 0x00AF07A0
};
class C3DBoundingSphere { public:
    C3DVector Center;
    float     Radius;
    C3DBoundingSphere() : Center(0.0f, 0.0f, 0.0f), Radius(0.0f) {}
};
class CCharString { public:
    void* PStringData;
    CCharString();                                     // 0x0099E4B0
};
class CBaseClass {
public:
    CBaseClass();                                           // 0x0099A2D0
    virtual ~CBaseClass();
};
class CBaseObject { public:
    void* m_pIntelligentPointer;
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
template <class T> struct CAllocProxy {               // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* _M_data;
    CAllocProxy(T* p) : _M_data(p) {}
};
template <class T> struct CVector {                    // STLport vector, 12 bytes
    T* _M_start; T* _M_finish; CAllocProxy<T> _M_end_of_storage;
    CVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
template <class K, class V> class CVectorMap { public:        // retail 16 bytes
    CVector<V> Data;
    char       Compare;                                // empty comparator
    bool       Dirty;                                  // +0x0d
    CVectorMap() : Dirty(false) {}
};
namespace NParticleEngine {
class CParticleSystem;
struct CParticleSystemPtr { CParticleSystem* Data; void* Info; };
struct CParticleUpdateParamDesc { unsigned long a, b; };
struct CParticleUpdateParameters : CBaseClass {             // vtable 0x0125FFDC, retail 0x24 bytes
    CVectorMap<unsigned long, C3DVector>                VectorParams;   // +0x04
    CVectorMap<unsigned long, CParticleUpdateParamDesc> UpdateParams;   // +0x14
};
}
class CEnginePersistentPrimitiveBase;
class CEnginePrimitiveHandle : public CBaseClass { public:                // vtable 0x0122F598
    CEnginePersistentPrimitiveBase* Data;              // +0x04 CIVCountedPointer
    CEnginePrimitiveHandle() : Data(0) {}
};
class C3DOrientationInterpolator : public CBaseClass { public:            // vtable 0x01239824
    CRightHandedSet StartOrientation;                  // +0x04
    CRightHandedSet EndOrientation;                    // +0x1c
};
class C3DPositionInterpolator : public CBaseClass { public:               // vtable 0x0123981C
    C3DVector StartPosition;                           // +0x04
    C3DVector EndPosition;                             // +0x10
};
namespace NParticleEngine {
class CParticleEmitter : public CBaseObject {
public:
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
#pragma pack(push, 2)
    struct { C3DVector OldPos; C3DVector Velocity; } Motion;   // +0x46 OldPos, +0x52 Velocity
#pragma pack(pop)
    C3DOrientationInterpolator OrientationInterpolator;        // +0x60
    C3DPositionInterpolator    PositionInterpolator;           // +0x94
    CMatrix3x4                 Transform;                      // +0xb0
    CMatrix3x4                 InverseTransform;               // +0xe0
    CMatrix3x4                 EndTransform;                   // +0x110
    CCharString                Name;                           // +0x140
    C3DBoundingSphere          BSphere;                        // +0x144
    float                      DistanceToCam;                  // +0x154
    float                      MaxSpawnDistance;               // +0x158
    float                      MaxDrawDistance;                // +0x15c
    float                      FadeOutStart;                   // +0x160
    float                      FadeInStart;                    // +0x164
    float                      FadeInEnd;                      // +0x168
    static float GetDefaultMaxSpawnDistance();                 // 0x00AEAA60
    static float GetDefaultMaxDrawDistance();                  // 0x00AEAA50
    static float GetDefaultFadeOutStart();                     // 0x00AEAA20
    static float GetDefaultFadeInEnd();                        // 0x00AEAA30
    static float GetDefaultFadeInStart();                      // 0x00AEAA40
    CParticleEmitter();
    virtual void e001();
};

CParticleEmitter::CParticleEmitter()
{
    IsScreenDisplacement = false;
    ReadZBuffer = true;
    Dead = false;
    ParticleCreationDisabled = false;
    WaitingToSleep = false;
    Sleeping = false;
    Transform.SetIdentity();
    InverseTransform.SetIdentity();
    EndTransform.SetIdentity();
    Emitter2DLayer = 74;
    ContinuousEmitter = false;
    Emitter2D = false;
    PreWaterEffect = false;
    WaterEffect = false;
    ZBufferWriteable = false;
    AlwaysUpdate = false;
    DieIfOffsceen = true;
    OffscreenUpdate = false;
    ClipEffectToWeatherMask = false;
    EnableDithering = true;
    CalcBoundingSphereOnceOnly = false;
    BoundingSphereValid = false;
    MaxSpawnDistance = GetDefaultMaxSpawnDistance();
    MaxDrawDistance = GetDefaultMaxDrawDistance();
    FadeOutStart = GetDefaultFadeOutStart();
    FadeInEnd = GetDefaultFadeInEnd();
    FadeInStart = GetDefaultFadeInStart();
    Priority = 0;
    Initialised = false;
    OrientationInterpolator.StartOrientation.SetDefault();
    OrientationInterpolator.EndOrientation.SetDefault();
    PositionInterpolator.StartPosition = C3DVector(0.0f, 0.0f, 0.0f);
    PositionInterpolator.EndPosition = C3DVector(0.0f, 0.0f, 0.0f);
    Motion.Velocity = C3DVector(0.0f, 0.0f, 0.0f);
    Motion.OldPos = C3DVector(0.0f, 0.0f, 0.0f);
    Visible = false;
    ClearedParticles = true;
    ClearedDecals = true;
}
}