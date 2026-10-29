// CTCDegradableBase::ShowParticleEffects(long) -- create the stage's smash particle thing at the parent's position and attach its emitter to the parent (bone 8)
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: ShowParticleEffects
//
// Retail 0x008ECA50, 100 bytes (`ret 4`). Ego_r.pdb `?ShowParticleEffects@CTCDegradableBase@@AAEXJ@Z`.
// Callees: CThing::GetPos() const @ 0x004C73D0, the global Create(long, const C3DVector&, bool)
// @ 0x006E0880 (`__fastcall`, the bool on the stack, pushed first), CCharString::CCharString(const
// char*, long) @ 0x0099EBF0 (the empty string 0x0122D70E), CTCDParticleEmitter::AttachToThing(CThing*,
// long, const CCharString&, long, float) @ 0x006E0BE0, ~CCharString @ 0x0099EAE0.
// Layout: retail CThing interface bitset +0x20 (particle emitter = interface 0x4b, word 2 bit 11) and
// the cached particle-emitter component pointer at +0x5c.
// Shape: `T* e = 0; if (Has(k)) e = cached;` gives the preloaded `xor edi,edi`; the empty bone name is
// a named CCharString in its own block, built in the dead argument slot (the function has no other
// named locals and no frame).
class C3DVector;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    unsigned char _pad44[0x5c - 0x44];
    class CTCDParticleEmitter* PParticleEmitter;           // +0x5c
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    const C3DVector& GetPos() const;                   // 0x004C73D0
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);             // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

CThing* __fastcall Create(long defIndex, const C3DVector& position, bool flag);   // 0x006E0880

class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

class CTCDegradableBase { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    CThing* GetParentThing() const { return ParentThing; }
    void ShowParticleEffects(long particleDefIndex);
};

void CTCDegradableBase::ShowParticleEffects(long particleDefIndex)
{
    if (particleDefIndex > 0)
    {
        CThing* particle = Create(particleDefIndex, GetParentThing()->GetPos(), false);
        if (particle)
        {
            CTCDParticleEmitter* emitter = 0;
            if (particle->HasInterface(0x4b))
                emitter = particle->PParticleEmitter;
            {
                CCharString noBone("", -1);
                emitter->AttachToThing(*ParentThing, 8, noBone, 0, 0.0f);
            }
        }
    }
}