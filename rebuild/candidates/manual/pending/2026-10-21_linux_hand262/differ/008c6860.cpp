// Grade: DIFFER(493v482) -- frame + ownership model: retail's frame is 0x10 because the shot def pointer lives in ebp (its GetDef out slot is reused for the TC-name string and then the melee def pointer), the HasTC(0x46) bit is kept as a stored bool byte, and the ability def is released with no null test; ours keeps all three def pointers in memory (frame 0x18) and allocates registers differently from the carrying lookup on.
// CCreatureAction_TrollThrowProjectile::PerformAction() -- when the troll carries the projectile thing: drop it from the carrying component,
// look up the ability's shot def, add that shot component (named from the def string table) to the projectile and initialise it from the
// troll at either the stored target position or the target thing, with the troll's shot accuracy.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C6860, length 482 bytes.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50; LowerBound 0x0040F020 (interface 0x46, bit 6 of +0x28); CTCCarrying::RemoveThing
// 0x006A19B0; CDefClassBase::GetSubDef<CAbilityDef> 0x0042B1A6; 0x00692940 game context (+0x10 def manager); CDefinitionManager::GetDef
// 0x0078B230; CThingCreatureBase::GetShotAccuracyPercentage 0x00663B10; CDefStringTable::GetString 0x009D49B0 on 0x013CA828; CThing::AddTC
// 0x004C9D60; ~CCharString 0x0099EAE0; CTCShotDefinitionBase::Init 0x007BF790 (target position) / 0x007C0690 (target thing); def slot 1 release.
// Layout (descriptive): PCreature +8, PObjectToUse +0xa8, projectile +0x114, target position +0x11c, use-position flag +0x128; thing def
// +0x70; ability def shot index +0xa0; shot def TC name +0x60.
// Leaf: PerformAction
struct CCharString {
    char* m_pData;
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefRefCounted {
    virtual void v00();
    virtual void Release();                            // slot 1
    long RefCount;                                     // +4
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) {}
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CAbilityDef : CDefRefCounted { unsigned char _pad08[0xa0 - 8]; long ShotDefIndex; };   // +0xa0
struct CThingShotDef : CDefRefCounted { unsigned char _pad08[0x60 - 8]; long TCName; };      // +0x60
struct CMeleeCombatAbilityDef : CDefRefCounted {};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>* out);    // 0x0042B1A6
};
struct CDefinitionManager {
    bool GetDef(long index, CDefPointer<CThingShotDef>* out);   // 0x0078B230
};
struct CDefStringTable {
    CCharString GetString(long index);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828
struct CTCParamsBase;
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CTCEntry { long Type; CTCBase* TC; };
struct CInterfaceMap {
    CTCEntry* first;
    CTCEntry* last;
    CTCEntry* LowerBound(const long* type);            // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                    // +0x20
    CInterfaceMap m_interfaces;                        // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                               // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                           // +0x91
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    long GetShotAccuracyPercentage();                  // 0x00663B10
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CTCEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->Type)
            it = map->last;
        return (T*)it->TC;
    }
};
struct CTCCarrying {
    void RemoveThing(CThing* thing);                   // 0x006A19B0
};
struct CTCShotDefinitionBase {
    void Init(CDefPointer<CThingShotDef> def, CThing* shooter, CThing* a, const C3DVector* target, float accuracy, float b, float c,
              CDefPointer<CMeleeCombatAbilityDef>* melee);   // 0x007BF790
    void Init(CDefPointer<CThingShotDef> def, CThing* shooter, CThing* a, CThing* target, float accuracy, float b, float c,
              CDefPointer<CMeleeCombatAbilityDef>* melee);   // 0x007C0690
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                   // +4
    CThing* GetPItem() const;                          // 0x00A01B50
};
struct CActionGameContext { unsigned char _pad[0x10]; CDefinitionManager* PDefManager; };   // +0x10
struct CCreatureActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CActionGameContext* GetGameContext();              // 0x00692940
};
struct CCreatureAction_TrollThrowProjectile : CCreatureActionBase {
    CBaseIntelligentPointer PObjectToUse;              // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    CBaseIntelligentPointer PProjectile;               // +0x114
    C3DVector TargetPos;                               // +0x11c
    bool UseTargetPos;                                 // +0x128
    void PerformAction();
};

void CCreatureAction_TrollThrowProjectile::PerformAction()
{
    CThing* creature = PCreature.GetPItem();
    bool hasCarrying = (creature->m_presentBits[2] >> 6) & 1;
    if (!hasCarrying)
        return;
    CTCCarrying* carrying = creature->GetTC<CTCCarrying>(0x46);
    if (PProjectile.GetPItem() == 0 || (PProjectile.GetPItem()->m_flags91 & 1))
        return;
    carrying->RemoveThing(PProjectile.GetPItem());
    CDefPointer<CAbilityDef> abilityDef;
    PCreature.GetPItem()->PDef->GetSubDef(&abilityDef);
    CDefPointer<CThingShotDef> shotDef;
    GetGameContext()->PDefManager->GetDef(abilityDef->ShotDefIndex, &shotDef);
    long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
    CTCShotDefinitionBase* shot = (CTCShotDefinitionBase*)PProjectile.GetPItem()->AddTC(g_DefStringTable.GetString(shotDef->TCName), true, 0);
    CDefPointer<CMeleeCombatAbilityDef> melee;
    if (UseTargetPos)
        shot->Init(shotDef, PCreature.GetPItem(), 0, &TargetPos, (float)accuracy, 1.0f, 0.0f, &melee);
    else {
        CThing* shooter = PCreature.GetPItem();
        shot->Init(shotDef, shooter, 0, PObjectToUse.GetPItem(), (float)accuracy, 1.0f, 0.0f, &melee);
    }
}
