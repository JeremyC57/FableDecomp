// NFire::CFirePoint::SetAsSmoldering() -- unless a live singe effect is already attached, look up the parent def's "SINGE" special effect and, when it exists, create the point's effect at the parent's position, attach its particle emitter to the parent's helper point and keep it in PSingeEffect
// Grade: DIFFER(299v299) 28 difflines: same length; "SINGE" passed as const char* (implicit CCharString temporary built before the pushes); residue effects pointer cached in edi, zero register reuse, temp slots 4 lower than retail, CDefPointer dtor reloads (retail local copy in edi); needs a CDefPointer default ctor the shared header lacks (PENDING_HEADER once matched) (retry wave 2 sec2b)
// Was: DIFFER(300v299) -- the "SINGE" conditional temporary: retail keeps its construction flag in a frame word (zeroed at entry) and the && result in bl (`mov bl,1 / xor bl,bl`), builds the string before both argument pushes; this build keeps the flag in ebx and the result in a frame byte. Tried: `if (A && B)`, `bool found = A && B; if (found)`, the wrapped `if (!p || dead) {}` form, an inline bool helper (goes out of line, 224 bytes). Next: GOTCHAS wave 25 chunks 201-202 (`while`-condition form gives the frame-word flag + bl)
// Leaf: ?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007805D0, 299 bytes, thiscall, plain `ret`. Caller: NFire::CFirePoint::
//   AddFireDamage 0x00780800 (landed; it binds this address as `?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ`).
// Identity: Ego_r.pdb `?SetAsSmoldering@CFirePoint@NFire@@AAEXXZ` (private), the CTCFlammable TU with the landed
//   SetAsBurning 0x00780540 / Reset 0x00780700 / AddFireDamage 0x00780800.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / ctor 0x00A01B10 / dtor 0x00A01C10;
//   CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>*) const 0x00669580 (Ego_r.pdb
//   spelling); CCharString(const char*, long) 0x0099EBF0 ("SINGE" 0x0126B184) / ~CCharString 0x0099EAE0;
//   CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) const 0x00669D30; CThing::GetPosition 0x004C73D0;
//   CTCDParticleEmitter::Create(long, const C3DVector&, bool) 0x006E0880 (static __fastcall) and
//   AttachToThing(CThing&, long, const CCharString&, long, float) 0x006E0BE0; vtable 0x01238C6C = CIntelligentPointer<CThing>.
// Layout: Ego_r.pdb NFire::CFirePoint (0x2c): HelperName +0, HelperIndex +4, EffectIndex +8, PParentThing +0x18,
//   PSingeEffect +0x24; CThing def pointer +0x70, interface bits +0x28 (0x800: particle emitter at +0x5c), dead bit +0x91.
// Shape: the "SINGE" string is a temporary in the right operand of `&&` (the conditional-destruction flag word); the
//   looked-up effect index is a local the retail body never reads -- Create takes EffectIndex; the def pointer is
//   released inline (refcount +4, slot 1); the effect is stored through a CIntelligentPointer temporary as SetAsBurning.
class C3DVector { public: float X, Y, Z; };

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CSpecialEffectsDef { public:
    virtual void v00();
    virtual void Destroy() const;                       // slot 1
    mutable long RefCount;                              // +0x04
    bool GetEffectFromString(const CCharString& name, long& effect) const;   // 0x00669D30
};

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};

class CDefClassBase { public:
    template <class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00669580 (T = CSpecialEffectsDef)
};

class CThing;
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
    void AttachToThing(CThing& thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};

class CThing { public:
    unsigned char        _pad00[0x28];
    unsigned long        m_interfaceFlags;          // +0x28
    unsigned char        _pad2c[0x30];
    CTCDParticleEmitter* m_pParticleEmitter;        // +0x5c
    unsigned char        _pad60[0x10];
    CDefClassBase*       m_pDef;                    // +0x70
    unsigned char        _pad74[0x1d];
    unsigned char        m_statusFlags;             // +0x91 (bit 0: dead)
    bool IsDead() const { return (m_statusFlags & 1) != 0; }
    const C3DVector& GetPosition() const;           // 0x004C73D0
    CTCDParticleEmitter* GetParticleEmitter()
    {
        CTCDParticleEmitter* emitter = 0;
        if (m_interfaceFlags & 0x800)
            emitter = m_pParticleEmitter;
        return emitter;
    }
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C
public:
    CIntelligentPointer() {}
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

namespace NFire {
class CFirePoint { public:
    CCharString                 HelperName;         // +0x00
    long                        HelperIndex;        // +0x04
    long                        EffectIndex;        // +0x08
    long                        Resistance;         // +0x0c
    bool                        Burning;            // +0x10
    long                        FireDamageAfterFrame;   // +0x14
    CThing*                     PParentThing;       // +0x18
    CIntelligentPointer<CThing> PEffect;            // +0x1c
    CIntelligentPointer<CThing> PSingeEffect;       // +0x24
private:
    void SetAsSmoldering();
};

void CFirePoint::SetAsSmoldering()
{
    if (!PSingeEffect.GetPItem() || PSingeEffect.GetPItem()->IsDead()) {
        CDefPointer<const CSpecialEffectsDef> effects;
        long singeEffect;
        if (PParentThing->m_pDef->GetSubDef<CSpecialEffectsDef>(&effects)
            && effects->GetEffectFromString("SINGE", singeEffect)) {
            CThing* effect = CTCDParticleEmitter::Create(EffectIndex, PParentThing->GetPosition(), false);
            if (effect && !effect->IsDead()) {
                effect->GetParticleEmitter()->AttachToThing(*PParentThing, 0xc, HelperName, HelperIndex, 0.0f);
                PSingeEffect = CIntelligentPointer<CThing>(effect);
            }
        }
    }
}
}
