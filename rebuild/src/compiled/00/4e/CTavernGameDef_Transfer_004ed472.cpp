// CTavernGameDef::Transfer(CPersistContext& context) -- persist every tavern-game def field (phrases, fonts, layout floats, prize table) through the typed CPersistContext transfers
// Leaf: ?Transfer@CTavernGameDef@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004ED472, 1247 bytes, `ret 4`, virtual Transfer of CTavernGameDef.
// Callees: CPersistContext::Transfer(unsigned long&) 0x00431020 (57 text-entry/graphic ids),
//   Transfer(CDefString&) 0x0045228F (CameraName and the three fonts), Transfer(long&) 0x00431102 (bets, prize),
//   Transfer(float&) 0x00431061 (screen positions), Transfer(bool&) 0x0043314A (BestScoreHigh, DisplayErrata),
//   TransferVectorOfSubComponents<CPrizeScoreDef>(vector//   Transfer(vector<CPrizeScoreDef>&) 0x004ED953 (the TransferVectorOfSubComponents instance emitted right) 0x004ED953 (the template instance emitted right
//   after this body; its loop strides 0x30-byte prize defs).  The catalog's "UpdateShadowScene" labels on
//   0x431020/0x431061/0x431102/0x43314A/0x45228F are propagated; landed def Transfers bind them by type.
// Shape: no base Transfer call; one `lea eax,[esi+N] ; push eax ; mov ecx,edi ; call` per member in layout
// order, the last member's address formed with `add esi,1A0h` (the natural &PointerPhaseSpeed of the final call).
// Size-optimised def TU (unaligned retail address, no padding): `#pragma optimize("s", on)`.
// Layout: Ego_r.pdb CTavernGameDef (0x1A8) with the retail 12-byte STLport vector: PrizeScores +0x174 and
// everything after it sit 4 bytes below the PDB offsets (Prize +0x180 .. PointerPhaseSpeed +0x1A0; retail 0x1A4).
// No engine header exists for this def; the def base is an opaque 0x24-byte body after the vptr.
#pragma optimize("s", on)

struct CDefString { long m_index; };
class CPrizeScoreDef;
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };   // retail 12-byte STLport vector

class CPersistContext {
public:
    void Transfer(unsigned long& value);          // 0x00431020
    void Transfer(CDefString& v);                 // 0x0045228F
    void Transfer(long& value);                   // 0x00431102
    void Transfer(float& value);                  // 0x00431061
    void Transfer(bool& value);                   // 0x0043314A
    template <class T> void TransferVectorOfSubComponents(vector<T>& value);   // <CPrizeScoreDef> 0x004ED953
};

