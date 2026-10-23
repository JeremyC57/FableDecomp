// CTCCarrying::PutThingInCarrySlot(CThing* thing, long slot, bool setCombatMode) -- empty the slot if something is in it,
// Grade: DIFFER(664v660) -- frame 8 vs retail 0xc: the second find stores the copy into the slot home and keeps the slot in ebx
// store the thing there (the thing's carryable component is told who carries it), take it out of limbo, track it in the
// primary-slot list when the slot is a primary one and, for a non-melee weapon, re-enter the parent's current combat mode
// Leaf: PutThingInCarrySlot
//
// Retail 0x006A1F80, 658 bytes (`ret 0xc`).
// Callees: CVectorMap<long,CIntelligentPointer<CThing>>::LowerBound @ 0x006A2A10 / const @ 0x006A29B0,
// CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
// (CIntelligentPointer<CThing> vtable 0x01238C6C), CTCCarrying::RemoveThingInCarrySlot @ 0x006A16A0,
// CVectorMap::LowerBound @ 0x0040F020 / const @ 0x004365B0 (interfaces 0x47 CTCCarryable, 0x48 CTCWeapon, 0x31 mode manager),
// CTCCarryable::SetAsCarriedBy @ 0x00705D40, list<CIntelligentPointer<CThing>>::remove @ 0x0065DE8A / push_back
// @ 0x004BA4E0, CThing::SetInLimbo @ 0x004C8CF0, CGameDefinitionManager::Get @ 0x0044C6B0 (__fastcall), GetDef
// @ 0x006A3060, CTCWeapon::IsMeleeWeapon @ 0x005DAA70, CTCCreatureModeManager::GetCurrentMode @ 0x0073A6E0 /
// RemoveMode @ 0x0073ABA0 / AddMode @ 0x0073AE10; physics TC slot 80.
// Layout: CTCCarrying CarrySlots +0x0c (retail 12-byte vector + compare), ThingsInPrimarySlots +0x24; CThing bitset +0x20,
// map +0x44, PhysicsTC +0x60; CCarrySlotDef IsPrimary +0x30 (as the landed IsSlotPrimarySlot 0x006A1330).
struct CBaseObject;
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CBaseIntelligentPointer();                               // 0x00A01B10
    ~CBaseIntelligentPointer();                              // 0x00A01C10
    CThing* GetPItem() const;                                // 0x00A01B50
    void SetPItem(CBaseObject* item);                        // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem((CBaseObject*)other.GetPItem()); return *this; }
};
struct SCarrySlotEntry {                                     // pair<long, CIntelligentPointer<CThing>>
    long                         first;
    CIntelligentPointer<CThing>  second;
};
struct CCarrySlotMap {                                       // retail CVectorMap<long,CIntelligentPointer<CThing>>
    SCarrySlotEntry* m_pFirst;
    SCarrySlotEntry* m_pLast;
    SCarrySlotEntry* m_pEnd;
    bool             m_compare;
    bool             m_dirty;
    SCarrySlotEntry* LowerBound(const long& key);            // 0x006A2A10
    SCarrySlotEntry* LowerBound(const long& key) const;      // 0x006A29B0
    SCarrySlotEntry* End() const { return m_pLast; }
    SCarrySlotEntry* Find(long key)
    {
        long k = key;
        SCarrySlotEntry* it = LowerBound(k);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
    SCarrySlotEntry* Find(long key) const
    {
        long k = key;
        SCarrySlotEntry* it = LowerBound(k);
        if (it == End() || key < it->first)
            it = End();
        return it;
    }
};
struct CThingList {                                          // list<CIntelligentPointer<CThing>>
    void* m_node;
    void remove(const CIntelligentPointer<CThing>& value);   // 0x0065DE8A
    void push_back(const CIntelligentPointer<CThing>& value);   // 0x004BA4E0
};
struct CCarrySlotDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long          m_refCount;                                // +0x04
    unsigned char _pad08[0x30 - 0x08];
    bool          IsPrimary;                                 // +0x30
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        T* p = m_p;
        long count = p->m_refCount - 1;
        p->m_refCount = count;
        if (count == 0)
            p->Destroy();
    }
    T* operator->() const { return m_p; }
};
struct CDefinitionManager {
    void GetDef(long index, CDefPointer<CCarrySlotDef>* out);   // 0x006A3060
};
void* FindDef_CDefinitionManager_GetDef_0();
#pragma auto_inline(off)
__declspec(noinline) void CDefinitionManager::GetDef(long index, CDefPointer<CCarrySlotDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return;
    *(void**)out = def;
}
#pragma auto_inline(on)
struct CGameDefinitionManager {
    static CDefinitionManager* __fastcall Get();             // 0x0044C6B0
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
    SEntry* LowerBound(const long* type) const;              // 0x004365B0
};
struct CTCPhysicsBase {
#define V(n) virtual void v##n();
    V(00) V(01) V(02) V(03) V(04) V(05) V(06) V(07) V(08) V(09) V(10) V(11) V(12) V(13) V(14) V(15) V(16) V(17) V(18) V(19)
    V(20) V(21) V(22) V(23) V(24) V(25) V(26) V(27) V(28) V(29) V(30) V(31) V(32) V(33) V(34) V(35) V(36) V(37) V(38) V(39)
    V(40) V(41) V(42) V(43) V(44) V(45) V(46) V(47) V(48) V(49) V(50) V(51) V(52) V(53) V(54) V(55) V(56) V(57) V(58) V(59)
    V(60) V(61) V(62) V(63) V(64) V(65) V(66) V(67) V(68) V(69) V(70) V(71) V(72) V(73) V(74) V(75) V(76) V(77) V(78) V(79)
#undef V
    virtual void ResetMovement();                            // slot 80 (+0x140)
};
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    void SetInLimbo(bool inLimbo);                           // 0x004C8CF0
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline T* GetTCConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CTCCarrying;
struct CTCCarryable { void SetAsCarriedBy(CTCCarrying* carrier); };   // 0x00705D40
struct CTCWeapon { bool IsMeleeWeapon(); };                  // 0x005DAA70
struct CCreatureModeBase {
    virtual void v00();
    virtual long GetType();                                  // slot 1
};
struct CTCCreatureModeManager {
    CCreatureModeBase* GetCurrentMode();                     // 0x0073A6E0
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
    bool AddMode(unsigned long mode);                        // 0x0073AE10
};
struct CTCCarrying {
    virtual void v00();
    CThing*       ParentThing;                               // +0x04
    bool          ToBeRemoved;                               // +0x08
    bool          IsInGlobalUpdateTCs;                       // +0x09
    CCarrySlotMap CarrySlots;                                // +0x0c
    bool          BypassFilters;                             // +0x1c
    void*         Key;                                       // +0x20
    CThingList    ThingsInPrimarySlots;                      // +0x24
    bool IsCarrySlotEmpty(long slot)
    {
        SCarrySlotEntry* it = CarrySlots.Find(slot);
        if (it == CarrySlots.End())
            return false;
        return it->second.GetPItem() == 0;
    }
    bool IsSlotPrimarySlot(long slot)
    {
        CDefPointer<CCarrySlotDef> def;
        CGameDefinitionManager::Get()->GetDef(slot, &def);
        return def->IsPrimary;
    }
    void RemoveThingInCarrySlot(long slot, bool drop);       // 0x006A16A0
    void PutThingInCarrySlot(CThing* thing, long slot, bool setCombatMode);
};

