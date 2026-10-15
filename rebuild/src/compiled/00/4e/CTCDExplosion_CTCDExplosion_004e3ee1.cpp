// CTCDExplosion::~CTCDExplosion() -- destroy the three thing-pointer vectors, the emitter vector and the explosion def, then CTCBase
// Leaf: ??1CTCDExplosion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3EE1, length 64 bytes (size-optimised def/factory module -> #pragma
// optimize("s", on)). Called by the scalar deleting destructor 0x004E3EC5 (CTCDExplosion vtable 0x01242A54 slot 0).
// Callees: vector<CIntelligentPointer<CThing> >::~vector @ 0x004406D0 (x3), vector<pair<CIntelligentPointer<CThing>,long> >
// ::~vector @ 0x004E1B08, CTCBase::~CTCBase @ 0x00686830 (tail jump).
// Shape: reverse member order with the RETAIL offsets (12-byte vectors, PDB 16): SpecifiedThingsToHit +0x6c, ImmuneThings
// +0x60, HitThings +0x54, Emitters +0x40, then the inlined CDefPointer release of PExplosionDef (+0x38).
#pragma optimize("s", on)
#define FORCEINLINE __forceinline

struct CThing;

struct CTCBase {                            // CTCBase root: vptr, ParentThing, two flags (0x0c bytes)
    void* __vftable;                        // +0x00
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    ~CTCBase();                             // 0x00686830 (stores the CTCBase vtable, then the CBaseObject destructor)
};

struct CDefPointeeBase {
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

struct CIntelligentPointer {                // CIntelligentPointer<CThing>
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    ~CIntelligentPointer();                 // 0x00A01C10 (CBaseIntelligentPointer::~CBaseIntelligentPointer)
};

struct C3DVector { float X, Y, Z; };

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

struct CTCDExplosion : CTCBase {
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