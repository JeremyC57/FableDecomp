// Grade: DIFFER(467v463) -- the presence-bit test of the bool HasTC: ours and retail differ in the shr/test form of the masked word (an unsigned long HasTC fixes that but flips esi/edi); everything else matches
// CTCActionUseBuyHouse::ReceivedConfirmation(CTCBuyableHouse* house) -- apply the hero's yes to the pending house question: buy/sell the house, toggle renting, or pay for the next dress level
// Retail 0x0072EB40, 464 bytes (`ret 4`). Callees: the player-manager accessor @ 0x00686D40 (game singleton
// 0x013B8A1C +0x30), CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface, hero stats = type 4),
// CTCBuyableHouse::GetPrice @ 0x006C0120, GetPriceForNextDressLevel @ 0x006BF920, IncrementDressLevel @ 0x006C04B0,
// SetRented @ 0x006C1150, SetOwnedByPlayer @ 0x006C17B0, CTCHeroStats::AddMoney @ 0x0057B327.
// Layout: CTCActionUseBuyHouse HouseQuestion +0x1c (Ego_r.pdb); CTCBuyableHouse OwnedByPlayer +0x11, Rented +0x12
// (engine/CTCBuyableHouse.h), parent thing +4 (CTCBase). CTCHeroStats retail +0xe0/+0xe4 are the PDB's
// NumberOfShopsOwned/NumberOfHousesOwned (+0xe8/+0xec; retail is 8 bytes lower there). The house thing's
// interface 0x53 bit (+0x28 bit 19) picks the shop counter.
struct CTCBase;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
    __forceinline bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
};

struct CPlayer {
    CThing* GetHeroCreature();                      // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};
CPlayerManager* GetPlayerManager();                 // 0x00686D40

struct CTCHeroStats {
    unsigned char _pad0[0xe0];
    long NumberOfShopsOwned;                        // +0xe0
    long NumberOfHousesOwned;                       // +0xe4
    void AddMoney(long amount);                     // 0x0057B327
};

struct CTCBuyableHouse {
    void*         __vftable;                        // +0x00
    CThing*       Thing;                            // +0x04 CTCBase parent thing
    unsigned char _pad08[0x11 - 0x08];
    bool          OwnedByPlayer;                    // +0x11
    bool          Rented;                           // +0x12
    long GetPriceForNextDressLevel(bool b);         // 0x006BF920
    void IncrementDressLevel();                     // 0x006C04B0
    void SetRented(bool rented);                    // 0x006C1150
    long GetPrice(bool b);                          // 0x006C0120
    void SetOwnedByPlayer(bool owned);              // 0x006C17B0
};

struct CTCActionUseBuyHouse {
    unsigned char _pad0[0x1c];
    long          HouseQuestion;                    // +0x1c
    void ReceivedConfirmation(CTCBuyableHouse* house);
};

void CTCActionUseBuyHouse::ReceivedConfirmation(CTCBuyableHouse* house)
{
    switch (HouseQuestion) {
    case 1:
        if (!house->OwnedByPlayer) {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
            CTCHeroStats* stats;
            if (hero && hero->GetInterface(&stats, 4)) {
                stats->AddMoney(-house->GetPrice(true));
                house->SetOwnedByPlayer(true);
                if (house->Thing->HasTC(0x53))
                    ++stats->NumberOfShopsOwned;
                else
                    ++stats->NumberOfHousesOwned;
            }
        } else {
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
            CTCHeroStats* stats;
            if (hero && hero->GetInterface(&stats, 4)) {
                stats->AddMoney(house->GetPrice(true));
                house->SetOwnedByPlayer(false);
                if (house->Rented)
                    house->SetRented(false);
                if (house->Thing->HasTC(0x53))
                    --stats->NumberOfShopsOwned;
                else
                    --stats->NumberOfHousesOwned;
            }
        }
        break;
    case 2:
        house->SetRented(!house->Rented);
        break;
    case 3: {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
        CTCHeroStats* stats;
        if (hero && hero->GetInterface(&stats, 4)) {
            stats->AddMoney(-house->GetPriceForNextDressLevel(true));
            house->IncrementDressLevel();
        }
        break;
    }
    }
}
