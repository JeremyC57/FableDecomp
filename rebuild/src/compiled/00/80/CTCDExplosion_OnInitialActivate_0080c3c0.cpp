// CTCDExplosion::OnInitialActivate() -- fetch the explosion sub-def and copy its radius, damage and life time
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C3C0, length 79 bytes (+1 int3). CTCDExplosion vtable 0x01242A54 slot 6.
// Callees: CDefClassBase::GetSubDef<CExplosionDef>(CDefPointer<T>*) const @ 0x00751190 (manifest label names another
// byte-identical instantiation), __ftol2 @ 0x00BFEA70. Global: the game frames-per-second long @ 0x01375550.
// Shape: the thing's def (+0x70) is used in place (no local copy, so no add-ref/release, unlike CTCDExplosiveTrail's
// hook), GetSubDef's bool result is dropped; the def pointer is read once into eax and DamageRadius and PhysicsRadius
// both take ExplosionRadius; ExplosionLifeTimer = (long)(seconds * frames-per-second) (`fild [g] ; fmul [eax+34h]`);
// then the ring evolution is reset (timer 0, ring -1).
// Layout: CTCDExplosion (Ego_r.pdb) to +0x3c; Emitters is a 12-byte STLport vector in retail (PDB 16), so
// ExplosionEvolutionTimer/CurrentExplosionRing sit at +0x4c/+0x50. CExplosionDef offsets are the PDB's. Local model (no
// engine header for CTCDExplosion).
class C3DVector { public: float X, Y, Z; };

template <class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};

class CExplosionDef { public:
    unsigned char _pad00[0x28];
    float Damage;                           // +0x28
    float ExplosionRadius;                  // +0x2c
    long FireDamage;                        // +0x30
    float ExplosionLifeTime;                // +0x34
};

struct CThingDef {
    bool GetSubDef(CDefPointer<CExplosionDef>* out) const;   // GetSubDef<CExplosionDef> @ 0x00751190
};

class CThing { public:
    unsigned char _pad00[0x70];
    CDefPointer<CThingDef> PDef;            // +0x70
};

extern long g_GameFramesPerSecond;          // 0x01375550

class CTCDExplosion { public:
    void* __vftable;                        // +0x00 (CTCBase)
    CThing* ParentThing;                    // +0x04
    bool ToBeRemoved;                       // +0x08
    bool IsInGlobalUpdateTCs;               // +0x09
    float DamageRadius;                     // +0x0c
    float PhysicsRadius;                    // +0x10
    float MaxDamage;                        // +0x14
    long FireDamage;                        // +0x18
    long ExplosionLifeTimer;                // +0x1c
    C3DVector StartPos;                     // +0x20
    C3DVector EndPos;                       // +0x2c
    CDefPointer<CExplosionDef> PExplosionDef;       // +0x38
    bool CreatedEmitter;                    // +0x3c
    bool NeedToTriggerExplosion;            // +0x3d
    void* Emitters[3];                      // +0x40 vector<pair<CIntelligentPointer<CThing>,long> > (12 bytes)
    long ExplosionEvolutionTimer;           // +0x4c
    long CurrentExplosionRing;              // +0x50
    void OnInitialActivate();
};

void CTCDExplosion::OnInitialActivate()
{
    ParentThing->PDef->GetSubDef(&PExplosionDef);
    DamageRadius = PExplosionDef->ExplosionRadius;
    PhysicsRadius = PExplosionDef->ExplosionRadius;
    MaxDamage = PExplosionDef->Damage;
    FireDamage = PExplosionDef->FireDamage;
    ExplosionLifeTimer = (long)(PExplosionDef->ExplosionLifeTime * g_GameFramesPerSecond);
    ExplosionEvolutionTimer = 0;
    CurrentExplosionRing = -1;
}