// Grade: DIFFER(186v171) -- register allocation: retail keeps `this` in esi and the result in edi (ours the reverse), and pushes the Set2DPosition arguments interleaved with the result load; tried the result declared before/after the template fetch
// CParticleEmitterManager::Create2DEmitter(long, const C2DVector*, EEngine2DLayer, long, const CRightHandedSet*) -- fetch the emitter template; if it is a 2D emitter, clone it into the active list, put it at the screen position on the layer and orient it; null otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AEB790, length 171 bytes (`ret 14h`).
// Callees: CParticleEmitterDatabase::GetEmitterTemplate @ 0x00AE89E0 on the static counted pointer 0x013D2E14,
//          CParticleEmitter::DatabaseClone @ 0x00AF0650, Set2DPosition @ 0x00AED090, SetOrientation @ 0x00AEDE60,
//          CCountedPointer<CParticleEmitter>(CParticleEmitter*) @ 0x00AE9450, CParticleEmitterList::Insert @ 0x00AEB060,
//          C3DOrientationInterpolator ctor @ 0x005508D0, ~CBaseClass @ 0x0099A2E0, operator delete @ 0x00BFE9BC.
// Shape: unlike the 3D creators, the counted-pointer local's destructor is inlined here (release of the count
// block: the destroy member-function pointer, then delete); the 4th argument is the template flag, forwarded as
// is (the manifest's "bool start" -- the callers in this chunk, NUISystem::CParticleEffect, pass 1).
void __cdecl operator delete(void* block);         // 0x00BFE9BC
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

struct CCountedItem { };
struct SCountBlock {                               // the shared count block
    long m_count;                                  // +0x00
    void (CCountedItem::*m_pfnDestroy)();          // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                         // +0x08
};

struct CEmitterPointer {                           // CCountedPointer<CParticleEmitter>
    CEmitter*    Object;
    SCountBlock* Block;
    CEmitterPointer(CEmitter* object);             // 0x00AE9450
    ~CEmitterPointer()
    {
        if (Block) {
            if (--Block->m_count == 0) {
                (Block->m_pItem->*Block->m_pfnDestroy)();
                operator delete(Block);
            }
        }
    }
    CEmitter* operator->() const { return Object; }
};

struct CParticleEmitterList {
    unsigned char _list[4];                        // +0x00 STLport list<CEmitterPointer>
    long          MaxActiveEmitters;               // +0x04
    bool Insert(CEmitterPointer& emitter);         // 0x00AEB060
};

struct CParticleEmitterManager {
    CParticleEmitterList ActiveEmitters;           // +0x0000
    CEmitter* Create2DEmitter(long handle, const C2DVector* position, EEngine2DLayer layer, long flag, const CRightHandedSet* orientation);
};

CEmitter* CParticleEmitterManager::Create2DEmitter(long handle, const C2DVector* position, EEngine2DLayer layer, long flag, const CRightHandedSet* orientation)
{
    CEmitter* emitterTemplate = g_ParticleEmitterDatabase->GetEmitterTemplate(handle, flag);
    CEmitter* result = 0;
    if (emitterTemplate && emitterTemplate->Is2D()) {
        CEmitterPointer emitter(emitterTemplate->DatabaseClone());
        if (ActiveEmitters.Insert(emitter)) {
            result = emitter.Object;
            result->Set2DPosition(position, layer);
            result->SetOrientation(C3DOrientationInterpolator(*orientation, *orientation));
        }
    }
    return result;
}