class CTavernGameDef {
public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _defBase[0x24];                 // +0x04 CParentDefClassBase body after the vptr
    unsigned long Banter;                         // +0x28
    unsigned long Greeting;                       // +0x2c
    unsigned long OptionsInitial;                 // +0x30
    unsigned long OptionsSubsequent;              // +0x34
    unsigned long Instructions;                   // +0x38
    unsigned long InstructionsPC;                 // +0x3c
    unsigned long Betting;                        // +0x40
    unsigned long Play;                           // +0x44
    unsigned long ReactionWin;                    // +0x48
    unsigned long ReactionLose;                   // +0x4c
    unsigned long ReactionDraw;                   // +0x50
    unsigned long ReactionWinNewBestScore;        // +0x54
    unsigned long FarewellInitial;                // +0x58
    unsigned long FarewellSubsequent;             // +0x5c
    unsigned long WinRoundPhrase;                 // +0x60
    unsigned long OutOfTimePhrase;                // +0x64
    unsigned long NoMoney;                        // +0x68
    unsigned long GreetingForward;                // +0x6c
    unsigned long OptionsForward;                 // +0x70
    unsigned long OptionsBack;                    // +0x74
    unsigned long OptionsAlternative;             // +0x78
    unsigned long InstructionsBack;               // +0x7c
    unsigned long BettingForward;                 // +0x80
    unsigned long BettingBack;                    // +0x84
    unsigned long ReactionForward;                // +0x88
    unsigned long FarewellForward;                // +0x8c
    CDefString CameraName;                        // +0x90
    unsigned long BoxGraphicL;                    // +0x94
    unsigned long BoxGraphicC;                    // +0x98
    unsigned long BoxGraphicR;                    // +0x9c
    unsigned long ClickToContinue;                // +0xa0
    unsigned long WinPhrase;                      // +0xa4
    unsigned long LosePhrase;                     // +0xa8
    unsigned long DrawPhrase;                     // +0xac
    unsigned long NewGame;                        // +0xb0
    unsigned long BestScore;                      // +0xb4
    unsigned long CurrentScore;                   // +0xb8
    unsigned long RequiredScore;                  // +0xbc
    unsigned long AdditionalInfo;                 // +0xc0
    unsigned long BlackjackBusted;                // +0xc4
    unsigned long BlackjackSplit;                 // +0xc8
    unsigned long BlackjackDouble;                // +0xcc
    unsigned long BlackjackHit;                   // +0xd0
    unsigned long BlackjackStand;                 // +0xd4
    unsigned long BlackjackDealerTakesCard;       // +0xd8
    unsigned long BlackjackSplitGUI;              // +0xdc
    unsigned long BlackjackDoubleGUI;             // +0xe0
    unsigned long BlackjackHitGUI;                // +0xe4
    unsigned long BlackjackStandGUI;              // +0xe8
    unsigned long Bet;                            // +0xec
    unsigned long PlayersMoney;                   // +0xf0
    unsigned long TotalWinnings;                  // +0xf4
    unsigned long Continue;                       // +0xf8
    unsigned long Quit;                           // +0xfc
    unsigned long Yes;                            // +0x100
    unsigned long No;                             // +0x104
    unsigned long PrizeGiven;                     // +0x108
    unsigned long MoneyBagGraphic;                // +0x10c
    long MinBet;                                  // +0x110
    long MaxBet;                                  // +0x114
    long BetIncrement;                            // +0x118
    CDefString ScoreFont;                         // +0x11c
    CDefString TargetFont;                        // +0x120
    CDefString StatsFont;                         // +0x124
    float ScoreX;                                 // +0x128
    float ScoreY;                                 // +0x12c
    float TargetX;                                // +0x130
    float TargetY;                                // +0x134
    float BestX;                                  // +0x138
    float BestY;                                  // +0x13c
    float AdditionalX;                            // +0x140
    float AdditionalY;                            // +0x144
    float BetX;                                   // +0x148
    float BetY;                                   // +0x14c
    float MoneyX;                                 // +0x150
    float MoneyY;                                 // +0x154
    float WinningsX;                              // +0x158
    float WinningsY;                              // +0x15c
    float MainBetX;                               // +0x160
    float MainBetY;                               // +0x164
    float MainMoneyX;                             // +0x168
    float MainMoneyY;                             // +0x16c
    bool BestScoreHigh;                           // +0x170
    unsigned char _pad171[3];                     // +0x171
    vector<CPrizeScoreDef> PrizeScores;             // +0x174
    long Prize;                                   // +0x180
    long PrizeRenown;                             // +0x184
    float MainGameScoreBoxX;                      // +0x188
    float MainGameScoreBoxY;                      // +0x18c
    float MainGameScoreBoxWidthXbox;              // +0x190
    float MainGameScoreBoxWidthPC;                // +0x194
    float MainGameScoreBoxHeight;                 // +0x198
    bool DisplayErrata;                           // +0x19c
    unsigned char _pad19d[3];                     // +0x19d
    float PointerPhaseSpeed;                      // +0x1a0
};

