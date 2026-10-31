// NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(const CParticleEmitter* from, const CParticleEmitter* to, float interpolate, const C3DVector& position, const CRightHandedSet& orientation) -- when both templates exist and may spawn here, clone the first into the active list, interpolate it towards the second and place it; null otherwise
// Grade: DIFFER(202v202) -- register allocation only: after Insert retail loads the clone straight into esi (the result) and the interpolate argument into ecx (`mov ecx,[esp+5Ch] ; mov esi,[esp+14h] ; push ecx ; push ebx ; mov ecx,esi`); ours loads the clone into ecx, the argument into edx and copies ecx to esi
// Leaf: ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@PBV32@0MABVC3DVector@@ABVCRightHandedSet@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB600, 202 bytes (`ret 14h`).
// Callees: CParticleEmitter::ShouldSpawn(const C3DVector&) const 0x00AECF90 (x2), DatabaseClone() const 0x00AF0650,
//   InterpolateTo(const CParticleEmitter&, float) 0x00AED600, SetPosition 0x00AEDDC0, SetOrientation 0x00AEDE60,
//   CCountedPointer<CParticleEmitter>(CParticleEmitter*) 0x00AE9450 and ~CCountedPointer 0x00AE94A0 (out of line, from the
//   shared engine/CCountedPointer.h), CParticleEmitterList::Insert(CCountedPointer<CParticleEmitter>&) 0x00AEB060,
//   C3DPositionInterpolator ctor 0x006AF570, C3DOrientationInterpolator ctor 0x005508D0, ~CBaseClass 0x0099A2E0.
// Names (chunk 631): Ego_r.pdb spellings: the manager and the list are in NParticleEngine, the two templates are const
//   pointers, ShouldSpawn / DatabaseClone are const and InterpolateTo takes a const reference (PBV32@0M / QBE / ABV12@M).
// Tried: the PDB signatures, the shared CCountedPointer header, `created` local vs result assigned directly, a float
//   local for interpolate (all the same residue). Earlier: CBaseClass header (neutral), the sibling's emitter-> calls (216).
// Next: lane gap: register preference of the reloaded counted-pointer Data (this register first vs the result variable).
#include "engine/CCountedPointer.h"
class C2DVector { public: float X, Y; };
class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) {}
};
class CRightHandedSet {
public:
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

class CBaseClass {                                 // polymorphic root of the interpolators
public:
    CBaseClass();                                  // 0x0099A2D0
    virtual ~CBaseClass();                         // 0x0099A2E0 (PDB ??1CBaseClass@@UAE@XZ)
};
class C3DPositionInterpolator : public CBaseClass {
public:      // 0x1c bytes
    C3DVector Start;                               // +0x04
    C3DVector End;                                 // +0x10
    C3DPositionInterpolator(const C3DVector& start, const C3DVector& end);                  // 0x006AF570
};
class C3DOrientationInterpolator : public CBaseClass {
public:   // 0x34 bytes
    CRightHandedSet Start;                         // +0x04
    CRightHandedSet End;                           // +0x1c
    C3DOrientationInterpolator(const CRightHandedSet& start, const CRightHandedSet& end);   // 0x005508D0
};

namespace NParticleEngine {
// CParticleEmitter at retail offsets: the ushort flag word at +0x40 (Emitter2D is bit 11).
class CParticleEmitter {
public:
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
    bool ShouldSpawn(const C3DVector& position) const;                                    // 0x00AECF90
    CParticleEmitter* DatabaseClone() const;                                              // 0x00AF0650
    void InterpolateTo(const CParticleEmitter& target, float interpolate);                // 0x00AED600
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
typedef CCountedPointer<CEmitter> CEmitterPointer;

namespace NParticleEngine {
class CParticleEmitterList {
public:
    unsigned char _list[4];                        // +0x00 STLport list<CEmitterPointer>
    long          MaxActiveEmitters;               // +0x04
    bool Insert(CEmitterPointer& emitter);         // 0x00AEB060
};
}
using NParticleEngine::CParticleEmitterList;

namespace NParticleEngine {
class CParticleEmitterManager {
public:
    CParticleEmitterList ActiveEmitters;           // +0x0000
    CEmitter* CreateInterpolatedEmitter(const CEmitter* from, const CEmitter* to, float interpolate,
                                        const C3DVector& position, const CRightHandedSet& orientation);
};
}

CEmitter* NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(const CEmitter* from, const CEmitter* to, float interpolate,
                                                             const C3DVector& position, const CRightHandedSet& orientation)
{
    CEmitter* result = 0;
    if (from && from->ShouldSpawn(position) && to && to->ShouldSpawn(position)) {
        CEmitterPointer emitter(from->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            result = emitter.Data;
            result->InterpolateTo(*to, interpolate);
            result->SetPosition(C3DPositionInterpolator(position, position));
            result->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
        }
    }
    return result;
}
