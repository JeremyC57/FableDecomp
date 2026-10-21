// CAIStateGroup_TrollBase::Initialise(...) -- base initialise, then the throwing distances from the sight range and the troll's weapon defs (defaults 28/8/11/8)
// Leaf: Initialise
// Grade: DIFFER(487v427) -- rough (one grade): retail keeps every local (perception out-pointer, four CDefPointers) in the dead argument slots with no frame and reuses one slot for the ability and the weapon def (the first def value moved to esi), plus a fifth callee-saved register; the nested CDefPointer release order and the default-constants tail need the arg-slot homing VC7.1 RTM does not reproduce from this source
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094D780; best of the probes graded under VC7.1 RTM (probe/0094d780.cpp).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CDefClassBase;
struct CRefCountedDef {
    virtual void v00();
    virtual void Destroy();
    long m_refs;                                // +0x04
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p != 0 && --m_p->m_refs == 0) m_p->Destroy(); }
    T* operator->() const { return m_p; }
};
struct CAbilityDef : CRefCountedDef {
    unsigned char _pad[0x50 - 8];
    long ThrowWeaponDefIndex;                   // +0x50
};
struct CThrowRangeDef : CRefCountedDef {
    unsigned char _pad[0x2c - 8];
    float MinThrowingDistance;                  // +0x2c
};
struct CSpecialAbilityDef : CRefCountedDef {
    bool GetAbilitySubDef(CDefPointer<CAbilityDef>* out) const;   // 0x00788E53
};
struct CWeaponDef : CRefCountedDef {
    bool GetThrowRangeSubDef(CDefPointer<CThrowRangeDef>* out) const;   // 0x00751190
};
struct CTrollCreatureDef : CRefCountedDef {
    unsigned char _pad[0x3c - 8];
    long SpecialAbilityDefIndex;                // +0x3c
    long ThrowWeaponDefIndex;                   // +0x40
};
struct CDefHolder {
    bool GetCreatureSubDef(CDefPointer<CTrollCreatureDef>* out) const;   // 0x008C79B0
};
struct CGameDefinitionManager {
    static CGameDefinitionManager& Get();       // 0x0044C6B0
    bool GetSpecialAbilityDef(long index, CDefPointer<CSpecialAbilityDef>* out);   // 0x0042AF3C
    bool GetWeaponDef(long index, CDefPointer<CWeaponDef>* out) { return GetSpecialAbilityDef(index, (CDefPointer<CSpecialAbilityDef>*)out); }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x70 - 0x4c];
    CDefHolder* PDef;                           // +0x70
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
struct CTCPerceiveThing {                       // interface 0x34
    float GetSightRange() const;                // 0x00744460
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CThingAICreature;
struct CCharString;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
        const CCharString& name);               // 0x008FCEC0
};
struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    unsigned char PEnemy[8];                    // +0x14
    long EnemyKnockDownFrame;                   // +0x1c
    long ShotCounter;                           // +0x20
    float TROLL_MAX_THROWING_DISTANCE;          // +0x24
    float TROLL_MIN_THROWING_DISTANCE;          // +0x28
    float TROLL_STOP_POUNDING_DISTANCE;         // +0x2c
    float TROLL_STOP_THROWING_DISTANCE;         // +0x30
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority,
        const CCharString& name);
};

void CAIStateGroup_TrollBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority,
    EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    CTCPerceiveThing* perception;
    PBrain->PCreature->GetInterface(&perception, 0x34);
    TROLL_MAX_THROWING_DISTANCE = perception->GetSightRange();
    CDefPointer<CTrollCreatureDef> trollDef;
    if (!PBrain->PCreature->PDef->GetCreatureSubDef(&trollDef))
        return;
    long weaponIndex = 0;
    if (trollDef->ThrowWeaponDefIndex != 0)
        weaponIndex = trollDef->ThrowWeaponDefIndex;
    else {
        long abilityIndex = trollDef->SpecialAbilityDefIndex;
        if (abilityIndex == 0)
            goto defaults;
        CDefPointer<CSpecialAbilityDef> ability;
        if (CGameDefinitionManager::Get().GetSpecialAbilityDef(abilityIndex, &ability)) {
            CDefPointer<CAbilityDef> sub;
            if (ability->GetAbilitySubDef(&sub))
                weaponIndex = sub->ThrowWeaponDefIndex;
        }
    }
    if (weaponIndex == 0)
        goto defaults;
    {
        CDefPointer<CWeaponDef> weapon;
        if (CGameDefinitionManager::Get().GetWeaponDef(weaponIndex, &weapon)) {
            CDefPointer<CThrowRangeDef> range;
            if (weapon->GetThrowRangeSubDef(&range)) {
                TROLL_MIN_THROWING_DISTANCE = range->MinThrowingDistance;
                TROLL_STOP_POUNDING_DISTANCE = range->MinThrowingDistance;
                TROLL_STOP_THROWING_DISTANCE = range->MinThrowingDistance + 1.0f;
            }
        }
    }
    return;
defaults:
    TROLL_MAX_THROWING_DISTANCE = 28.0f;
    TROLL_MIN_THROWING_DISTANCE = 8.0f;
    TROLL_STOP_POUNDING_DISTANCE = 11.0f;
    TROLL_STOP_THROWING_DISTANCE = 8.0f;
}
