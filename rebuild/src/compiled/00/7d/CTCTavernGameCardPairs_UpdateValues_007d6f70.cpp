// CTCTavernGameCardPairs::UpdateValues() -- settle the round through CTCTavernGame::UpdateValues, award the pairs prize once (lower time wins), report the pairs time and winning streak to the online scoreboard and keep the best pairs score
// Leaf: ?UpdateValues@CTCTavernGameCardPairs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D6F70, 359 bytes (plain `ret`).
// Callees: CTCTavernGame::UpdateValues() 0x005EA010 (non-virtual base call); CTCTavernGame::GetHeroStats(CTCHeroStats**)
//          0x005EB4B0; CTCTavernGame::GetDef(CDefPointer<CTavernGameDef>*) 0x005EA000 (out of line);
//          CTCTavernGame::GivePrize() 0x005EB590; CPlayerManager::GetMainPlayer() const 0x00449960;
//          CPlayer::GetHeroThing() 0x00487DC0; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool) 0x0055F3B0; the def's slot 1.
// Layout (retail CTCTavernGame, as the landed 0x005EA010 / 0x007DAB60): GameWon +0x21, Score +0x2c,
//          NumConsecutiveRoundsWon +0xa4; CTavernGameDef PrizeScores +0x174 (0x30-byte entries, Score +0x28);
//          CTCHeroStats (retail) PrizeGivenPairs +0x194, BestTimePairs +0x178 (Ego_r.pdb - 0x18);
//          CTCTavernGameCardBase Counter +0x154 (the round time).
// Label: the catalog calls this CTCTavernGameShoveHaPenny, but the row sits in CTCTavernGameCardPairs vtable 0x0126F6FC
//          slot 40 and touches the pairs stats (labels swapped with 0x007D0850).
class CTCBase;

struct CInterfaceMap {                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);    // 0x0040F020
};

class CThing { public:
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

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
class CPlayer { public:
    CThing* GetHeroThing();                  // 0x00487DC0
};
class CPlayerManager { public:
    CPlayer* GetMainPlayer() const;          // 0x00449960
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

class CPrizeScoreDef { public:
    unsigned char _pad_0x00[0x28];
    float Score;                             // +0x28
    float Mult;                              // +0x2c
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                  // slot 1
};
class CTavernGameDef : public CDefVirtual { public:
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

class CTCHeroStats { public:                        // retail layout
    unsigned char _pad_0x00[0x178];
    float BestTimePairs;                     // +0x178
    unsigned char _pad_0x17c[0x18];
    bool  PrizeGivenPairs;                   // +0x194
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0x1d];
    bool          GameWon;                   // +0x21
    unsigned char _pad_0x22[0xa];
    float         Score;                     // +0x2c
    unsigned char _pad_0x30[0x74];
    long          NumConsecutiveRoundsWon;   // +0xa4
    unsigned char _pad_0xa8[0xac];
    float         Counter;                   // +0x154 (CTCTavernGameCardBase)
    bool UpdateValues();                     // 0x005EA010
    void GetHeroStats(CTCHeroStats** stats); // 0x005EB4B0
    void GetDef(CDefPointer* out);           // 0x005EA000
    void GivePrize();                        // 0x005EB590
};
struct CTCTavernGameCardPairs : CTCTavernGame {
    bool UpdateValues();
};

bool CTCTavernGameCardPairs::UpdateValues()
{
    bool result = CTCTavernGame::UpdateValues();
    CTCHeroStats* stats = 0;
    GetHeroStats(&stats);
    CDefPointer def;
    GetDef(&def);
    if (GameWon) {
        if (!stats->PrizeGivenPairs && Score <= def->PrizeScores[0].Score) {
            stats->PrizeGivenPairs = true;
            GivePrize();
        }
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetHeroThing();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        scoreboard->UpdateScoreForActivity(0x12, Counter, true);
        scoreboard->UpdateScoreForActivity(0x1a, (float)NumConsecutiveRoundsWon, false);
        if (stats->BestTimePairs == 0.0f || Score < stats->BestTimePairs) {
            stats->BestTimePairs = Counter;
            return true;
        }
    }
    return result;
}