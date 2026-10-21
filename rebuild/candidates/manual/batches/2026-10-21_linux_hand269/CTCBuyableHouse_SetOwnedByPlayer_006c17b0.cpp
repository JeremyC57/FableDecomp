// CTCBuyableHouse::SetOwnedByPlayer(bool owned) -- (un)register the house with the living hero (ownership flag, frame stamp, persisted state, the hero stats' house positions, eviction on sale), refresh the sign post and the player GUI
// Retail 0x006C17B0, 435 bytes (tail `jmp` to CPlayerGui::Refresh with the argument slot reused). Callees: the
// player-manager accessor @ 0x00686D40, CPlayerManager::GetMainPlayer @ 0x00449970, CPlayer::GetHeroCreature @ 0x00487DC0,
// GFGetCurrentFrame @ 0x0049D870 (__fastcall), CThing::SetAsGameStatePersisted(bool) @ 0x004C7880, CTCBase::GetWorld
// @ 0x00686A80, CWorld virtual slot 13 GetWorldMap, CWorldMap::GetCurrentRegionNumber @ 0x004FB150, CWorldMap::GetRegion
// @ 0x004FC180, CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface, CTCHeroStats = 4), CThing::GetPosition @ 0x004C73D0,
// CTCHeroStats::AddHousePosition @ 0x0057CB4B / RemoveHousePosition @ 0x0057ADF7, CTCBuyableHouse::Evict @ 0x006C0FD0,
// CIntelligentPointer<CThing>::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, NeedsMainSignPost @ 0x006BFB90,
// CreateMainSignPost @ 0x006C0F00, CPlayerGui::Refresh(long) @ 0x004378C0 on g_pPlayerGui 0x013B8790.
// Layout: engine/CTCBuyableHouse.h (PDef +0x0c, OwnedByPlayer +0x11, MainSignPost +0x24,
// FrameToNextCheckIfWeNeedToCreateAnything +0x34); the parent thing is CTCBase +4; the def's byte +0x26 asks for eviction.
// Shape: the stats lookup has an unchecked result (the miss path reads the never-written out local, which shares the
// dead `owned` slot with the key); the world is a named local read before the position for the stats call.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
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
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                         // +0x91 bit 0
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
    const C3DVector& GetPosition() const;           // 0x004C73D0
    void Kill(bool b);                              // 0x004C9B80
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
};
struct CPlayer {
    CThing* GetHeroCreature();                      // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer();                       // 0x00449970
};
CPlayerManager* GetPlayerManager();                 // 0x00686D40
long __fastcall GFGetCurrentFrame();                // 0x0049D870

struct CRegion;
struct CWorldMap {
    long GetCurrentRegionNumber();                  // 0x004FB150
    CRegion& GetRegion(long region);                // 0x004FC180
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12();
    virtual CWorldMap* GetWorldMap();               // slot 13 (+0x34)
};
struct CTCHeroStats {
    void AddHousePosition(const CRegion& region, const C3DVector& pos);      // 0x0057CB4B
    void RemoveHousePosition(const CRegion& region, const C3DVector& pos);   // 0x0057ADF7
};
template<class T> struct CIntelligentPointer {
    void* m_pHandle;
    void* m_pExtra;
    T* GetPItem() const;                            // 0x00A01B50
};
struct CPlayerGui {
    void Refresh(long flag);                        // 0x004378C0
};
extern CPlayerGui* g_pPlayerGui;                    // 0x013B8790

struct CBuyableHouseDef {
    unsigned char _pad00[0x26];
    bool          EvictOnSale;                      // +0x26
};

struct CTCBuyableHouse {
    void*            __vftable;                     // +0x00
    CThing*          Thing;                         // +0x04 CTCBase parent thing
    unsigned char    _pad08[0x4];
    CBuyableHouseDef* PDef;                         // +0x0c CDefPointer<CBuyableHouseDef const>
    bool             IsScripted;                    // +0x10
    bool             OwnedByPlayer;                 // +0x11
    bool             Rented;                        // +0x12
    unsigned char    _pad13[0x24 - 0x13];
    CIntelligentPointer<CThing> MainSignPost;       // +0x24
    CIntelligentPointer<CThing> DressingSignPost;   // +0x2c
    long             FrameToNextCheckIfWeNeedToCreateAnything;   // +0x34
    CWorld* GetWorld() const;                       // 0x00686A80
    void Evict();                                   // 0x006C0FD0
    bool NeedsMainSignPost();                       // 0x006BFB90
    void CreateMainSignPost();                      // 0x006C0F00
    void SetOwnedByPlayer(bool owned);
};

void CTCBuyableHouse::SetOwnedByPlayer(bool owned)
{
    CThing* hero = GetPlayerManager()->GetMainPlayer()->GetHeroCreature();
    if (hero && !hero->Dead) {
        if (owned) {
            OwnedByPlayer = true;
            FrameToNextCheckIfWeNeedToCreateAnything = GFGetCurrentFrame();
            Thing->SetAsGameStatePersisted(true);
            long region = GetWorld()->GetWorldMap()->GetCurrentRegionNumber();
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            CWorld* world = GetWorld();
            stats->AddHousePosition(world->GetWorldMap()->GetRegion(region), Thing->GetPosition());
        } else {
            OwnedByPlayer = false;
            FrameToNextCheckIfWeNeedToCreateAnything = GFGetCurrentFrame();
            long region = GetWorld()->GetWorldMap()->GetCurrentRegionNumber();
            CTCHeroStats* stats;
            hero->GetInterface(&stats, 4);
            CWorld* world = GetWorld();
            stats->RemoveHousePosition(world->GetWorldMap()->GetRegion(region), Thing->GetPosition());
            if (PDef->EvictOnSale)
                Evict();
        }
    }
    if (MainSignPost.GetPItem() && !MainSignPost.GetPItem()->Dead)
        MainSignPost.GetPItem()->Kill(true);
    if (NeedsMainSignPost())
        CreateMainSignPost();
    g_pPlayerGui->Refresh(0);
}
