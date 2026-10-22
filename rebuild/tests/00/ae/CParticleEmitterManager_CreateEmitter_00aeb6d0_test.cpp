#include <stdio.h>
#include <string.h>
// CParticleEmitterManager::CreateEmitter(long, const C3DVector&, long, const CRightHandedSet&) -- fetch the emitter template; if it may spawn here and is not a 2D emitter, clone it into the active list and place it (position and orientation interpolators from/to the same value); null when anything fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB6D0, length 192 bytes (`ret 10h`).
// Callees: CParticleEmitterDatabase::GetEmitterTemplate @ 0x00AE89E0 on the static counted pointer 0x013D2E14,
//          CParticleEmitter::ShouldSpawn @ 0x00AECF90, DatabaseClone @ 0x00AF0650, SetPosition @ 0x00AEDDC0,
//          SetOrientation @ 0x00AEDE60, CCountedPointer<CParticleEmitter>(CParticleEmitter*) @ 0x00AE9450 and its destructor
//          @ 0x00AE94A0 (labels propagated), CParticleEmitterList::Insert @ 0x00AEB060, C3DPositionInterpolator ctor
//          @ 0x006AF570, C3DOrientationInterpolator ctor @ 0x005508D0, ~CBaseClass @ 0x0099A2E0 (each temporary).
// Shape: the manager starts with its CParticleEmitterList, so Insert takes `this`; the clone is held in a counted-
// pointer local (out-of-line ctor/dtor) that only lives inside the spawn test; the template flag is forwarded
// unchanged (a long, as the landed GetEmitterTemplate declaration).
// The three tests are one && chain (nested ifs duplicate the null-template epilogue). The counted pointer's ctor/dtor
// and Insert have same-TU read-only stand-in bodies, so the local does not escape: the clone is read through
// operator-> after the position interpolator temporary is built and kept in esi for both setters and the result.
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

// Same-TU read-only stand-in bodies (the real ones are out of line in retail): they take the counted pointer's fields
// by value, so VC7.1 knows the local's address does not escape and keeps the emitter in a register after the first
// operator-> (loaded after the interpolator temporary, as retail).
SCountBlock* MakeBlock_CEmitterPointer_0(CEmitter* object);
void ReleaseBlock_CEmitterPointer_0(CEmitter* object, SCountBlock* block);
bool InsertEmitter_CParticleEmitterList_0(CParticleEmitterList* list, CEmitter* object, SCountBlock* block);
#pragma auto_inline(off)
__declspec(noinline) CEmitterPointer::CEmitterPointer(CEmitter* object)
{
    Object = object;
    Block = MakeBlock_CEmitterPointer_0(object);
}
__declspec(noinline) CEmitterPointer::~CEmitterPointer()
{
    ReleaseBlock_CEmitterPointer_0(Object, Block);
}
__declspec(noinline) bool CParticleEmitterList::Insert(CEmitterPointer& emitter)
{
    return InsertEmitter_CParticleEmitterList_0(this, emitter.Object, emitter.Block);
}
#pragma auto_inline(on)

struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;           // +0x0000
    CEmitter* CreateEmitter(long handle, const C3DVector& position, long flag, const CRightHandedSet& orientation);
};

CEmitter* CParticleEmitterManager::CreateEmitter(long handle, const C3DVector& position, long flag, const CRightHandedSet& orientation)
{
    CEmitter* result;
    CEmitter* emitterTemplate = g_ParticleEmitterDatabase->GetEmitterTemplate(handle, flag);
    result = 0;
    if (emitterTemplate && emitterTemplate->ShouldSpawn(position) && !emitterTemplate->Is2D()) {
        CEmitterPointer emitter(emitterTemplate->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            emitter->SetPosition(C3DPositionInterpolator(position, position));
            emitter->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
            result = emitter.Object;
        }
    }
    return result;
}

