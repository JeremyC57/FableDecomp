// CCreatureAction_CastWillEnflameSpell::FinishAction() -- base FinishAction; if the item removed for the cast is still alive, clear the primary carry slot (weapons dropped, other things removed, the showable hook run when no longer carried), put the item back in its carry slot (CCarryableDef::CarrySlot), refresh sheathed weapons, invalidate the melee flourish, update the front mode and put the main player in aggressive mode; finally reset the creature's enflame component if it is not active
// Leaf: ?FinishAction@CCreatureAction_CastWillEnflameSpell
// Retail: Fable.exe (TLC, VC7.1) @ 0x008BA3C0, 744 bytes (752 with padding), virtual FinishAction, plain `ret`.
// Callees: CCombatActionBase::FinishAction 0x00859A20; CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 /
//   SetPItem 0x00A01B90 / dtor 0x00A01C10 (CIntelligentPointer<CThing> vtable 0x01238C6C); CVectorMap::LowerBound 0x0040F020
//   (inline GetInterface: 0x46 CTCCarrying, 0x1A showable item, 0x13 CTCInventoryWeapons, 0x30 CTCCombat,
//   0x31 CTCCreatureModeManager, 0x95 enflame); CTCCarrying::IsPrimarySlotEmpty 0x006A1270 / GetThingInPrimarySlot 0x006A1440 /
//   RemoveThing 0x006A19B0 / IsCarryingThing 0x006A1060 / RemoveThingFromPrimarySlot 0x006A1E00 / PutThingInCarrySlot
//   0x006A1F80; DropWeapon 0x00845CE0 (__fastcall); CTCShowableItem::OnShowEnded 0x005D8E50; GetSubDef<CCarryableDef>
//   0x005CC10E; CTCInventoryWeapons::RefreshSheathedWeapons 0x005C9962; CTCCombat::SetMeleeFlourishValid 0x00733E10;
//   CTCCreatureModeManager::UpdateFrontMode 0x0073A840; CThing::GetPlayer 0x004C7A30; CPlayer::IsMainPlayer 0x00487520 /
//   SetAggressiveMode 0x00487EB0; the enflame reset 0x007A44C0.
// Layout: retail CCombatActionBase 0x114 (PCreature +0x08, PItemRemoved +0x2c), Level +0x114; CThing bitset +0x20, map +0x44,
//   def +0x70, dead bit +0x91; CCarryableDef CarrySlot +0x28; the enflame component's active flag +0xd.
// Shape: every component lookup is the out-parameter GetInterface (the carrying and mode lookups unchecked: retail reads the
//   never-written out slot on the miss path; the conditional ones fetch the creature once). The class def is an inline
//   accessor (its GetPItem runs before the &def push) and the item goes to PutThingInCarrySlot through the inline
//   conversion operator (its GetPItem runs before the other pushes). LowerBound / GetSubDef are same-TU noinline bodies so
//   the key and def words are not escaped; the def release folds its null test (DecRefToZero, landed 0x00848500 form).
class CTCBase;
class CThing;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCarryableDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                   // +0x28
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CCarryableDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCarryableDef>* out); // 0x005CC10E
};
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCarryableDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCarryableDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
class CPlayer { public:
    bool IsMainPlayer();                              // 0x00487520
    void SetAggressiveMode(bool aggressive);          // 0x00487EB0
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    CPlayer& GetPlayer();                             // 0x004C7A30
    CDefClassBase* GetDef() const { return m_pDef; }
    __forceinline unsigned long HasTC(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template<class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CBaseObject;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CBaseIntelligentPointer();                        // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                       // 0x00A01C10
    CBaseObject* GetPItem() const;                         // 0x00A01B50
    void SetPItem(CThing* item);                      // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    operator CThing*() const { return ((CThing*)GetPItem()); }
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(((CThing*)p.GetPItem())->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CTCShowableItem {
    void OnShowEnded();                               // 0x005D8E50
};
class CTCCarrying;
void __fastcall DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing* owner);   // 0x00845CE0
class CTCCarrying { public:
    bool IsPrimarySlotEmpty();                        // 0x006A1270
    CThing* GetThingInPrimarySlot();                  // 0x006A1440
    void RemoveThing(CThing* thing);                  // 0x006A19B0
    bool IsCarryingThing(CThing* thing);              // 0x006A1060
    CThing* RemoveThingFromPrimarySlot(bool unused);  // 0x006A1E00
    void PutThingInCarrySlot(CThing* thing, long slot, bool immediately);   // 0x006A1F80
};
class CTCInventoryWeapons { public:
    void RefreshSheathedWeapons();                    // 0x005C9962
};
class CTCCombat { public:
    void SetMeleeFlourishValid(bool valid);           // 0x00733E10
};
class CTCCreatureModeManager { public:
    void UpdateFrontMode();                           // 0x0073A840
};
struct CTCEnflame {
    unsigned char _pad[0xd];
    bool Active;                                      // +0xd
    void ResetState();                                // 0x007A44C0
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    protected: virtual void FinishAction(); public:                      // slot 4: 0x00859A20
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x2c - 0x10];
    CBaseIntelligentPointer PItemRemoved;             // +0x2c
    unsigned char _pad34[0x114 - 0x34];
};
class CCreatureAction_CastWillEnflameSpell : public CCombatActionBase { public:
    long Level;                                       // +0x114
    unsigned char _pad118[4];
    virtual void FinishAction();
};

void CCreatureAction_CastWillEnflameSpell::FinishAction()
{
    CCombatActionBase::FinishAction();
    CIntelligentPointer item;
    item.SetPItem(((CThing*)PItemRemoved.GetPItem()));
    if (IS_THING_ALIVE(item)) {
        CTCCarrying* carrying;
        ((CThing*)PCreature.GetPItem())->GetInterface(&carrying, 0x46);
        if (!carrying->IsPrimarySlotEmpty()) {
            CThing* thing = carrying->GetThingInPrimarySlot();
            if (thing->HasTC(0x48))
                DropWeapon(thing, carrying, ((CThing*)PCreature.GetPItem()));
            else
                carrying->RemoveThing(thing);
            CTCShowableItem* showable;
            if (thing->GetInterface(&showable, 0x1a)) {
                if (!carrying->IsCarryingThing(thing))
                    showable->OnShowEnded();
            }
            if (!carrying->IsPrimarySlotEmpty())
                carrying->RemoveThingFromPrimarySlot(false);
        }
        if (carrying->IsCarryingThing(((CThing*)item.GetPItem())))
            carrying->RemoveThing(((CThing*)item.GetPItem()));
        CDefPointer<CCarryableDef> carryDef;
        ((CThing*)item.GetPItem())->GetDef()->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot(item, carryDef->CarrySlot, true);
        CTCInventoryWeapons* weapons;
        if (((CThing*)PCreature.GetPItem())->GetInterface(&weapons, 0x13))
            weapons->RefreshSheathedWeapons();
        CTCCombat* combat;
        if (((CThing*)PCreature.GetPItem())->GetInterface(&combat, 0x30))
            combat->SetMeleeFlourishValid(false);
        CTCCreatureModeManager* modes;
        ((CThing*)PCreature.GetPItem())->GetInterface(&modes, 0x31);
        modes->UpdateFrontMode();
        if (((CThing*)PCreature.GetPItem())->GetPlayer().IsMainPlayer())
            ((CThing*)PCreature.GetPItem())->GetPlayer().SetAggressiveMode(true);
    }
    CTCEnflame* enflame;
    if (((CThing*)PCreature.GetPItem())->GetInterface(&enflame, 0x95)) {
        if (!enflame->Active)
            enflame->ResetState();
    }
}