// CTCShop::FrameUpdate() -- per frame: flag the crime display while the live controlled hero stands inside the shop building, switch the alarm off once the shop's village no longer considers the hero a criminal, and on a new day (live hero and shopkeeper) clear, restock and re-display the shop
// Grade: DIFFER(274v274) -- register swap only: retail keeps this in esi and the parent / interface map / &PShopKeeper temporaries in edi, ours the reverse
// Leaf: FrameUpdate
// Retail: Fable.exe (TLC, VC7.1) @ 0x00775B40, 274 bytes (plain `ret`). CTCShop vtable 0x0126A94C slot 9 (the manifest's
//   "CTCShop::vfunc_9"; slot 9 is the CTCBase FrameUpdate).
// Callees: CTCBase::GetGameSubsystem() @ 0x00686D60, CPlayerManager::GetMainPlayer() const @ 0x00449960,
//   CPlayer::GetControlledCreature() @ 0x00487DD0, CThing::GetPosition() @ 0x004C73D0, CThingBuilding::IsPointInside(const
//   C3DVector&) @ 0x0082D000, CPlayerGui::SetCrimeAsDisplaying() @ 0x004376B0 (on g_pPlayerGui @ 0x013B8790),
//   CVectorMap::LowerBound @ 0x0040F020 (interface 0x23 = CTCVillageMember), CTCVillageMember::GetPVillage() const
//   @ 0x007EE540, CTCVillage::IsHeroCriminal() @ 0x0065BFCE, CTCShop::SetAlarm(bool) @ 0x00773C90,
//   CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice), CTCBase::GetWorld() @ 0x00686A70,
//   CTCShop::RemoveStockItemsFromShop @ 0x00773E10, the activation body @ 0x00775350 (landed name OnInitialActivateBody),
//   CTCShop::IsPermanentlyStocked @ 0x007721A0, IS_THING_ALIVE (out of line, `__fastcall`) @ 0x006961F0,
//   CTCShop::StockDisplay @ 0x00774C60.
// Layout: Ego_r.pdb CTCShop with retail containers: PShopKeeper +0x10, DayLastChecked +0x48 (PDB 0x54), Alarm +0x54 (PDB
//   0x60); CThing type byte +0x10 (3 = building), interface bitset +0x20 (0x23 = byte +0x24 bit 3), map +0x44, dead bit
//   +0x91; the world's game-time block at +0x1C with the day at +0x14; the game subsystem's player manager at +0x1C.
// Shape: the hero liveness test is the inline IS_THING_ALIVE(CThing*) at both sites (the hero kept in ebp); the shopkeeper
//   is tested with the inline intelligent-pointer form first and the out-of-line copy (0x006961F0) last.
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                            // +0x10 (3 = building)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;               // 0x004C73D0
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& point);         // 0x0082D000
};
inline bool IS_THING_ALIVE(const CThing* thing)
{
    if (thing != 0) {
        if (!(thing->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CBaseIntelligentPointer {
    void*   __vftable;                                  // +0x00
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
bool __fastcall IsThingAlive_006961f0(CBaseIntelligentPointer& pointer);   // 0x006961F0 (IS_THING_ALIVE out of line)
struct CPlayer {
    CThing* GetControlledCreature();                    // 0x00487DD0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;                     // 0x00449960
};
struct CGameSubsystem {
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                     // +0x1c
};
struct CPlayerGui {
    void SetCrimeAsDisplaying();                        // 0x004376B0
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
struct CGameTime {
    unsigned char _pad00[0x14];
    long          Day;                                  // +0x14
};
struct CWorld {
    unsigned char _pad00[0x1c];
    CGameTime*    PGameTime;                            // +0x1c
};
struct CTCVillage {
    bool IsHeroCriminal();                              // 0x0065BFCE
};
struct CTCVillageMember {
    CTCVillage* GetPVillage() const;                    // 0x007EE540
};
struct CTCShop {
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x10 - 0x08];
    CBaseIntelligentPointer PShopKeeper;                // +0x10
    unsigned char _pad18[0x48 - 0x18];
    long          DayLastChecked;                       // +0x48
    unsigned char _pad4c[0x54 - 0x4c];
    unsigned long Alarm;                                // +0x54
    CGameSubsystem* GetGameSubsystem();                 // 0x00686D60 (CTCBase)
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void SetAlarm(bool on);                             // 0x00773C90
    void RemoveStockItemsFromShop();                    // 0x00773E10
    void OnInitialActivateBody();                       // 0x00775350
    bool IsPermanentlyStocked();                        // 0x007721A0
    void StockDisplay();                                // 0x00774C60
    void FrameUpdate();
};

void CTCShop::FrameUpdate()
{
    CThing* hero = GetGameSubsystem()->PPlayerManager->GetMainPlayer()->GetControlledCreature();
    if (IS_THING_ALIVE(hero)) {
        CThing* parent = ParentThing;
        if (parent->ThingType == 3 && ((CThingBuilding*)parent)->IsPointInside(hero->GetPosition()))
            g_pPlayerGui->SetCrimeAsDisplaying();
    }
    if (Alarm && ParentThing->HasTC(0x23)) {
        CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetPVillage();
        if (village && !village->IsHeroCriminal())
            SetAlarm(false);
    }
    if (IS_THING_ALIVE(hero) && IS_THING_ALIVE(PShopKeeper) && DayLastChecked != GetWorld()->PGameTime->Day) {
        RemoveStockItemsFromShop();
        OnInitialActivateBody();
        if (IsPermanentlyStocked() && IsThingAlive_006961f0(PShopKeeper))
            StockDisplay();
    }
}
