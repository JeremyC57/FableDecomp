// Grade: DIFFER(381v370) -- register choice only: retail loads the GetSubDef result straight into esi for the null test (mov esi,[def] ; cmp esi,ebx), ours goes through eax (mov eax ; cmp eax,ebx ; mov esi,eax); frame, slots and every call match
// CCombatAction_WillHealInto::FinishAction() -- plays the creature's HEAL special effect (attached to it) when its definition maps one, runs the base FinishAction, gives the creature a CTCHealLife at the spell level when it has none, and moves on to a CCombatAction_WillHealRelease
// Leaf: ?FinishAction@CCombatAction_WillHealInto@@UAEXXZ
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCParamsBase;
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
    ~CDefPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
struct CDefClassPointer {                       // CDefPointer<const CDefClassBase>, copied with an add-ref
    CDefClassBase* m_p;
    CDefClassPointer(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassPointer() { if (m_p && --m_p->m_refCount == 0) m_p->Destroy(); }
    CDefClassBase* operator->() const { return m_p; }
};
struct CThing;
struct CTCDParticleEmitter {                    // interface type 0x4B
    void AttachToThing(CThing* thing, long attachType, const CCharString& helperName, long helperIndex, float offset);   // 0x006E0BE0
};
struct CTCHealLife {
    void SetAICast(long level);                 // 0x007C3EA0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    unsigned char _pad44[0x5c - 0x44];
    CTCDParticleEmitter* PParticleEmitter;      // +0x5c (cached interface 0x4B)
    unsigned char _pad60[0x70 - 0x60];
    CDefClassBase* PBaseDef;                    // +0x70 CDefPointer<const CDefClassBase> object word
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CTCHealLife* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CCombatActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CCreaturePointer {                       // CIntelligentPointer<CThingCreatureBase>
    virtual void v00();
    CThingCreatureBase* m_pItem;                // +4
    CThingCreatureBase* GetPItem() const;       // 0x00A01B50
};
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                // slot 4: 0x00859A20
    unsigned char _pad04[4];
    CCreaturePointer PCreature;                 // +0x08
    unsigned char _pad10[0x114 - 0x10];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCombatAction_WillHealRelease : CCombatActionBase {   // vtable 0x01280644
    CCombatAction_WillHealRelease(CThingCreatureBase* owner);   // 0x008C3930
};
struct CCombatAction_WillHealInto : CCombatActionBase {
    long SpellLevel;                            // +0x114 (PDB +0x12c)
    virtual void FinishAction();
};

void CCombatAction_WillHealInto::FinishAction()
{
    CDefPointer<CSpecialEffectsDef> effects;
    CThing* effect = 0;
    {
        long effectType = 0;
        {
            CDefClassPointer def = PCreature.GetPItem()->PBaseDef;
            def->GetSubDef(effects);
        }
        CSpecialEffectsDef* fx = effects.m_p;
        if (fx) {
            if (fx->GetEffectFromString("HEAL", effectType))
                effect = CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        }
    }
    if (effect && !(effect->m_flags91 & 1) && effect->HasTC(0x4b)) {
        CTCDParticleEmitter* emitter = effect->PParticleEmitter;
        CCharString helper("");
        emitter->AttachToThing(PCreature.GetPItem(), 10, helper, 0, 0.0f);
    }
    CThingCreatureBase* creature = PCreature.GetPItem();
    CCombatActionBase::FinishAction();
    if (!creature->HasTC(0x99)) {
        CTCHealLife* heal = creature->AddTC("CTCHealLife", true, 0);
        heal->SetAICast(SpellLevel);
    }
    creature->SetCurrentAction(CCombatAction_WillHealRelease(creature));
}
