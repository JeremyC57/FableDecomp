// CFirePoint::SetAsBurning() -- mark the point burning; when it has a fire effect type, create that effect thing at the parent's position, and if it is alive attach its particle emitter to the parent's helper and keep it in PEffect
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00780540, length 144 bytes.
// Callees: CThing::GetPosition @ 0x004C73D0 (`[ecx+60h] + 0Ch`; manifest label "CVertexBufferWin32::DoSizeof" is propagated),
// the global __fastcall effect factory Create(long, const C3DVector*, bool) @ 0x006E0880, CTCDParticleEmitter::AttachToThing(CThing*, long,
// const CCharString&, long, float) @ 0x006E0BE0, CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 (x2) / GetPItem @ 0x00A01B50
// / dtor @ 0x00A01C10; vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Layout: Ego_r.pdb CFirePoint (44 bytes, as the landed constructor 0x00781200): HelperName +0, HelperIndex +4, EffectIndex +8,
// Burning +0x10, PParentThing +0x18, PEffect +0x1c. CThing: interface bits word +0x28 (bit 0x800 = particle emitter present,
// component at +0x5c), the dead-flag byte +0x91.
// Shape: Create's bool is pushed before the position call (right-to-left argument evaluation of a __fastcall); the emitter
// getter is an inline zero-initialised local assigned under the bit test (`xor ecx,ecx` hoisted; a ternary adds a `jmp`); the effect is stored as `PEffect = CIntelligentPointer(effect)` (temporary
// built with SetPItem, assigned through GetPItem/SetPItem, destroyed), as the landed CSeatTable::SetOccupant 0x007B85F0.
struct CThing;
struct C3DVector { float x, y, z; };

struct CCharString {
    void* PStringData;
};

struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};

struct CThing {
    unsigned char        _pad00[0x28];
    unsigned long        m_interfaceFlags;          // +0x28
    unsigned char        _pad2c[0x30];
    CTCDParticleEmitter* m_pParticleEmitter;        // +0x5c
    unsigned char        _pad60[0x31];
    unsigned char        m_statusFlags;             // +0x91 (bit 0: dead)
    const C3DVector& GetPosition();                 // 0x004C73D0
    CTCDParticleEmitter* GetParticleEmitter()
    {
        CTCDParticleEmitter* emitter = 0;
        if (m_interfaceFlags & 0x800)
            emitter = m_pParticleEmitter;
        return emitter;
    }
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
    ~CBaseIntelligentPointer();                     // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() {}
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CFirePoint {
    CCharString         HelperName;                 // +0x00
    long                HelperIndex;                // +0x04
    long                EffectIndex;                // +0x08
    long                Resistance;                 // +0x0c
    bool                Burning;                    // +0x10
    long                FireDamageAfterFrame;       // +0x14
    CThing*             PParentThing;               // +0x18
    CIntelligentPointer PEffect;                    // +0x1c
    CIntelligentPointer PSingeEffect;               // +0x24
    void SetAsBurning();
};

void CFirePoint::SetAsBurning()
{
    Burning = true;
    if (EffectIndex) {
        CThing* effect = CreateEffectThing(EffectIndex, &PParentThing->GetPosition(), false);
        if (effect && !(effect->m_statusFlags & 1)) {
            effect->GetParticleEmitter()->AttachToThing(PParentThing, 0xc, HelperName, HelperIndex, 0.0f);
            PEffect = CIntelligentPointer(effect);
        }
    }
}