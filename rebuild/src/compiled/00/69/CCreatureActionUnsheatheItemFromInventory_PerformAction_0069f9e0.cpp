// CCreatureAction_UnsheatheItemFromInventory::PerformAction() -- unless the carrying component is busy: drop or remove whatever is in the primary slot (dropping weapons, letting a 0x1a component react when the item is not carried), and clear the slot; then, when the item to unsheathe is alive, prepare the weapon sheathes, put it into the carry slot its CCarryableDef names and refresh the sheathes; finally clear the melee flourish, restart the current mode and put the main player in aggressive mode
// Leaf: ?PerformAction@CCreatureAction_UnsheatheItemFromInventory
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069F9E0, length 596 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020, CTCCarrying::
//          HasPendingActions @ 0x006A1270 / GetThingInPrimarySlot @ 0x006A1440 / RemoveThing @ 0x006A19B0 /
//          IsCarryingThing @ 0x006A1060 / RemoveThingFromPrimarySlot @ 0x006A1E00 / PutThingInCarrySlot @ 0x006A1F80,
//          DropWeapon(CThing*, CTCCarrying*, CThing*) @ 0x00845CE0 (__fastcall), the 0x1a component's reaction
//          @ 0x005D8E50, CTCInventoryWeapons sheathing update @ 0x005C552C / RefreshSheathedWeapons @ 0x005C9962,
//          CDefClassBase::GetSubDef<CCarryableDef> @ 0x005CC10E, CTCCombat::SetMeleeFlourishValid @ 0x00733E10,
//          CTCCreatureModeManager::RestartCurrentMode @ 0x0073A840, CThing::GetPlayer @ 0x004C7A30, CPlayer::
//          IsMainPlayer @ 0x00487520 / SetAggressiveMode @ 0x00487EB0.
// Layout: retail CCreatureActionBase 0xa8 bytes (PCreature +8); this class's PItem intelligent pointer +0xa8; CThing def
//   +0x70, dead flag +0x91; CCarryableDef carry slot +0x28. Model: the landed CCreatureAction_SheatheItemToInventory::
//   PerformAction 0x0069F4F0 and CCreatureAction_GetPlateFromCupboard::PerformAction 0x00848500.
// Shape: inline GetCreature()/GetItem() accessors everywhere (retail spills &PCreature, keeps `this` in ebp); a same-TU
//   noinline GetSubDef<CCarryableDef> stand-in keeps the carry def in edi; the def pointer is declared before the item
//   local (zero store before the GetPItem call) and its release has no null test (retail dereferences it unchecked).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
struct CPlayer {
    bool IsMainPlayer();                                     // 0x00487520
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
    CPlayer* GetPlayer_004c7a30();                                    // 0x004C7A30
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
    void PrepareWeaponSheathing();                           // 0x005C552C
    void RefreshSheathedWeapons();                           // 0x005C9962
};
struct CTCCombat {
    void SetMeleeFlourishValid(bool valid);                  // 0x00733E10
};
struct CTCCreatureModeManager {
    void RestartCurrentMode();                               // 0x0073A840
};
struct CCreatureActionBase {                                 // retail sizeof 0xa8
    virtual void v00();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_UnsheatheItemFromInventory : CCreatureActionBase {
    CBaseIntelligentPointer PItem;                           // +0xa8
    CThing* GetItem() const { return PItem.GetPItem(); }
    void PerformAction();
};

void CCreatureAction_UnsheatheItemFromInventory::PerformAction()
{
    CTCCarrying* carrying = 0;
    {
        CThing* creature = GetCreature();
        if (creature->HasTC(0x46))
            carrying = creature->GetTC<CTCCarrying>(0x46);
    }
    if (!carrying->HasPendingActions()) {
        CThing* held = carrying->GetThingInPrimarySlot();
        if (held->HasTC(0x48))
            DropWeapon(held, carrying, GetCreature());
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
    if (GetItem() && !(GetItem()->m_flags91 & 1)) {
        CTCInventoryWeapons* weapons = 0;
        {
            CThing* creature = GetCreature();
            if (creature->HasTC(0x13)) {
                weapons = creature->GetTC<CTCInventoryWeapons>(0x13);
                weapons->PrepareWeaponSheathing();
            }
        }
        CDefPointer<CCarryableDef> carryDef;
        CThing* item = GetItem();
        item->m_pDef->GetSubDef(&carryDef);
        carrying->PutThingInCarrySlot(GetItem(), carryDef->CarrySlot, true);
        if (weapons)
            weapons->RefreshSheathedWeapons();
    }
    CTCCombat* combat;
    if (GetCreature()->GetInterface(&combat, 0x30))
        combat->SetMeleeFlourishValid(false);
    CTCCreatureModeManager* modeManager = 0;
    {
        CThing* creature = GetCreature();
        if (creature->HasTC(0x31))
            modeManager = creature->GetTC<CTCCreatureModeManager>(0x31);
    }
    modeManager->RestartCurrentMode();
    if (GetCreature()->GetPlayer_004c7a30()->IsMainPlayer())
        GetCreature()->GetPlayer_004c7a30()->SetAggressiveMode(true);
}