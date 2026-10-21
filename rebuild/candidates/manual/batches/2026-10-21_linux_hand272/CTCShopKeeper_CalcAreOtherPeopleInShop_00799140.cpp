// CTCShopKeeper::CalcAreOtherPeopleInShop() -- true when some other living villager of the keeper's village is inside the shop building without living there (or outside while living there), or blocks its door, or when the hero is in the shop or blocks its door
// Retail: Fable.exe @ 0x00799140, 480 bytes (plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, LowerBound 0x0040F020 (x2), CTCVillageMember::GetPVillage 0x007EE540,
//   CThing::GetPosition 0x004C73D0, CThingBuilding::IsPointInside 0x0082D000, CThingBuilding::IsDoorBlockedBy 0x0082D9C0,
//   CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetHeroCreature 0x00487DC0.
// Layout (retail CThing, 0x10 below the PDB): state byte +0x10, TC bitset +0x20 (bit 0x23 = village member at +0x24 bit 3,
//   bit 0x75 at +0x2c bit 21), interface map +0x44, Dead +0x91 bit 0, home building pointer +0x170; CVillage villager list
//   header +0x2c (node data at +0x08); TC 0x75's building pointer +0x0c.
// Shape: the two TC lookups are the out-parameter GetInterface (bit test, key local, map pointer, one end/hit ternary on
//   the constant type), which lets the village pointer's spill share the key's frame word (sub esp,0xc); the building test
//   is an inline IsThingAlive (keeps retail's second inline `return false` exit); the villager is a named local before
//   its home-building read (`lea ecx,[eax+0x170]` instead of mov/add).
struct C3DVector { float X, Y, Z; };
struct CBaseObject;
struct CThing;
struct CBaseIntelligentPointer {
    void* __vftable;
    CBaseObject* m_pItem;
    CThing* GetPItem() const;                       // 0x00A01B50
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first; SEntry* last; SEntry* end;
    SEntry* LowerBound(const long& key);            // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char State;                            // +0x10
    unsigned char _pad11[0x0f];
    unsigned long TCBits[4];                        // +0x20
    unsigned char _pad30[0x14];
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad50[0x41];
    unsigned char Dead : 1;                         // +0x91 bit 0
    unsigned char _pad92[0x170 - 0x92];
    CBaseIntelligentPointer HomeBuilding;           // +0x170
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (TCBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(key);
            *out = (T*)(it == map->last || type < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;           // 0x004C73D0
};
struct CThingBuilding : CThing {
    bool IsPointInside(const C3DVector& pos);       // 0x0082D000
    bool IsDoorBlockedBy(CThing* thing);            // 0x0082D9C0
};
__forceinline bool IsThingAlive(const CThing* thing) { return thing && !thing->Dead; }
struct CVillagerNode { CVillagerNode* _M_next; CVillagerNode* _M_prev; CBaseIntelligentPointer Villager; };
struct CVillage {
    unsigned char _pad00[0x2c];
    CVillagerNode* Villagers;                       // +0x2c (list header)
};
struct CTCVillageMember { CVillage* GetPVillage(); };   // 0x007EE540
struct CTCHeroBuilding { unsigned char _pad00[0x0c]; CBaseIntelligentPointer Building; };   // TC 0x75, +0x0c
struct CPlayer { CThing* GetHeroCreature(); };      // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };   // 0x00449970
struct CTCShopKeeper {
    void*   __vftable;
    CThing* ParentThing;                            // +0x04
    CPlayerManager* GetPlayerManager();             // 0x00686D40
    bool CalcAreOtherPeopleInShop();
};

bool CTCShopKeeper::CalcAreOtherPeopleInShop()
{
    CThing* owner = ParentThing;
    if (owner && !owner->Dead && owner->State != 1)
        return false;
    CThingBuilding* building = (CThingBuilding*)owner->HomeBuilding.GetPItem();
    if (!IsThingAlive(building))
        return false;
    CTCVillageMember* member;
    if (owner->GetInterface(0x23, &member))
    {
        CVillage* village = member->GetPVillage();
        if (village)
        {
            for (CVillagerNode* node = village->Villagers->_M_next; node != village->Villagers; node = node->_M_next)
            {
                CBaseIntelligentPointer& villager = node->Villager;
                if (villager.GetPItem() && !villager.GetPItem()->Dead && villager.GetPItem() != owner)
                {
                    CThing* other = villager.GetPItem();
                    CThing* home = other->HomeBuilding.GetPItem();
                    if (building->IsPointInside(villager.GetPItem()->GetPosition()) != (home == building))
                        return true;
                }
                if (villager.GetPItem() && !villager.GetPItem()->Dead && villager.GetPItem() != owner)
                {
                    if (building->IsDoorBlockedBy(villager.GetPItem()))
                        return true;
                }
            }
        }
    }
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    CTCHeroBuilding* heroBuilding;
    if (hero && !hero->Dead && hero->GetInterface(0x75, &heroBuilding))
    {
        if (heroBuilding->Building.GetPItem() == building)
            return true;
        if (building->IsDoorBlockedBy(hero))
            return true;
    }
    return false;
}
