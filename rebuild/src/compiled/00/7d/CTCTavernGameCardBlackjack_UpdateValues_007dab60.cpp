// CTCTavernGameCardBlackjack::UpdateValues() -- after a won sitting reports the winnings and the winning streak to the hero's online scoreboard, then rolls the bet over (last bet = bet, bet = 0)
// Leaf: UpdateValues
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DAB60, length 163 bytes (plain `ret`); CTCTavernGameCardBlackjack vtable
// 0x0126F8F4 slot 40 (the catalog's "vfunc_40"). Name from Ego_r.pdb `?UpdateValues@CTCTavernGameCardBlackjack@@UAE_NXZ`
// (Ego_r's source order: ... HasPlayerDrawnHand, LeaveGame, EndRound, UpdateValues; retail EndRound is 0x007DAAE0).
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::GetPControlledCreature() @ 0x00487DC0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCHeroOnlineScoreboard::UpdateScoreForActivity(
// EOnlineScoreboardActivity, float, bool) @ 0x0055F3B0 (activities 0x18 and 0x20). Global: g_pMainGameComponent
// @ 0x013B86A0 (player manager at +0x1c).
// Layout (retail CTCTavernGame; local model because engine/CTCTavernGame.h carries Ego_r.pdb offsets and retail's
// 12-byte vectors put everything after UIComponents 8..0x1c bytes lower): GameWon +0x21, BaseBet +0x34, LastBet +0x38,
// TotalWinnings +0x3c, NumConsecutiveRoundsWon +0xa4. Thing: interface bitset +0x20 (0x10D = word 8 bit 13, `test ch,20h`),
// interface map +0x44.
// Shape: the unchecked out-parameter GetInterface (the absent arm reads the never-written scoreboard pointer from the
// key's `push ecx` frame word); the winnings are read into a `long` local, and its `fild` needs memory, so VC parks it in
// that same dead frame word; the winning streak is `fild`ed straight from the member. Returns false.
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
    const CPlayer& GetMainPlayer() const;          // 0x00449960 (const)
};
class CMainGameComponent { public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

struct CTCTavernGameCardBlackjack {         // vptr +0x00 (CTCBase)
    unsigned char _pad_0x04[0x1c];
    bool          IsBeingPlayed;             // +0x20
    bool          GameWon;                   // +0x21
    unsigned char _pad_0x22[0x12];
    long          BaseBet;                   // +0x34
    long          LastBet;                   // +0x38
    long          TotalWinnings;             // +0x3c
    unsigned char _pad_0x40[0x64];
    long          NumConsecutiveRoundsWon;   // +0xa4
    virtual bool UpdateValues();
};

bool CTCTavernGameCardBlackjack::UpdateValues()
{
    if (GameWon) {
        CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetInterface(&scoreboard, 0x10d);
        long winnings = TotalWinnings;
        if (winnings > 0) {
            scoreboard->UpdateScoreForActivity(0x18, (float)winnings, false);
            scoreboard->UpdateScoreForActivity(0x20, (float)NumConsecutiveRoundsWon, false);
        }
    }
    LastBet = BaseBet;
    BaseBet = 0;
    return false;
}