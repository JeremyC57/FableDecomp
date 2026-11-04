#include <stdio.h>
#include <string.h>
// Behaviour test for CParticleEmitterManager::CreateDecalEmitter (0x00AEABE0).
class C2DVector { public: float X, Y; };
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};
class CRightHandedSet { public:
    CRightHandedSet(const C3DVector& u, const C3DVector& f) : Up(u), Forward(f) {}
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

class CBaseClass {                                 // polymorphic root of the interpolators
public:
    virtual void v00() {}
    CBaseClass() {}
    ~CBaseClass() {}
};
class C3DPositionInterpolator : public CBaseClass { public:      // 0x1c bytes
    C3DVector Start;                               // +0x04
    C3DVector End;                                 // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end) : Start(start), End(end) {}
};
class C3DOrientationInterpolator : public CBaseClass { public:   // 0x34 bytes
    CRightHandedSet Start;                         // +0x04
    CRightHandedSet End;                           // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end) : Start(start), End(end) {}
};

namespace NParticleEngine {
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
    bool spawn; float posX, upX; CParticleEmitter* clone;
    bool ShouldSpawn(const C3DVector&) { return spawn; }
    CParticleEmitter* DatabaseClone() { return clone; }
    void SetPosition(const C3DPositionInterpolator& p) { posX = p.End.X; }
    void SetOrientation(const C3DOrientationInterpolator& o) { upX = o.End.Up.X; }
};
}
typedef NParticleEngine::CParticleEmitter CEmitter;

struct SCountBlock;

struct CParticleEmitterDatabase {
    CEmitter* tmpl; long h, i;
    CEmitter* GetEmitterTemplate(long handle, long index) { h = handle; i = index; return tmpl; }
};
struct CEmitterDatabasePointer {                   // static CCountedPointer<CParticleEmitterDatabase>
    CParticleEmitterDatabase* Object;
    SCountBlock*              Block;
    CParticleEmitterDatabase* operator->() const { return Object; }
};
CEmitterDatabasePointer g_ParticleEmitterDatabase;

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

int main()
{
    int ok = 1;
    CParticleEmitterDatabase db; g_ParticleEmitterDatabase.Object = &db; db.tmpl = 0;
    CParticleEmitterManager mgr; C3DVector p(1.0f, 2.0f, 3.0f); CRightHandedSet o(C3DVector(4.0f, 0, 0), C3DVector(0, 1, 0));
    if (mgr.CreateDecalEmitter(7, p, 2, o) != 0 || db.h != 7 || db.i != 2) ok = 0;
    CEmitter t, c; memset(&t, 0, sizeof(t)); memset(&c, 0, sizeof(c)); t.clone = &c; db.tmpl = &t;
    t.spawn = false; if (mgr.CreateDecalEmitter(1, p, 0, o) != 0) ok = 0;
    t.spawn = true; t.Emitter2D = 1; if (mgr.CreateDecalEmitter(1, p, 0, o) != 0) ok = 0;
    t.Emitter2D = 0;
    if (mgr.CreateDecalEmitter(1, p, 0, o) != &c || c.posX != 1.0f || c.upX != 4.0f) ok = 0;
    if (ok) { printf("00aeabe0_TEST PASS\n"); return 0; }
    printf("00aeabe0_TEST FAIL\n"); return 1;
}