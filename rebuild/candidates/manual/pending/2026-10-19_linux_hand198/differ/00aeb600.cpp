// Grade: DIFFER(202v202) -- register allocation only: retail loads the clone straight into esi and the interpolate argument into ecx, ours loads the clone into ecx then copies it to esi (argument in edx); tried result/created locals
// CParticleEmitterManager::CreateInterpolatedEmitter(CParticleEmitter*, CParticleEmitter*, float, const C3DVector&, const CRightHandedSet&) -- when both templates exist and may spawn here, clone the first into the active list, interpolate it towards the second and place it; null otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB600, length 208 bytes (`ret 14h`).
// Leaf: ?CreateInterpolatedEmitter@CParticleEmitterManager@@QAEPAUCParticleEmitter@NParticleEngine@@PAU23@0M
// Callees: CParticleEmitter::ShouldSpawn @ 0x00AECF90 (x2), DatabaseClone @ 0x00AF0650, InterpolateTo @ 0x00AED600,
//          SetPosition @ 0x00AEDDC0, SetOrientation @ 0x00AEDE60, CCountedPointer<CParticleEmitter>(CParticleEmitter*)
//          @ 0x00AE9450 and its destructor @ 0x00AE94A0, CParticleEmitterList::Insert @ 0x00AEB060,
//          C3DPositionInterpolator ctor @ 0x006AF570, C3DOrientationInterpolator ctor @ 0x005508D0, ~CBaseClass @ 0x0099A2E0.
// Shape: the overload the landed 0x00AEB850 forwards to (the same parameter list); `this` is homed in a frame slot
// for Insert; the new emitter is read from the counted-pointer local once, right after Insert succeeds.
struct C2DVector { float X, Y; };
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
struct CRightHandedSet {
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

class CBaseClass {                                 // polymorphic root of the interpolators
public:
    virtual void v00();
    CBaseClass();                                  // 0x0099A2D0
    ~CBaseClass();                                 // 0x0099A2E0
};
struct C3DPositionInterpolator : CBaseClass {      // 0x1c bytes
    C3DVector Start;                               // +0x04
    C3DVector End;                                 // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);                  // 0x006AF570
};
struct C3DOrientationInterpolator : CBaseClass {   // 0x34 bytes
    CRightHandedSet Start;                         // +0x04
    CRightHandedSet End;                           // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
};

namespace NParticleEngine {
// CParticleEmitter at retail offsets: the ushort flag word at +0x40 (Emitter2D is bit 11).
struct CParticleEmitter {
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
struct CEmitterPointer {                           // CCountedPointer<CParticleEmitter>
    CEmitter*    Object;
    SCountBlock* Block;
    CEmitterPointer(CEmitter* object);             // 0x00AE9450
    ~CEmitterPointer();                            // 0x00AE94A0
    CEmitter* operator->() const { return Object; }
};

struct CParticleEmitterList {
    unsigned char _list[4];                        // +0x00 STLport list<CEmitterPointer>
    long          MaxActiveEmitters;               // +0x04
    bool Insert(CEmitterPointer& emitter);         // 0x00AEB060
};

struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;           // +0x0000
    CEmitter* CreateInterpolatedEmitter(CEmitter* from, CEmitter* to, float interpolate,
                                        const C3DVector& position, const CRightHandedSet& orientation);
};

CEmitter* CParticleEmitterManager::CreateInterpolatedEmitter(CEmitter* from, CEmitter* to, float interpolate,
                                                             const C3DVector& position, const CRightHandedSet& orientation)
{
    CEmitter* result = 0;
    if (from && from->ShouldSpawn(position) && to && to->ShouldSpawn(position)) {
        CEmitterPointer emitter(from->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            CEmitter* created = emitter.Object;
            created->InterpolateTo(to, interpolate);
            created->SetPosition(C3DPositionInterpolator(position, position));
            created->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
            result = created;
        }
    }
    return result;
}
