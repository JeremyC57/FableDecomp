// CTCTavernGameSpotTheAddition::UpdateValues() -- settle the round through CTCTavernGame::UpdateValues, award the guess-the-addition prize once (only when prizes are enabled; lower time wins), report score and winning streak to the online scoreboard and keep the best sorting time
// Leaf: ?UpdateValues@CTCTavernGameSpotTheAddition
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007D8AB0, 362 bytes (plain `ret`).
// Callees: CTCTavernGame::UpdateValues() 0x005EA010 (non-virtual base call); CTCTavernGame::GetHeroStats(CTCHeroStats**)
//          0x005EB4B0; CTCTavernGame::GetDef(CDefPointer<CTavernGameDef>*) 0x005EA000 (out of line);
//          CTCTavernGame::GivePrize() 0x005EB590; CPlayerManager::GetMainPlayer() const 0x00449960;
//          CPlayer::GetPControlledCreature() 0x00487DC0; CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020;
//          CTCHeroOnlineScoreboard::UpdateScoreForActivity(long, float, bool) 0x0055F3B0; the def's slot 1.
// Layout (retail CTCTavernGame, as the landed 0x005EA010 / 0x007DAB60): GameWon +0x21, Score +0x2c,
//          NumConsecutiveRoundsWon +0xa4; CTavernGameDef PrizeScores +0x174 (0x30-byte entries, Score +0x28);
//          CTCHeroStats (retail) PrizeGivenGuessTheAddition +0x19a, BestTimeGuessTheAddition +0x190 (Ego_r.pdb - 0x18).
// Globals: a prize-enable byte @ 0x013833D0 and a "scoreboard updated" byte @ 0x013BAE25 (stored after the reports).
// Layout (supersedes any local-model note above): engine/CTCHeroStats.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCHeroStats CTCHeroStatsDonor
#include "engine/CTCHeroStats.h"
#undef CTCHeroStats
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                       // CVectorMap<ETCInterfaceType, CTCBase*>
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);    // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];          // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;              // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                  // 0x00487DC0
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;          // 0x00449960
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern bool g_bTavernPrizesEnabled;                // 0x013833D0
extern bool g_bTavernScoreReported;                // 0x013BAE25

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

class CTCHeroStats : public CTCHeroStatsDonor {
 public:                        // retail layout
};

class CTCTavernGame { public:
    virtual void v00();
    unsigned char _pad_0x04[0x1d];
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
struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    bool UpdateValues();
};

bool CTCTavernGameSpotTheAddition::UpdateValues()
{
    bool result = CTCTavernGame::UpdateValues();
    CTCHeroStats* stats;
    GetHeroStats(&stats);
    CDefPointer def;
    GetDef(&def);
    if (GameWon) {
        if (g_bTavernPrizesEnabled && !stats->PrizeGivenGuessTheAddition && Score <= def->PrizeScores[0].Score) {
            stats->PrizeGivenGuessTheAddition = true;
            GivePrize();
        }
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        scoreboard->UpdateScoreForActivity(0x14, Score, true);
        scoreboard->UpdateScoreForActivity(0x1c, (float)NumConsecutiveRoundsWon, false);
        g_bTavernScoreReported = true;
        if (stats->BestTimeGuessTheAddition == 0.0f || Score < stats->BestTimeGuessTheAddition) {
            stats->BestTimeGuessTheAddition = Score;
            return true;
        }
    }
    return result;
}