// CModeJackOfBladesFloat::OnActivate() -- spawn the FLOAT_POWER effect at the creature and attach it to the creature's target cursor point
// Grade: DIFFER(ours v retail) -- frame packing: retail reuses the dead GetSubDef out slot for the two string temporaries and the dead effect-type word for the CIntelligentPointer temporary (frame 0xc), ours gives the temporaries their own words (frame 0x10); everything else matches
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefSlot {            // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefSlot() : m_p(0) { }
};
template <class T> struct CDefPointer {         // a counted def reference
    T* Object;
    CDefPointer(T* object) : Object(object) { if (Object) ++Object->m_refCount; }
    ~CDefPointer() { if (Object) { if (--Object->m_refCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (--def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefSlot<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
struct CThing;
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};
struct CThing {
    unsigned char        _pad00[0x20];
    unsigned long        m_presentBits[9];      // +0x20 std::bitset of interface types
    unsigned char        _pad44[0x5c - 0x44];
    CTCDParticleEmitter* m_pParticleEmitter;    // +0x5c
    unsigned char        _pad60[0x70 - 0x60];
    CDefClassBase*       m_pDef;                // +0x70
    unsigned char        _pad74[0x91 - 0x74];
    unsigned char        m_flags91;             // +0x91 bit 0: dead
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CModeJackOfBladesFloat {
    void*                    __vftable;         // +0x00
    CThing*                  PCreature;         // +0x04
    unsigned char            _pad08[0x74 - 0x08];
    CIntelligentPointerThing PEmitter;          // +0x74
    CIntelligentPointerThing PEmitter2;         // +0x7c
    void OnActivate();
};

void CModeJackOfBladesFloat::OnActivate()
{
    CDefSlot<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    {
        CDefPointer<CDefClassBase> def(PCreature->m_pDef);
        def->GetSubDef(effectsSlot);
    }
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("FLOAT_POWER", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &PCreature->GetPosition(), false);
            if (effect && !(effect->m_flags91 & 1)) {
                PEmitter2 = effect;
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = effect->m_pParticleEmitter;
                    emitter->AttachToThing(PCreature, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        ReleaseDef(effects);
    }
}
