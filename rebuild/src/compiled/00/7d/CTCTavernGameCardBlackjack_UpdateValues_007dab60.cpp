// CTCTavernGameCardBlackjack::UpdateValues() -- after a won sitting reports the winnings and the winning streak to the hero's online scoreboard, then rolls the bet over (last bet = bet, bet = 0)
// Leaf: UpdateValues
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pMainGameComponent (0x013B86A0) is the protected static CMainGameComponent::PMainGameComponent, read through the
//   inline static accessor Get() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DAB60, length 163 bytes (plain `ret`); CTCTavernGameCardBlackjack vtable
// 0x0126F8F4 slot 40 (the catalog's "vfunc_40"). Name from Ego_r.pdb `?UpdateValues@CTCTavernGameCardBlackjack@@UAE_NXZ`
// (Ego_r's source order: ... HasPlayerDrawnHand, LeaveGame, EndRound, UpdateValues; retail EndRound is 0x007DAAE0).
// Callees: CPlayerManager::GetMainPlayer() const @ 0x00449960, CPlayer::GetPControlledCreature() @ 0x00487DC0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, CTCHeroOnlineScoreboard::UpdateScoreForActivity(
// EOnlineScoreboardActivity, float, bool) @ 0x0055F3B0 (activities 0x18 and 0x20). Global: CMainGameComponent::Get()
// @ 0x013B86A0 (player manager at +0x1c).
// Layout (retail CTCTavernGame; local model because engine/CTCTavernGame.h carries Ego_r.pdb offsets and retail's
// 12-byte vectors put everything after UIComponents 8..0x1c bytes lower): GameWon +0x21, BaseBet +0x34, LastBet +0x38,
// TotalWinnings +0x3c, NumConsecutiveRoundsWon +0xa4. Thing: interface bitset +0x20 (0x10D = word 8 bit 13, `test ch,20h`),
// interface map +0x44.
// Shape: the unchecked out-parameter GetInterface (the absent arm reads the never-written scoreboard pointer from the
// key's `push ecx` frame word); the winnings are read into a `long` local, and its `fild` needs memory, so VC parks it in
// that same dead frame word; the winning streak is `fild`ed straight from the member. Returns false.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };

class CTCHeroOnlineScoreboard { public:
    void UpdateScoreForActivity(long activity, float score, bool onlyIfBetter);   // 0x0055F3B0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                  // 0x00487DC0
};
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
class CMainGameComponent {
protected:
    static CMainGameComponent* PMainGameComponent;      // 0x013B86A0 ?PMainGameComponent@CMainGameComponent@@1PAV1@A (protected static; ESTABLISHED_SPELLINGS)
public:
    static CMainGameComponent* __fastcall Get() { return PMainGameComponent; }   // Ego_r / FableWin ?Get@CMainGameComponent@@SIPAV1@XZ (inline)
private: public:
    unsigned char   _pad_0x00[0x1c];
    CPlayerManager* PlayerManager;           // +0x1c
    const CPlayerManager& PeekPlayerManager() const { return *PlayerManager; }   // FableWin.pdb ?PeekPlayerManager@CMainGameComponent@@QBEABVCPlayerManager@@XZ
};

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
        CThing* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCHeroOnlineScoreboard* scoreboard;
        hero->GetTC(TCI_HERO_ONLINE_SCOREBOARD, &scoreboard);
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