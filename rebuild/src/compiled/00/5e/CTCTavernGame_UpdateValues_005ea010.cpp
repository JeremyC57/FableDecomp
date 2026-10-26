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
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
template <class T> struct CDefOut {                     // the out slot (released by the holder below)
    T* Object;
    CDefOut() : Object(0) { }
};
template <class T> struct CDefHolder {                  // the def read once from the out slot, released at scope end
    T* Object;
    CDefHolder(T* object) : Object(object) { }
    ~CDefHolder() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
class CPrizeScoreDef { public:
    unsigned char _pad00[0x28];
    float Score;                                        // +0x28
    long Mult;                                          // +0x2c
};
class CTavernGameDef : public CDefBase { public:
    unsigned char _pad08[0x170 - 0x08];
    bool BestScoreHigh;                                 // +0x170
    CPrizeScoreDef* PrizeScores_first;                  // +0x174 PrizeScores
    CPrizeScoreDef* PrizeScores_last;                   // +0x178
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out);   // 0x005ED4A0
};
void* FindDef_CDefClassBase_GetSubDef_005ed4a0_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef_005ed4a0(CDefOut<CTavernGameDef>& out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_005ed4a0_0();
    if (!def)
        return false;
    *(void**)&out = def;
    return true;
}
#pragma auto_inline(on)
class CHeroStatsDef { public:
    unsigned char _pad00[0x5c];
    long MaximumMoney;                                  // +0x5c
};
struct CDefinitionHolder {
    unsigned char _pad00[0xdc];
    CHeroStatsDef* PHeroStatsDef;                       // +0xdc CDefPointer<const CHeroStatsDef>
};
class CThing { public:
    unsigned char _pad00[0x70];
    CDefClassBase* PDef;                                // +0x70
};
class CTCTavernGame { public:
    virtual void v00();
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
    CDefinitionHolder* GetDefinitions() const;          // 0x006869D0 (CTCBase)
    bool UpdateValues();
};

bool CTCTavernGame::UpdateValues()
{
    CDefClassBase* thingDef = ParentThing->PDef;
    CDefOut<CTavernGameDef> defOut;
    thingDef->GetSubDef_005ed4a0(defOut);
    CDefHolder<CTavernGameDef> def(defOut.Object);
    bool prizeWon = false;
    if (GameWon) {
        ++NumConsecutiveRoundsWon;
        for (CPrizeScoreDef* prize = def->PrizeScores_first; prize != def->PrizeScores_last; ++prize) {
            if (def->BestScoreHigh ? Score >= prize->Score : Score <= prize->Score) {
                CurrentMoney += BaseBet * prize->Mult;
                if (CurrentMoney > GetDefinitions()->PHeroStatsDef->MaximumMoney)
                    CurrentMoney = GetDefinitions()->PHeroStatsDef->MaximumMoney;
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