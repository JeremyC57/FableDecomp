// CTCDExplosion::CTCDExplosion(CThing*) -- explosion driver constructor: zero radii, no life timer / ring (-1), an armed trigger and empty emitter / hit lists
// Leaf: ??0CTCDExplosion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E30E7, 93 bytes. Manifest label "CTCHeroRangedAttacks::CTCHeroRangedAttacks"
// is propagated from an unrelated body: the vtable stored is CTCDExplosion's (0x01242A54, rtti-vtables.tsv) and the
// stores follow CTCDExplosion's PDB members; single caller 0x004E3EBA (the CTCDExplosion factory).
// Callee: CTCDriverBase::CTCDriverBase(CThing*) @ 0x00771200.
//
// Shape: a size-optimised TU (`push dword ptr [esp+8]` pass-through, `or dword ptr [..],-1`). DamageRadius /
// PhysicsRadius (+0x0C/+0x10, `fldz ; fstp`) and ExplosionLifeTimer (+0x1C, -1) are plain initialisers scheduled
// above the vtable store; MaxDamage, FireDamage, StartPos and EndPos are left alone. After the vtable store come the
// def pointer (+0x38), the two flags (+0x3C false, +0x3D true), the Emitters vector (+0x40), the evolution timer / ring
// (+0x4C 0, +0x50 -1) and the three thing vectors (+0x54/+0x60/+0x6C).
// Layout: retail vectors are 12 bytes (PDB 16), so everything after Emitters sits 4..16 bytes below the PDB offsets
// (ExplosionEvolutionTimer +0x4C vs PDB +0x50); a local model.
#pragma optimize("s", on)

class CThing;

class CTCDriverBase { public:
    virtual void v00();
    unsigned char _body[0x8];                  // +0x04 CTCBase
    CTCDriverBase(CThing* parent);             // 0x00771200
};

class C3DVector { public:
    float x, y, z;
    C3DVector() {}
};

struct CDefPointer_Retail {                    // CDefPointer<const CExplosionDef>
    void* Object;
    CDefPointer_Retail() : Object(0) {}
};

struct CPtrVector {                            // retail std::vector: first, last, end
    void* m_pFirst;
    void* m_pLast;
    void* m_pEnd;
    CPtrVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) {}
};

class CTCDExplosion : public CTCDriverBase { public:
    float              DamageRadius;           // +0x0C
    float              PhysicsRadius;          // +0x10
    float              MaxDamage;              // +0x14
    long               FireDamage;             // +0x18
    long               ExplosionLifeTimer;     // +0x1C
    C3DVector          StartPos;               // +0x20
    C3DVector          EndPos;                 // +0x2C
    CDefPointer_Retail PExplosionDef;          // +0x38
    bool               CreatedEmitter;         // +0x3C
    bool               NeedToTriggerExplosion; // +0x3D
    CPtrVector         Emitters;               // +0x40 (PDB 16 bytes)
    long               ExplosionEvolutionTimer; // +0x4C (PDB +0x50)
    long               CurrentExplosionRing;   // +0x50 (PDB +0x54)
    CPtrVector         HitThings;              // +0x54 (PDB +0x58)
    CPtrVector         ImmuneThings;           // +0x60 (PDB +0x68)
    CPtrVector         SpecifiedThingsToHit;   // +0x6C (PDB +0x78)

    virtual void v00();
    CTCDExplosion(CThing* parent);
};

CTCDExplosion::CTCDExplosion(CThing* parent)
    : CTCDriverBase(parent),
      DamageRadius(0.0f), PhysicsRadius(0.0f), ExplosionLifeTimer(-1),
      CreatedEmitter(false), NeedToTriggerExplosion(true),
      ExplosionEvolutionTimer(0), CurrentExplosionRing(-1)
{
}