void CTavernGameDef::Transfer(CPersistContext& context)
{
    context.Transfer(Banter);
    context.Transfer(Greeting);
    context.Transfer(OptionsInitial);
    context.Transfer(OptionsSubsequent);
    context.Transfer(Instructions);
    context.Transfer(InstructionsPC);
    context.Transfer(Betting);
    context.Transfer(Play);
    context.Transfer(ReactionWin);
    context.Transfer(ReactionLose);
    context.Transfer(ReactionDraw);
    context.Transfer(ReactionWinNewBestScore);
    context.Transfer(FarewellInitial);
    context.Transfer(FarewellSubsequent);
    context.Transfer(WinRoundPhrase);
    context.Transfer(OutOfTimePhrase);
    context.Transfer(NoMoney);
    context.Transfer(GreetingForward);
    context.Transfer(OptionsForward);
    context.Transfer(OptionsBack);
    context.Transfer(OptionsAlternative);
    context.Transfer(InstructionsBack);
    context.Transfer(BettingForward);
    context.Transfer(BettingBack);
    context.Transfer(ReactionForward);
    context.Transfer(FarewellForward);
    context.Transfer(CameraName);
    context.Transfer(BoxGraphicL);
    context.Transfer(BoxGraphicC);
    context.Transfer(BoxGraphicR);
    context.Transfer(ClickToContinue);
    context.Transfer(WinPhrase);
    context.Transfer(LosePhrase);
    context.Transfer(DrawPhrase);
    context.Transfer(NewGame);
    context.Transfer(BestScore);
    context.Transfer(CurrentScore);
    context.Transfer(RequiredScore);
    context.Transfer(AdditionalInfo);
    context.Transfer(BlackjackBusted);
    context.Transfer(BlackjackSplit);
    context.Transfer(BlackjackDouble);
    context.Transfer(BlackjackHit);
    context.Transfer(BlackjackStand);
    context.Transfer(BlackjackDealerTakesCard);
    context.Transfer(BlackjackSplitGUI);
    context.Transfer(BlackjackDoubleGUI);
    context.Transfer(BlackjackHitGUI);
    context.Transfer(BlackjackStandGUI);
    context.Transfer(Bet);
    context.Transfer(PlayersMoney);
    context.Transfer(TotalWinnings);
    context.Transfer(Continue);
    context.Transfer(Quit);
    context.Transfer(Yes);
    context.Transfer(No);
    context.Transfer(PrizeGiven);
    context.Transfer(MoneyBagGraphic);
    context.Transfer(MinBet);
    context.Transfer(MaxBet);
    context.Transfer(BetIncrement);
    context.Transfer(ScoreFont);
    context.Transfer(TargetFont);
    context.Transfer(StatsFont);
    context.Transfer(ScoreX);
    context.Transfer(ScoreY);
    context.Transfer(TargetX);
    context.Transfer(TargetY);
    context.Transfer(BestX);
    context.Transfer(BestY);
    context.Transfer(AdditionalX);
    context.Transfer(AdditionalY);
    context.Transfer(BetX);
    context.Transfer(BetY);
    context.Transfer(MoneyX);
    context.Transfer(MoneyY);
    context.Transfer(WinningsX);
    context.Transfer(WinningsY);
    context.Transfer(MainBetX);
    context.Transfer(MainBetY);
    context.Transfer(MainMoneyX);
    context.Transfer(MainMoneyY);
    context.Transfer(BestScoreHigh);
    context.TransferVectorOfSubComponents(PrizeScores);
    context.Transfer(Prize);
    context.Transfer(PrizeRenown);
    context.Transfer(MainGameScoreBoxX);
    context.Transfer(MainGameScoreBoxY);
    context.Transfer(MainGameScoreBoxWidthXbox);
    context.Transfer(MainGameScoreBoxWidthPC);
    context.Transfer(MainGameScoreBoxHeight);
    context.Transfer(DisplayErrata);
    context.Transfer(PointerPhaseSpeed);
}