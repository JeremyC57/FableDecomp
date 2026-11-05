// CTCDExplosion::~CTCDExplosion() -- destroy the three thing-pointer vectors, the emitter vector and the explosion def, then CTCBase
// Leaf: ??1CTCDExplosion
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3EE1, length 64 bytes (size-optimised def/factory module -> #pragma
// optimize("s", on)). Called by the scalar deleting destructor 0x004E3EC5 (CTCDExplosion vtable 0x01242A54 slot 0).
// Callees: vector<CIntelligentPointer<CThing> >::~vector @ 0x004406D0 (x3), vector<pair<CIntelligentPointer<CThing>,long> >
// ::~vector @ 0x004E1B08, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: reverse member order with the RETAIL offsets (12-byte vectors, PDB 16): SpecifiedThingsToHit +0x6c, ImmuneThings
// +0x60, HitThings +0x54, Emitters +0x40, then the inlined CDefPointer release of PExplosionDef (+0x38).
#pragma optimize("s", on)
#define FORCEINLINE __forceinline

class CThing;

class CTCBase { public:                            // CTCBase root: vptr, ParentThing, two flags (0x0c bytes)
    void* __vftable;                        // +0x00
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    ~CTCBase();                             // 0x00686830 (stores the CTCBase vtable, then the CBaseObject destructor)
};

class CDefPointeeBase { public:
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long RefCount;                          // +0x04
};

struct CDefPointer {                        // CDefPointer<T>: the pointee's count at +4, destroyed through slot 1
    CDefPointeeBase* Object;
    FORCEINLINE ~CDefPointer()
    {
        CDefPointeeBase* object = Object;
        if (object) {
            if (--object->RefCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
};

#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

#include "engine/C3DVector.h"

struct CThingPointerVector {                // vector<CIntelligentPointer<CThing> > (12 bytes)
    void* First;
    void* Last;
    void* End;
    ~CThingPointerVector();                 // 0x004406D0
};

struct CEmitterVector {                     // vector<pair<CIntelligentPointer<CThing>, long> > (12 bytes)
    void* First;
    void* Last;
    void* End;
    ~CEmitterVector();                      // 0x004E1B08
};

class CTCDExplosion : public CTCBase { public:
    float DamageRadius;                     // +0x0c
    float PhysicsRadius;                    // +0x10
    float MaxDamage;                        // +0x14
    long FireDamage;                        // +0x18
    long ExplosionLifeTimer;                // +0x1c
    C3DVector StartPos;                     // +0x20
    C3DVector EndPos;                       // +0x2c
    CDefPointer PExplosionDef;              // +0x38
    bool CreatedEmitter;                    // +0x3c
    bool NeedToTriggerExplosion;            // +0x3d
    CEmitterVector Emitters;                // +0x40
    long ExplosionEvolutionTimer;           // +0x4c
    long CurrentExplosionRing;              // +0x50
    CThingPointerVector HitThings;          // +0x54
    CThingPointerVector ImmuneThings;       // +0x60
    CThingPointerVector SpecifiedThingsToHit;   // +0x6c
    ~CTCDExplosion();
};

CTCDExplosion::~CTCDExplosion()
{
}