// ---- test stubs ----
using namespace NParticleEngine;
static char g_log[512];
static void logs(const char* t) { strcat(g_log, t); strcat(g_log, ";"); }
void CBaseClass::v00() { }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { logs("~interp"); }
C3DPositionInterpolator::C3DPositionInterpolator(const C3DVector& start, const C3DVector& end) : Start(start), End(end) { logs("posinterp"); }
C3DOrientationInterpolator::C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end) : Start(start), End(end) { logs("orinterp"); }
static CParticleEmitter g_template, g_clone; static int g_spawn = 1;
static float g_posX; static float g_orUpZ;
bool CParticleEmitter::ShouldSpawn(const C3DVector& position) { g_posX = position.X; logs("spawn?"); return g_spawn != 0; }
CParticleEmitter* CParticleEmitter::DatabaseClone() { logs(this == &g_template ? "clone" : "clone?"); return &g_clone; }
void CParticleEmitter::InterpolateTo(CParticleEmitter*, float) { }
static float g_setPosX, g_setPosEndX;
void CParticleEmitter::SetPosition(const C3DPositionInterpolator& p) { g_setPosX = p.Start.X; g_setPosEndX = p.End.X; logs(this == &g_clone ? "setpos" : "setpos?"); }
void CParticleEmitter::SetOrientation(const C3DOrientationInterpolator& o) { g_orUpZ = o.End.Up.Z; logs(this == &g_clone ? "setor" : "setor?"); }
static CEmitter* g_returnTemplate; static long g_tHandle, g_tFlag;
CEmitter* CParticleEmitterDatabase::GetEmitterTemplate(long handle, long index) { g_tHandle = handle; g_tFlag = index; return g_returnTemplate; }
CEmitterDatabasePointer g_ParticleEmitterDatabase;
static int g_blocks;
SCountBlock* MakeBlock_CEmitterPointer_0(CEmitter* object) { g_blocks++; logs("ptr"); return (SCountBlock*)object; }
void ReleaseBlock_CEmitterPointer_0(CEmitter* object, SCountBlock* block) { g_blocks--; logs(object == &g_clone && block == (SCountBlock*)&g_clone ? "~ptr" : "~ptr?"); }
static int g_insertOk = 1; static CParticleEmitterList* g_insertList;
bool InsertEmitter_CParticleEmitterList_0(CParticleEmitterList* list, CEmitter* object, SCountBlock*) { g_insertList = list; logs(object == &g_clone ? "insert" : "insert?"); return g_insertOk != 0; }
static int fails;
static void check(int ok, const char* what) { if (!ok) { fails++; printf("FAIL %s log=%s\n", what, g_log); } }
int main()
{
    CParticleEmitterManager mgr; memset(&mgr, 0, sizeof mgr);
    float pv[3] = { 3.0f, 4.0f, 5.0f }; const C3DVector& pos = *(const C3DVector*)pv;
    float rv[6] = { 0, 0, 1, 0, 1, 0 }; const CRightHandedSet& rhs = *(const CRightHandedSet*)rv;
    g_ParticleEmitterDatabase.Object = (CParticleEmitterDatabase*)&g_tHandle;
    // 1: no template
    g_log[0] = 0; g_returnTemplate = 0;
    check(mgr.CreateEmitter(11, pos, 2, rhs) == 0 && g_log[0] == 0 && g_tHandle == 11 && g_tFlag == 2, "notemplate");
    // 2: full success
    g_log[0] = 0; g_returnTemplate = &g_template; g_spawn = 1; g_template.Emitter2D = 0; g_insertOk = 1;
    CEmitter* r = mgr.CreateEmitter(1, pos, 0, rhs);
    check(r == &g_clone, "result");
    check(strcmp(g_log, "spawn?;clone;ptr;insert;posinterp;setpos;~interp;orinterp;setor;~interp;~ptr;") == 0, "order");
    check(g_posX == 3.0f && g_setPosX == 3.0f && g_setPosEndX == 3.0f && g_orUpZ == 1.0f && g_insertList == &mgr.ActiveEmitters && g_blocks == 0, "values");
    // 3: should not spawn
    g_log[0] = 0; g_spawn = 0; check(mgr.CreateEmitter(1, pos, 0, rhs) == 0 && strcmp(g_log, "spawn?;") == 0, "nospawn");
    // 4: 2D template
    g_log[0] = 0; g_spawn = 1; g_template.Emitter2D = 1; check(mgr.CreateEmitter(1, pos, 0, rhs) == 0 && strcmp(g_log, "spawn?;") == 0, "is2d");
    // 5: insert fails -> pointer still released, null result
    g_log[0] = 0; g_template.Emitter2D = 0; g_insertOk = 0;
    check(mgr.CreateEmitter(1, pos, 0, rhs) == 0 && strcmp(g_log, "spawn?;clone;ptr;insert;~ptr;") == 0 && g_blocks == 0, "insertfail");
    if (!fails) printf("00aeb6d0_TEST PASS\n");
    return fails;
}