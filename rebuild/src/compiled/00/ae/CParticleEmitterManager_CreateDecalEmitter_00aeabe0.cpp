// CParticleEmitterManager::CreateDecalEmitter(long, const C3DVector&, long, const CRightHandedSet&) -- fetch the emitter template; if it may spawn here and is not a 2D emitter, return a placed clone that is not added to the active list; null otherwise
// Grade: RELOCATION_MATCH -- a zero-initialised result local assigned the clone inside one `t && t->ShouldSpawn(p) && !t->Is2D()` test and returned once: the zero then stays in ebx and both failure exits keep their own `mov eax,ebx` tails in retail order (the early `return none` forms folded the constant)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEABE0, length 176 bytes (`ret 10h`).
// Callees: CParticleEmitterDatabase::GetEmitterTemplate @ 0x00AE89E0 on the static counted pointer 0x013D2E14,
//          CParticleEmitter::ShouldSpawn @ 0x00AECF90, DatabaseClone @ 0x00AF0650, SetPosition @ 0x00AEDDC0,
//          SetOrientation @ 0x00AEDE60, C3DPositionInterpolator ctor @ 0x006AF570, C3DOrientationInterpolator ctor
//          @ 0x005508D0, ~CBaseClass @ 0x0099A2E0 (each temporary).
// Shape: the caller owns the clone, so there is no counted pointer; the null template returns through its own
// epilogue before edi is saved.
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet { public:
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

class CBaseClass {                                 // polymorphic root of the interpolators
public:
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                                 // 0x0099A2E0
};
class C3DPositionInterpolator : public CBaseClass { public:      // 0x1c bytes
    C3DVector Start;                               // +0x04
    C3DVector End;                                 // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);                  // 0x006AF570
};
class C3DOrientationInterpolator : public CBaseClass { public:   // 0x34 bytes
    CRightHandedSet Start;                         // +0x04
    CRightHandedSet End;                           // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
};

namespace NParticleEngine {
// CParticleEmitter at retail offsets: the ushort flag word at +0x40 (Emitter2D is bit 11).
class CParticleEmitter { public:
    void*          m_vtable;                       // +0x00
    unsigned char  m_base04[0x3c];                 // +0x04 .. +0x3f
    unsigned short Priority : 3;                   // +0x40
    unsigned short IsScreenDisplacement : 1;
    unsigned short ReadZBuffer : 1;
    unsigned short Dead : 1;
    unsigned short ParticleCreationDisabled : 1;
    unsigned short ContinuousEmitter : 1;
    unsigned short Initialised : 1;
    unsigned short WaitingToSleep : 1;
    unsigned short Sleeping : 1;
    unsigned short Emitter2D : 1;                  // bit 11
    bool Is2D() const { return Emitter2D; }
    bool ShouldSpawn(const C3DVector& position);                                    // 0x00AECF90
    CParticleEmitter* DatabaseClone();                                              // 0x00AF0650
    void InterpolateTo(CParticleEmitter* target, float interpolate);                // 0x00AED600
    void Set2DPosition(const C2DVector* position, EEngine2DLayer layer);            // 0x00AED090
    void SetPosition(const C3DPositionInterpolator& position);                      // 0x00AEDDC0
    void SetOrientation(const C3DOrientationInterpolator& orientation);             // 0x00AEDE60
};
}
typedef NParticleEngine::CParticleEmitter CEmitter;

struct SCountBlock;

struct CParticleEmitterDatabase {
    CEmitter* GetEmitterTemplate(long handle, long index);   // 0x00AE89E0
};
struct CEmitterDatabasePointer {                   // static CCountedPointer<CParticleEmitterDatabase>
    CParticleEmitterDatabase* Object;
    SCountBlock*              Block;
    CParticleEmitterDatabase* operator->() const { return Object; }
};
extern CEmitterDatabasePointer g_ParticleEmitterDatabase;   // 0x013D2E14

struct CParticleEmitterManager {
    CEmitter* CreateDecalEmitter(long handle, const C3DVector& position, long flag, const CRightHandedSet& orientation);
};

CEmitter* CParticleEmitterManager::CreateDecalEmitter(long handle, const C3DVector& position, long flag, const CRightHandedSet& orientation)
{
    CEmitter* emitterTemplate = g_ParticleEmitterDatabase->GetEmitterTemplate(handle, flag);
    CEmitter* result = 0;
    if (emitterTemplate && emitterTemplate->ShouldSpawn(position) && !emitterTemplate->Is2D()) {
        result = emitterTemplate->DatabaseClone();
        result->SetPosition(C3DPositionInterpolator(position, position));
        result->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
    }
    return result;
}