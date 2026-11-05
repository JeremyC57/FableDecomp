// NParticleEngine::CParticleEmitterManager::CreateInterpolatedEmitter(const CParticleEmitter* from, const CParticleEmitter* to, float interpolate, const C3DVector& position, const CRightHandedSet& orientation) -- when both emitters may spawn here, clone the source into the active list, interpolate it toward the target and place it; null otherwise
// Leaf: ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@PBV32@0MABVC3DVector@@ABVCRightHandedSet@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEB600, 202 bytes (`ret 14h`). Ego_r.pdb
//   ?CreateInterpolatedEmitter@CParticleEmitterManager@NParticleEngine@@QAEPAVCParticleEmitter@2@PBV32@0MABVC3DVector@@ABVCRightHandedSet@@@Z.
// Callees: CParticleEmitter::ShouldSpawn const 0x00AECF90 (x2), DatabaseClone const 0x00AF0650, InterpolateTo(const
//   CParticleEmitter&, float) 0x00AED600 (Ego_r.pdb ?InterpolateTo@CParticleEmitter@NParticleEngine@@QAEXABV12@M@Z), SetPosition
//   0x00AEDDC0, SetOrientation 0x00AEDE60, CCountedPointer<CParticleEmitter>(CParticleEmitter*) 0x00AE9450 / dtor 0x00AE94A0,
//   CParticleEmitterList::Insert 0x00AEB060, C3DPositionInterpolator 0x006AF570, C3DOrientationInterpolator 0x005508D0,
//   ~CBaseClass 0x0099A2E0.
// Shape: the TU model of the landed sibling 0x00AEB6D0 CreateEmitter, copied verbatim (its same-TU read-only stand-in bodies
//   for the counted pointer's ctor/dtor and Insert tell VC7.1 the local does not escape, so the clone is read once into esi
//   after Insert, as retail). Without them (chunk 754 tries a-i) VC loaded it through ecx and copied it to esi.
class C2DVector { public: float X, Y; };
#include "engine/C3DVector.h"
class CRightHandedSet { public:
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
};
enum EEngine2DLayer { ENGINE_2D_LAYER_DEFAULT = 0 };

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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

namespace NParticleEngine {
class CParticleEmitterManager {
public:
    CParticleEmitterList ActiveEmitters;           // +0x0000
    CParticleEmitter* CreateInterpolatedEmitter(const CParticleEmitter* from, const CParticleEmitter* to, float interpolate,
                                                const C3DVector& position, const CRightHandedSet& orientation);
};
CParticleEmitter* CParticleEmitterManager::CreateInterpolatedEmitter(const CParticleEmitter* from, const CParticleEmitter* to,
    float interpolate, const C3DVector& position, const CRightHandedSet& orientation)
{
    CParticleEmitter* result = 0;
    if (from && from->ShouldSpawn(position) && to && to->ShouldSpawn(position)) {
        CEmitterPointer emitter(from->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            emitter->InterpolateTo(*to, interpolate);
            emitter->SetPosition(C3DPositionInterpolator(position, position));
            emitter->SetOrientation(C3DOrientationInterpolator(orientation, orientation));
            result = emitter.Object;
        }
    }
    return result;
}
}