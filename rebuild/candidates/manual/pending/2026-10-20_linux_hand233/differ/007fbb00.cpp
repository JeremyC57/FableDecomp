// CTCKraken::CreateHeadEffect(const CCharString& name) -- replace the head effect: look the named effect up in the parent's special-effects sub-def, spawn it at the parent's position, keep it as the head effect and attach its particle emitter to the parent's "EFFECT_WATER" helper
// Grade: DIFFER(294v299) -- frame packing: retail builds the intelligent-pointer temporary over the dead GetSubDef out-slot (0xc frame, all four callee-saved pushes at entry, ebx the zero register); VC7.1 here gives the temporary its own words (0x10 frame) and sinks the esi push. Tried: block-scoped out-slot, block-scoped named effect pointer, both.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FBB00, length 297 bytes (+ int3 padding), `ret 4`. FableWin.pdb
// ?CreateHeadEffect@CTCKraken@@AAEXABVCCharString@@@Z (private).
// Callees: CTCKraken @ 0x007FB150 (no arguments, called first: drops the previous head effect; name descriptive),
// CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580, CSpecialEffectsDef::GetEffectFromString @ 0x00669D30 (result
// unused), CThing::GetPosition @ 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880,
// CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01B50/0x00A01C10 (CIntelligentPointer<CThing>
// vtable 0x01238C6C), the component-map LowerBound @ 0x0040F020, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0 ("EFFECT_WATER"
// @ 0x01271CF0), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) @ 0x006E0BE0.
// Shape: the landed UndeadReturnToEarth::Initialise (0x008EC320) def lookup: the out-slot and the effect type zeroed from the
// ebx zero register, the def copied into a named pointer (ebp) and released by hand at the end; the new effect goes through
// an intelligent-pointer temporary and CIntelligentPointer::operator= (SetPItem(other.GetPItem())); IS_THING_ALIVE is the
// double-GetPItem helper; the emitter pointer is preloaded with 0 (it reuses ebx) and the 0x4b key and the helper-name
// temporary are homed in the dead `name` argument slot.
// Layout: Ego_r.pdb CTCKraken: ParentThing +0x04, PHeadEffect +0x28 (CIntelligentPointer<CThing>); CThing PBaseDef +0x70,
// UsingComponents +0x20 (0x4b = word 2 bit 11), components map +0x44, dead bit +0x91. No engine header.
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
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
struct CTCBase_Retail;
struct STCEntry_Retail { long m_kind; CTCBase_Retail* m_pTC; };
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->m_kind) ? m_pLast : it)->m_pTC;
    }
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                 // +0x20 UsingComponents
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                       // +0x44
    unsigned char  _pad50[0x70 - 0x50];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                   // +0x91 bit 0: dead
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseObject;
struct CBaseIntelligentPointer {                // 8 bytes: vptr + the pointed-to item
    CBaseObject* m_pItem;                       // +4 (after the vptr)
    CBaseIntelligentPointer();                  // 0x00A01B10
    virtual ~CBaseIntelligentPointer();         // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

struct CTCKraken {
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x28 - 0x08];
    CIntelligentPointer PHeadEffect;            // +0x28
    void RemoveHeadEffect();                    // 0x007FB150
    void CreateHeadEffect(const CCharString& name);
};

void CTCKraken::CreateHeadEffect(const CCharString& name)
{
    RemoveHeadEffect();
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = ParentThing->m_pDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found)
        effects->GetEffectFromString(name, effectType);
    PHeadEffect = CIntelligentPointer(CreateEffectThing(effectType, &ParentThing->GetPosition(), false));
    if (IS_THING_ALIVE(PHeadEffect)) {
        CTCDParticleEmitter* emitter = 0;
        CThing* effect = PHeadEffect.GetPItem();
        if (effect->HasTC(0x4b))
            emitter = (CTCDParticleEmitter*)effect->m_tcs.Find(0x4b);
        emitter->AttachToThing(ParentThing, 0x4a, CCharString("EFFECT_WATER"), 0, 0.0f);
    }
    ReleaseDef(effects);
}