void CTCCarrying::PutThingInCarrySlot(CThing* thing, long slot, bool setCombatMode)
{
    if (!IsCarrySlotEmpty(slot))
        RemoveThingInCarrySlot(slot, false);
    SCarrySlotEntry* it = ((const CCarrySlotMap&)CarrySlots).Find(slot);
    if (it != CarrySlots.End()) {
        it->second = CIntelligentPointer<CThing>(thing);
        CTCCarryable* carryable;
        if (thing->HasTC(0x47))
            carryable = thing->GetTC<CTCCarryable>(0x47);
        carryable->SetAsCarriedBy(this);
        thing->PhysicsTC->ResetMovement();
    }
    ThingsInPrimarySlots.remove(CIntelligentPointer<CThing>(0));
    thing->SetInLimbo(false);
    if (IsSlotPrimarySlot(slot)) {
        ThingsInPrimarySlots.push_back(CIntelligentPointer<CThing>(thing));
        if (setCombatMode && thing->HasTC(0x48) && !thing->GetTCConst<CTCWeapon>(0x48)->IsMeleeWeapon()
            && ParentThing->HasTC(0x31)) {
            CTCCreatureModeManager* modeManager = ParentThing->GetTC<CTCCreatureModeManager>(0x31);
            long mode = modeManager->GetCurrentMode()->GetType();
            if (mode != 4)
                modeManager->AddMode(4);
            if (mode == 0x19) {
                modeManager->RemoveMode(mode);
                modeManager->AddMode(mode);
                return;
            }
            if (mode == 0x24) {
                modeManager->RemoveMode(mode);
                modeManager->AddMode(mode);
                return;
            }
            if (mode == 0x0c) {
                modeManager->RemoveMode(mode);
                modeManager->AddMode(mode);
            }
        }
    }
}
