// CCreatureAction_NymphGetHit::PerformAction() -- when the nymph's definition has a special-effects sub-def that maps "TELEPORT_POS" to an effect, spawn that effect at the nymph and attach its particle emitter to the nymph's "DUMMY_CENTRE" helper; then tell the CTCNymph component to fly around for the frames the action has left (TotalTime - FramesRunning)
// Grade: DIFFER(386v387) -- ours homes the copied def pointer in an extra frame word (sub esp,14h; `mov edi,[esp+14h] ; mov [esp+20h],edi`) and swaps the esi/edi roles (retail: &PCreature in edi, the def in esi reloaded from the GetSubDef slot after the emitter block); tried: named def copy, slot re-reads, def/emitter declared first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CA310, length 387 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature +8), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 on the
// thing's definition (+0x70), CCharString ctor/dtor @ 0x0099EBF0 / 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&,
// long&) @ 0x00669D30, CThing::GetPosition @ 0x004C73D0, CreateEffectThing(long, const C3DVector*, bool) @ 0x006E0880 (`__fastcall`),
// CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x4B = CTCDParticleEmitter, bit 11 of +0x28, `p = 0; if (HasTC) p = GetTC`;
// 0xF5 = CTCNymph, bit 21 of +0x3c, tested), CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float)
// @ 0x006E0BE0, CTCNymph::StartFlyingAround(long) @ 0x0080F5E0.
// Layout: CCreatureActionBase TotalTime +0x10, FramesRunning +0x1c (engine/CCreatureActionBase.h), PCreature +0x08.
// Shape: the landed CCreatureAction_TeleportIn 0x008B9F20 model -- the GetSubDef out-slot is a plain counted slot copied into a named
// def pointer and released by hand at the end (`dec [esi+4] ; call [edx+4]`); the lookup string is the implicit conversion inside a
// `found && ...` condition, so VC keeps a construction flag in bl and destroys the string before branching on the saved bool; the
// spawned effect is tested alive (null and bit 0 of +0x91) before the emitter lookup.
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
template <class T> struct CDefPointer {      // the out-slot GetSubDef fills (counted reference handed over)
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
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                      // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CTCDParticleEmitter {
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCNymph {
    void StartFlyingAround(long frames);        // 0x0080F5E0
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCreatureActionBase {                    // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    long                    TotalTime;          // +0x10
    long                    ActionTime;         // +0x14
    long                    AnimDelayTime;      // +0x18
    long                    FramesRunning;      // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
};
struct CCreatureAction_NymphGetHit : CCreatureActionBase {
    void PerformAction();
};

void CCreatureAction_NymphGetHit::PerformAction()
{
    CSpecialEffectsDef* effects;
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CDefClassBase* def = PCreature.GetPItem()->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    effects = effectsSlot.m_p;
    if (found && effects->GetEffectFromString("TELEPORT_POS", effectType)) {
        CThing* effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        if (effect && !(effect->m_flags91 & 1)) {
            CTCDParticleEmitter* emitter = 0;
            if (effect->HasTC(0x4b))
                emitter = effect->GetTC<CTCDParticleEmitter>(0x4b);
            emitter->AttachToThing(PCreature.GetPItem(), 10, "DUMMY_CENTRE", 0, 0.0f);
        }
    }
    if (PCreature.GetPItem()->HasTC(0xf5))
        PCreature.GetPItem()->GetTC<CTCNymph>(0xf5)->StartFlyingAround(TotalTime - FramesRunning);
    ReleaseDef(effects);
}
