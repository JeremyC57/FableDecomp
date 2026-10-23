// CTCDivineWrath::UnsheatheWeapon() -- when the item the spell took out of the hero's hand is still alive: clear the primary carry slot (dropping a weapon, letting a 0x1a component react when no longer carried), put the item back in its carry slot, refresh the sheathes, cancel the melee flourish, restart the current creature mode and put the player in aggressive mode
// Leaf: UnsheatheWeapon
//
// Retail 0x0078E9C0, 545 bytes, plain `ret`. A byte-identical copy of this body is CTCUnholyPower's at 0x00793EC0.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined lookups: 0x46
// CTCCarrying, 0x48 weapon, 0x1a, 0x13 CTCInventoryWeapons, 0x30 CTCCombat, 0x31 CTCCreatureModeManager),
// CTCCarrying::HasPendingActions @ 0x006A1270 / GetThingInPrimarySlot @ 0x006A1440 / RemoveThing @ 0x006A19B0 /
// IsCarryingThing @ 0x006A1060 / RemoveThingFromPrimarySlot @ 0x006A1E00 / PutThingInCarrySlot @ 0x006A1F80,
// DropWeapon @ 0x00845CE0 (__fastcall), the 0x1a component's reaction @ 0x005D8E50, CDefClassBase::GetSubDef<CCarryableDef>
// @ 0x005CC10E, CTCInventoryWeapons::RefreshSheathedWeapons @ 0x005C9962, CTCCombat::SetMeleeFlourishValid @ 0x00733E10,
// CTCCreatureModeManager::RestartCurrentMode @ 0x0073A840, CThing::GetPlayer @ 0x004C7A30, CPlayer::SetAggressiveMode
// @ 0x00487EB0.
// Layout = Ego_r.pdb CTCDivineWrath: PDef +0x0c, PItemRemoved +0x10 (CIntelligentPointer<CThing>); CThing bitset +0x20, map
// +0x44, def +0x70, dead flag +0x91; CCarryableDef carry slot +0x28.
// Model: the landed CCreatureAction_UnsheatheItemFromInventory::PerformAction (0x0069F9E0), with the parent thing held in a
// local; the carrying and mode-manager components are left unset when absent (retail reads the lookup key's frame word).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CPlayer {
    void SetAggressiveMode(bool aggressive);                 // 0x00487EB0
};
struct CCarryableDef {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long RefCount;                                           // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long CarrySlot;                                          // +0x28
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (--Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CCarryableDef>* out);         // 0x005CC10E
};
CCarryableDef* FindCarryableDef(const CDefClassBase* def);   // stand-in: the sub def lookup (not graded here)
#pragma auto_inline(off)
// Same-TU body of GetSubDef<CCarryableDef> (VC7.1 then knows the def pointer's address does not escape).
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCarryableDef>* out)
{
    CCarryableDef* def = FindCarryableDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad0[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x70 - 0x4c];
    CDefClassBase* m_pDef;                                   // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                                // +0x91 bit 0: dead
    CPlayer* GetPlayer();                                    // 0x004C7A30
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
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
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                         // +0x04
    CThing* GetPItem() const;                                // 0x00A01B50
};
struct CTCCarrying {
    bool HasPendingActions();                                // 0x006A1270
    CThing* GetThingInPrimarySlot();                         // 0x006A1440
    void RemoveThing(CThing* thing);                         // 0x006A19B0
    bool IsCarryingThing(CThing* thing);                     // 0x006A1060
    CThing* RemoveThingFromPrimarySlot(bool drop);           // 0x006A1E00
    void PutThingInCarrySlot(CThing* thing, long slot, bool flag);   // 0x006A1F80
};
void __fastcall DropWeapon(CThing* weapon, CTCCarrying* carrying, CThing* owner);   // 0x00845CE0
struct CTC1A {
    void OnNotCarried();                                     // 0x005D8E50
};
struct CTCInventoryWeapons {
    void RefreshSheathedWeapons();                           // 0x005C9962
};
struct CTCCombat {
    void SetMeleeFlourishValid(bool valid);                  // 0x00733E10
};
struct CTCCreatureModeManager {
    void RestartCurrentMode();                               // 0x0073A840
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                                     // +0x04
    bool    ToBeRemoved;                                     // +0x08
    bool    IsInGlobalUpdateTCs;                             // +0x09
    void*   PDef;                                            // +0x0c CDefPointer<CSpecialAbilitiesDivineWrathDef const>
    CBaseIntelligentPointer PItemRemoved;                    // +0x10
    CThing* GetItemRemoved() const { return PItemRemoved.GetPItem(); }
    void UnsheatheWeapon();
};

void CTCDivineWrath::UnsheatheWeapon()
{
    if (!PItemRemoved.GetPItem() || (PItemRemoved.GetPItem()->m_flags91 & 1))
        return;
    CThing* parent = ParentThing;
    CTCCarrying* carrying;
    if (parent->HasTC(0x46))
        carrying = parent->GetTC<CTCCarrying>(0x46);
    if (!carrying->HasPendingActions()) {
        CThing* held = carrying->GetThingInPrimarySlot();
        if (held->HasTC(0x48))
            DropWeapon(held, carrying, parent);
        else
            carrying->RemoveThing(held);
        if (held->HasTC(0x1a)) {
            CTC1A* tc = held->GetTC<CTC1A>(0x1a);
            if (!carrying->IsCarryingThing(held))
                tc->OnNotCarried();
        }
        if (!carrying->HasPendingActions())
            carrying->RemoveThingFromPrimarySlot(false);
    }
    if (carrying->IsCarryingThing(PItemRemoved.GetPItem()))
        carrying->RemoveThing(PItemRemoved.GetPItem());
    CDefPointer<CCarryableDef> carryDef;
    CThing* item = GetItemRemoved();
    item->m_pDef->GetSubDef(&carryDef);
    carrying->PutThingInCarrySlot(GetItemRemoved(), carryDef->CarrySlot, true);
    if (parent->HasTC(0x13))
        parent->GetTC<CTCInventoryWeapons>(0x13)->RefreshSheathedWeapons();
    CTCCombat* combat;
    if (parent->GetInterface(&combat, 0x30))
        combat->SetMeleeFlourishValid(false);
    CTCCreatureModeManager* modeManager;
    if (parent->HasTC(0x31))
        modeManager = parent->GetTC<CTCCreatureModeManager>(0x31);
    modeManager->RestartCurrentMode();
    parent->GetPlayer()->SetAggressiveMode(true);
}