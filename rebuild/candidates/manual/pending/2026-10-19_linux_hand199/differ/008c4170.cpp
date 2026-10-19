// CCreatureAction_ScorpionKingDeath::Initialise() -- play FALL_TO_DEATH, run the combat-action Initialise, then spawn the scorpion king definition's "DEATH" special effect at its position when its special-effects sub-def maps that name
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4170, length 224 bytes (plain `ret`, virtual).
// Grade: DIFFER(218v218) -- frame only: retail's 8-byte frame lets the "DEATH" string temporary reuse the dead GetSubDef
//        out-slot word; ours keeps the slot alive (0xC frame, every frame offset +4)
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCombatActionBase::SetAnimationName(const
// CCharString&) 0x008574D0, CCombatActionBase::Initialise 0x00857520 (direct base call), CBaseIntelligentPointer::GetPItem
// 0x00A01B50 (PCreature +8, twice), CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&)
// 0x00669580, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30 (landed as CCRC::CalcAndLookup),
// CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880.
// Shape: unlike the teleport/undead bodies the thing's definition is taken as a counted copy (addref `inc [esi+4]`,
// released through slot 1 right after GetSubDef); the sub-def is copied into a named pointer and the whole effect block
// is guarded by its null test, so the final release has no null test; the lookup string is a plain argument temporary
// (no construction flag) and the result rides in bl across its destructor.
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
    bool GetEffectFromString(const CCharString& name, long& type);   // 0x00669D30
};
template <class T> struct CDefPointer {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> struct CCountedDefPointer {   // CDefPointer<T> copy: addref on copy, release on destruction
    T* m_p;
    CCountedDefPointer(T* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CCountedDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

struct CThing {
    unsigned char  _pad0[0x70];
    CDefClassBase* m_pDef;                      // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

struct CCreatureAction_ScorpionKingDeath : CCombatActionBase {
    virtual void Initialise();
};

void CCreatureAction_ScorpionKingDeath::Initialise()
{
    {
        CCharString name("FALL_TO_DEATH", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    {
        CCountedDefPointer<CDefClassBase> def(PCreature.GetPItem()->m_pDef);
        def->GetSubDef(effectsSlot);
    }
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("DEATH", effectType))
            CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        if (--effects->m_refCount == 0)
            effects->Destroy();
    }
}
