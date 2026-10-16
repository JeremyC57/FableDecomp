// CTCDegradableBase::ShowParticleEffects(long) -- create the stage's smash particle thing at the parent's position and attach its emitter to the parent (bone 8)
// Grade: DIFFER(100v100) -- register swap only: retail keeps this in esi and the index (then the emitter) in edi, ours the reverse; tried nested ifs, an inline GetTCDParticleEmitter accessor, early returns
// Leaf: ShowParticleEffects
//
// Retail 0x008ECA50, 100 bytes (`ret 4`). Ego_r.pdb `?ShowParticleEffects@CTCDegradableBase@@AAEXJ@Z`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, the global Create(long, const C3DVector&, bool)
// @ 0x006E0880 (`__fastcall`, the bool on the stack, pushed first), CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 (the empty string 0x0122D70E), CTCDParticleEmitter::AttachToThing(CThing*,
// long, const CCharString&, long, float) @ 0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Layout: retail CThing interface bitset +0x20 (particle emitter = interface 0x4b, word 2 bit 11) and
// the cached particle-emitter component pointer at +0x5c.
// Shape: `T* e = 0; if (Has(k)) e = cached;` gives the preloaded `xor edi,edi`; the empty bone name is
// a named CCharString in its own block, built in the dead argument slot (the function has no other
// named locals and no frame).
struct C3DVector;

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    unsigned char _pad44[0x5c - 0x44];
    struct CTCDParticleEmitter* PParticleEmitter;           // +0x5c
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x006E0880

struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

struct CTCDegradableBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void ShowParticleEffects(long particleDefIndex);
};

void CTCDegradableBase::ShowParticleEffects(long particleDefIndex)
{
    if (particleDefIndex > 0)
    {
        CThing* particle = Create(particleDefIndex, ParentThing->GetPosition(), false);
        if (particle)
        {
            CTCDParticleEmitter* emitter = 0;
            if (particle->HasInterface(0x4b))
                emitter = particle->PParticleEmitter;
            {
                CCharString noBone("", -1);
                emitter->AttachToThing(ParentThing, 8, noBone, 0, 0.0f);
            }
        }
    }
}
