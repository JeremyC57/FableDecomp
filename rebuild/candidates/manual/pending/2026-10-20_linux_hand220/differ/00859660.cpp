// CCombatActionBase::ResetMeleeCombatAbilityEffects() -- unless already reset, look up the melee ability's effect def; when it affects speed, drop this action's haste multiplier on the attacker and on a live target and remove its bullet-time slowdown; when it changed the environment theme, transition channel 2 back to the default over its time; release the def
// Grade: DIFFER(317v311) -- register allocation: retail pushes edi at entry and keeps the effect def (the slot's value) in edi -- read once for the speed test, re-read from the slot after the speed block, and used for the theme and the release -- in a two-word frame; ours keeps it in eax and re-reads the slot at every use (an `effect` local adds a third, spilled frame word instead); tried a local, slot-> everywhere, and a releasing CDefPointer destructor
// Leaf: ?ResetMeleeCombatAbilityEffects@CCombatActionBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00859660, length 311 bytes (plain `ret`).
// Callees: GetPDefinitionManager 0x00692910 / CDefinitionManager::GetDef<T>(long, CDefPointer*) 0x0085ABB0 (arguments pushed
//          before the manager is fetched), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
//          (interface 0x8C = CTCHaste), CTCHaste::RemoveMultiplier 0x0058148D (landed), IS_THING_ALIVE 0x004CC340,
//          GetPWorld 0x00692940, RemoveBulletTimeSlowdown 0x004C6140 (world +0x68), the environment's (world +0x18) default
//          theme transition 0x006B3950 (channel 2, float time), the def's slot 1 (destroy at zero).
// Layout: the retail CCombatActionBase of the landed ctor 0x00857E40 (PCreature +8, PTarget +0xa8, PMeleeAbilityDef +0xb8,
//          HasResetMeleeCombatAbilityEffects +0xe0); the ability def's effect index +0x60; the effect def fields are
//          descriptive (+0x40 time, +0x4c theme, +0x58 speed flag).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* end;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
bool __fastcall IS_THING_ALIVE(const CBaseIntelligentPointer& thing);   // 0x004CC340
struct CTCHaste {
    void RemoveMultiplier(void* source);        // 0x0058148D
};
struct CDefVirtual {
    virtual ~CDefVirtual();                     // slot 0
    virtual void Destroy();                     // slot 1
};
struct CMeleeAbilityEffectDef : CDefVirtual {
    long          RefCount;                     // +0x04
    unsigned char _pad08[0x40 - 0x08];
    long          ThemeTransitionTime;          // +0x40
    unsigned char _pad44[0x4c - 0x44];
    long          EnvironmentTheme;             // +0x4c (-1: none)
    unsigned char _pad50[0x58 - 0x50];
    bool          AffectsSpeed;                 // +0x58
};
__forceinline bool DecRefToZero(long& count) { return --count == 0; }
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    __forceinline ~CDefPointer() { if (DecRefToZero(m_p->RefCount)) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
struct CDefinitionManager {
    template <class T> bool GetDef(long globalIndex, CDefPointer<T>* out);   // 0x0085ABB0
};
struct CMeleeCombatAbilityDef {
    unsigned char _pad[0x60];
    long          EffectDefIndex;               // +0x60
};
struct CBulletTimeManager {
    void RemoveBulletTimeSlowdown(void* key);   // 0x004C6140
};
struct CEnvironment {
    void TransitionToDefaultTheme(long channel, float time);   // 0x006B3950
};
struct CWorld {
    unsigned char       _pad[0x18];
    CEnvironment*       Environment;            // +0x18
    unsigned char       _pad1c[0x68 - 0x1c];
    CBulletTimeManager* BulletTime;             // +0x68
};
struct CCombatActionBase {
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[8];
    CMeleeCombatAbilityDef* PMeleeAbilityDef;   // +0xb8
    unsigned char           _padbc[0xe0 - 0xbc];
    bool                    HasResetMeleeCombatAbilityEffects;   // +0xe0
    CDefinitionManager* GetPDefinitionManager() const;   // 0x00692910
    CWorld* GetPWorld() const;                  // 0x00692940
    void ResetMeleeCombatAbilityEffects();
};

void CCombatActionBase::ResetMeleeCombatAbilityEffects()
{
    if (HasResetMeleeCombatAbilityEffects || !PMeleeAbilityDef || PMeleeAbilityDef->EffectDefIndex < 0)
        return;
    CDefPointer<CMeleeAbilityEffectDef> effectSlot;
    GetPDefinitionManager()->GetDef(PMeleeAbilityDef->EffectDefIndex, &effectSlot);
    CMeleeAbilityEffectDef* effect = effectSlot.m_p;
    if (effect->AffectsSpeed) {
        CTCHaste* haste;
        if (PCreature.GetPItem()->GetInterface(&haste, 0x8c))
            haste->RemoveMultiplier(this);
        if (IS_THING_ALIVE(PTarget)) {
            if (PTarget.GetPItem()->GetInterface(&haste, 0x8c))
                haste->RemoveMultiplier(this);
        }
        GetPWorld()->BulletTime->RemoveBulletTimeSlowdown(this);
    }
    if (effectSlot->EnvironmentTheme >= 0)
        GetPWorld()->Environment->TransitionToDefaultTheme(2, (float)effectSlot->ThemeTransitionTime);
}
