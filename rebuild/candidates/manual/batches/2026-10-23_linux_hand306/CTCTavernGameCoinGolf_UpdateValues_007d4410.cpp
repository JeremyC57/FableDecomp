// CTCTavernGameCoinGolf::UpdateValues() -- settle the round through the base CTCTavernGame::UpdateValues; on a won coin-golf table (the plain or the Snowspire one, by def name) award the prize once (lower score wins), report score and winning streak to the online scoreboard and keep the hero's best (lowest) coin-golf score
// Leaf: ?UpdateValues@CTCTavernGameCoinGolf
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D4410, 693 bytes (+ int3 padding), plain `ret`; referenced from the CoinGolf vtable
//   at 0x0126F53C (the catalog's "CTCTavernGameShoveHaPenny" label is wrong; the ShoveHaPenny one is 0x007D0850).
// Callees: CTCTavernGame::UpdateValues() 0x005EA010; GetHeroStats 0x005EB4B0; GetDef 0x005EA000; CThing::GetDefName
//   0x004C7CC0; CDefString::operator==(const char*) 0x005B3440; GivePrize 0x005EB590; GetMainPlayer 0x00449960;
//   GetHeroThing 0x00487DC0; LowerBound 0x0040F020; UpdateScoreForActivity 0x0055F3B0.
// Layout: as the landed ShoveHaPenny UpdateValues 0x007D0850; CTCHeroStats (retail) BestScoreCoinGolf +0x184,
//   BestScoreCoinGolfSnowspire +0x188, PrizeGivenCoinGolf +0x197, PrizeGivenCoinGolfSnowspire +0x198.
struct CDefString {                                 // 4-byte def-name handle (user constructor: hidden-pointer return)
    long m_tablePos;
    CDefString();
    bool operator==(const char* text) const;        // 0x005B3440
};
struct CTCBase;

struct CInterfaceMap {                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);    // 0x0040F020
};

struct CThing {
    CDefString GetDefName() const;           // 0x004C7CC0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
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

struct CTCHeroOnlineScoreboard {
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
struct CPlayer {
    CThing* GetHeroThing();                  // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetMainPlayer() const;          // 0x00449960
};
struct CMainGameComponent {
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CPrizeScoreDef {
    unsigned char _pad_0x00[0x28];
    float Score;                             // +0x28
    float Mult;                              // +0x2c
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                  // slot 1
};
struct CTavernGameDef : CDefVirtual {
    long m_count;                            // +0x04
    unsigned char _pad_0x08[0x16c];
    CPrizeScoreDef* PrizeScores;             // +0x174 (vector _M_start)
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CTavernGameDef* Object;
    CDefPointer() : Object(0) {}
    ~CDefPointer() { if (Object) Object->Release(); }
    CTavernGameDef* operator->() const { return Object; }
};

struct CTCHeroStats {                        // retail layout
    unsigned char _pad_0x00[0x184];
    float BestScoreCoinGolf;                 // +0x184
    float BestScoreCoinGolfSnowspire;        // +0x188
    unsigned char _pad_0x18c[0x197 - 0x18c];
    bool  PrizeGivenCoinGolf;                // +0x197
    bool  PrizeGivenCoinGolfSnowspire;       // +0x198
};

struct CTCTavernGame {
    virtual void v00();
    CThing*       ParentThing;               // +0x04
    unsigned char _pad_0x08[0x19];
    bool          GameWon;                   // +0x21
    unsigned char _pad_0x22[0xa];
    float         Score;                     // +0x2c
    unsigned char _pad_0x30[0x74];
    long          NumConsecutiveRoundsWon;   // +0xa4
    bool UpdateValues();                     // 0x005EA010
    void GetHeroStats(CTCHeroStats** stats); // 0x005EB4B0
    void GetDef(CDefPointer* out);           // 0x005EA000
    void GivePrize();                        // 0x005EB590
};
struct CTCTavernGameCoinGolf : CTCTavernGame {
    bool UpdateValues();
};

bool CTCTavernGameCoinGolf::UpdateValues()
{
    bool result = CTCTavernGame::UpdateValues();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CDefPointer def;
    GetDef(&def);
    if (ParentThing->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF") {
        if (GameWon) {
            if (!stats->PrizeGivenCoinGolf && Score <= def->PrizeScores[0].Score) {
                stats->PrizeGivenCoinGolf = true;
                GivePrize();
            }
            CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
            CTCHeroOnlineScoreboard* scoreboard;
            hero->GetInterface(&scoreboard, 0x10d);
            scoreboard->UpdateScoreForActivity(0x16, Score, true);
            scoreboard->UpdateScoreForActivity(0x1e, (float)NumConsecutiveRoundsWon, false);
            if (stats->BestScoreCoinGolf == 0.0f || Score < stats->BestScoreCoinGolf) {
                stats->BestScoreCoinGolf = Score;
                return true;
            }
        }
    } else if (ParentThing->GetDefName() == "OBJECT_TAVERN_TABLE_COIN_GOLF_SNOWSPIRE" && GameWon) {
        if (!stats->PrizeGivenCoinGolfSnowspire && Score <= def->PrizeScores[0].Score) {
            stats->PrizeGivenCoinGolfSnowspire = true;
            GivePrize();
        }
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        scoreboard->UpdateScoreForActivity(0x17, Score, true);
        scoreboard->UpdateScoreForActivity(0x1f, (float)NumConsecutiveRoundsWon, false);
        if (stats->BestScoreCoinGolfSnowspire == 0.0f || Score < stats->BestScoreCoinGolfSnowspire) {
            stats->BestScoreCoinGolfSnowspire = Score;
            return true;
        }
    }
    return result;
}
