// CTCTavernGame::UpdateValues() -- settle the round's bet: a won round pays the first prize score the score reaches (money capped at the hero maximum, winnings grow); a lost round loses the bet; returns whether a prize was paid
// Leaf: UpdateValues
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005EA010, 238 bytes (plain `ret`).
// Callees: CDefClassBase::GetSubDef_005ed4a0(CDefPointer&) @ 0x005ED4A0 (the 'CTavernGameDef' instantiation),
// CTCBase::GetDefinitions const @ 0x006869D0 (the game definitions; PHeroStatsDef at +0xdc -- landed spelling); the def's
// virtual slot 1.
// Layout (retail; Ego_r.pdb CTCTavernGame less the retail 12-byte containers, as the landed ClearBox/OnSerialise): GameWon
// +0x21, Score +0x2c, BaseBet +0x34, LastBet +0x38, TotalWinnings +0x3c, CurrentMoney +0x40, NumConsecutiveRoundsWon +0xa4;
// CThing PDef +0x70; CTavernGameDef BestScoreHigh +0x170, PrizeScores (vector<CPrizeScoreDef>, 0x30-byte entries: Score +0x28,
// Mult +0x2c) +0x174; CHeroStatsDef MaximumMoney +0x5c.
// Shape: the thing def is read into a local before the out slot (its zero store sinks below the push); the tavern def is
// read once from its out slot into a holder (ebp) released at the end; the prize search caches
// the vector end and the BestScoreHigh flag; the qualifying test is the ternary of `>=` (high scores win) and `<=`; both
// round outcomes share the TotalWinnings store.
// NF170 (q4278, 2026-11-20): the sub-def call 0x005ED4A0 is the shared CDefClassBase::GetSubDef<CTavernGameDef> template (Ego_r.pdb +
//   FableWin.pdb ??$GetSubDef@VCTavernGameDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z; the callee pushes the literal
//   'CTavernGameDef'), its noinline body visible (engine/CDefClassBase_GetSubDef.h FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE), on the
//   shared CDefPointer / CTavernGameDef (Ego_r member names; PrizeScores typed std::vector<CPrizeScoreDef> by the overlay opt-in
//   FABLE_RETAIL_CTAVERNGAMEDEF_PRIZESCORES_VECTOR, engine/CPrizeScoreDef.h Score +0x28 / Mult +0x2C) and fable_string.h. Was a local def / CDefPointer / CDefClassBase model (and
//   local string classes) with a stand-in GetSubDef body.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CTAVERNGAMEDEF_PRIZESCORES_VECTOR
#include "fable_string.h"
#include "engine/CTavernGameDef.h"
#include "engine/CDefClassBase_GetSubDef.h"
class CHeroStatsDef { public:
    unsigned char _pad00[0x5c];
    long MaximumMoney;                                  // +0x5c
};
class CGameDefinitionManager { public:
    unsigned char _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;                       // +0xdc CDefPointer<const CHeroStatsDef>
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCBase
{
public:
    virtual void v00();
protected:
    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0
};

class CTCTavernGame : public CTCBase { public:
    CThing* ParentThing;                                // +0x04 (CTCBase)
    unsigned char _pad08[0x20 - 0x08];
    bool IsBeingPlayed;                                 // +0x20
    bool GameWon;                                       // +0x21
    unsigned char _pad22[0x2c - 0x22];
    float Score;                                        // +0x2c
    float Target;                                       // +0x30
    long BaseBet;                                       // +0x34
    long LastBet;                                       // +0x38
    long TotalWinnings;                                 // +0x3c
    long CurrentMoney;                                  // +0x40
    unsigned char _pad44[0xa4 - 0x44];
    long NumConsecutiveRoundsWon;                       // +0xa4
    bool UpdateValues();
};

bool CTCTavernGame::UpdateValues()
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefPointer<const CTavernGameDef> def;
    thingDef->GetSubDef(&def);
    bool prizeWon = false;
    if (GameWon) {
        ++NumConsecutiveRoundsWon;
        for (std::vector<CPrizeScoreDef>::const_iterator prize = def->PrizeScores.begin(); prize != def->PrizeScores.end(); ++prize) {
            if (def->BestScoreHigh ? Score >= prize->Score : Score <= prize->Score) {
                CurrentMoney += BaseBet * prize->Mult;
                if (CurrentMoney > GetDefinitionManager().PHeroStatsDef->MaximumMoney)
                    CurrentMoney = GetDefinitionManager().PHeroStatsDef->MaximumMoney;
                TotalWinnings += (prize->Mult - 1) * BaseBet;
                prizeWon = true;
                break;
            }
        }
    } else {
        TotalWinnings -= BaseBet;
        NumConsecutiveRoundsWon = 0;
    }
    LastBet = BaseBet;
    BaseBet = 0;
    return prizeWon;
}