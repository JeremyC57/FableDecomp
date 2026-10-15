// CTCDExplosion::OnKill() -- kill every emitter thing the explosion created that is still alive
// Leaf: OnKill
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080C410, length 72 bytes (+8 int3). CTCDExplosion vtable 0x01242A54 slot 14
// (the CTCBase OnKill slot).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times per element), CThing::Kill(bool) @ 0x004C9B80.
// Shape: a plain iterator loop over Emitters (vector<pair<CIntelligentPointer<CThing>,long> >, 12-byte elements:
// `add esi,0Ch`), begin in esi, end re-read from +0x44 every trip (the loop head is aligned by `lea esp,[esp]`); the
// body is the landed IS_THING_ALIVE idiom (0x008CAD10, as in CTCCreatureOpinionOfHero::OnKill 0x00681B30): GetPItem
// re-fetched for the null test, the Dead bit (+0x91 bit 0) and the Kill target, `push 1` ahead of the third GetPItem.
// Layout: CTCDExplosion (Ego_r.pdb) with Emitters at +0x40 as a 12-byte STLport vector (first +0x40, last +0x44). No
// engine header for the class, so a local model.
struct C3DVector { float X, Y, Z; };

struct CThing {                             // only the dead bit and the call are modelled
    unsigned char _pad[0x91];
    unsigned char Dead : 1;                 // +0x91 bit 0
    void Kill(bool checkLocked);            // 0x004C9B80
};

struct CBaseIntelligentPointer {
    void* __vftable;                        // +0x00
    void* PData;                            // +0x04
    CThing* GetPItem() const;               // 0x00A01B50
};

// IS_THING_ALIVE(pointer): the item is set and its Dead bit is clear (GetPItem re-fetched, as the landed idiom 0x008CAD10).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->Dead)
            return true;
    }
    return false;
}

struct CEmitterEntry {                      // std::pair<CIntelligentPointer<CThing>, long>
    CBaseIntelligentPointer first;          // +0x00
    long second;                            // +0x08
};

struct CEmitterVector {                     // STLport vector<CEmitterEntry> (12 bytes)
    CEmitterEntry* _M_start;                // +0x00
    CEmitterEntry* _M_finish;               // +0x04
    CEmitterEntry* _M_end_of_storage;       // +0x08
    CEmitterEntry* begin() { return _M_start; }
    CEmitterEntry* end() { return _M_finish; }
};

struct CTCDExplosion {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13();
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
    void* PExplosionDef;                    // +0x38 CDefPointer<CExplosionDef const>
    bool CreatedEmitter;                    // +0x3c
    bool NeedToTriggerExplosion;            // +0x3d
    CEmitterVector Emitters;                // +0x40
    long ExplosionEvolutionTimer;           // +0x4c
    long CurrentExplosionRing;              // +0x50
    virtual void OnKill();
};

void CTCDExplosion::OnKill()
{
    for (CEmitterEntry* it = Emitters.begin(); it != Emitters.end(); ++it) {
        if (IS_THING_ALIVE(it->first))
            it->first.GetPItem()->Kill(true);
    }
}