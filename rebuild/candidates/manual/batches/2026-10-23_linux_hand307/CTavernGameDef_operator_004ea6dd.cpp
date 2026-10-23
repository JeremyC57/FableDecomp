// CTavernGameDef::operator=(const CTavernGameDef&) -- memberwise def assignment: CDefClassBase part, the tavern-game text/graphic/score-layout fields, the prize-score vector; returns *this
// Leaf: ??4CTavernGameDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004EA6DD, length 1011 bytes.
// Callees: CDefClassBase::operator= @ 0x00431F10, vector<CPrizeScoreDef>::operator= @ 0x004EAAD0.
// Identity: reached through the jmp thunk 0x004EA6D8 in slot 19 of vtable 0x012424BC = CTavernGameDef
// (rtti-vtables.tsv); the manifest "operator=" label is propagated.
// Layout: PDB CTavernGameDef (Ego_r) puts PrizeScores at +0x174 as a 16-byte vector; the retail
// STLport vector is 12 bytes, so every later member sits 4 bytes lower (Prize at +0x180 ..
// DisplayErrata at +0x19c, PointerPhaseSpeed at +0x1a0) -- exactly the offsets the body copies.
// Shape: compiler-style memberwise assignment in declaration order; dword members (ulong, long,
// float, the 4-byte CDefString index) copy through eax, the two bools through al.
#pragma optimize("s", on)   // the def module is size-optimised

struct CDefClassBase {
    unsigned char _body[0x28];          // vptr, resource links, PDefManager, GlobalIndex, flag bits
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CDefString {                     // a def-string table index (4 bytes, trivially copied)
    long Index;
};

struct CPrizeScoreDefVector {           // retail std::vector<CPrizeScoreDef> (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CPrizeScoreDefVector& operator=(const CPrizeScoreDefVector& other);   // 0x004EAAD0
};

struct CTavernGameDef : CDefClassBase {                             // vtable 0x012424BC
    unsigned long    Banter;                              // +0x28
    unsigned long    Greeting;                            // +0x2c
    unsigned long    OptionsInitial;                      // +0x30
    unsigned long    OptionsSubsequent;                   // +0x34
    unsigned long    Instructions;                        // +0x38
    unsigned long    InstructionsPC;                      // +0x3c
    unsigned long    Betting;                             // +0x40
    unsigned long    Play;                                // +0x44
    unsigned long    ReactionWin;                         // +0x48
    unsigned long    ReactionLose;                        // +0x4c
    unsigned long    ReactionDraw;                        // +0x50
    unsigned long    ReactionWinNewBestScore;             // +0x54
    unsigned long    FarewellInitial;                     // +0x58
    unsigned long    FarewellSubsequent;                  // +0x5c
    unsigned long    WinRoundPhrase;                      // +0x60
    unsigned long    OutOfTimePhrase;                     // +0x64
    unsigned long    NoMoney;                             // +0x68
    unsigned long    GreetingForward;                     // +0x6c
    unsigned long    OptionsForward;                      // +0x70
    unsigned long    OptionsBack;                         // +0x74
    unsigned long    OptionsAlternative;                  // +0x78
    unsigned long    InstructionsBack;                    // +0x7c
    unsigned long    BettingForward;                      // +0x80
    unsigned long    BettingBack;                         // +0x84
    unsigned long    ReactionForward;                     // +0x88
    unsigned long    FarewellForward;                     // +0x8c
    CDefString       CameraName;                          // +0x90
    unsigned long    BoxGraphicL;                         // +0x94
    unsigned long    BoxGraphicC;                         // +0x98
    unsigned long    BoxGraphicR;                         // +0x9c
    unsigned long    ClickToContinue;                     // +0xa0
    unsigned long    WinPhrase;                           // +0xa4
    unsigned long    LosePhrase;                          // +0xa8
    unsigned long    DrawPhrase;                          // +0xac
    unsigned long    NewGame;                             // +0xb0
    unsigned long    BestScore;                           // +0xb4
    unsigned long    CurrentScore;                        // +0xb8
    unsigned long    RequiredScore;                       // +0xbc
    unsigned long    AdditionalInfo;                      // +0xc0
    unsigned long    BlackjackBusted;                     // +0xc4
    unsigned long    BlackjackSplit;                      // +0xc8
    unsigned long    BlackjackDouble;                     // +0xcc
    unsigned long    BlackjackHit;                        // +0xd0
    unsigned long    BlackjackStand;                      // +0xd4
    unsigned long    BlackjackDealerTakesCard;            // +0xd8
    unsigned long    BlackjackSplitGUI;                   // +0xdc
    unsigned long    BlackjackDoubleGUI;                  // +0xe0
    unsigned long    BlackjackHitGUI;                     // +0xe4
    unsigned long    BlackjackStandGUI;                   // +0xe8
    unsigned long    Bet;                                 // +0xec
    unsigned long    PlayersMoney;                        // +0xf0
    unsigned long    TotalWinnings;                       // +0xf4
    unsigned long    Continue;                            // +0xf8
    unsigned long    Quit;                                // +0xfc
    unsigned long    Yes;                                 // +0x100
    unsigned long    No;                                  // +0x104
    unsigned long    PrizeGiven;                          // +0x108
    unsigned long    MoneyBagGraphic;                     // +0x10c
    long             MinBet;                              // +0x110
    long             MaxBet;                              // +0x114
    long             BetIncrement;                        // +0x118
    CDefString       ScoreFont;                           // +0x11c
    CDefString       TargetFont;                          // +0x120
    CDefString       StatsFont;                           // +0x124
    float            ScoreX;                              // +0x128
    float            ScoreY;                              // +0x12c
    float            TargetX;                             // +0x130
    float            TargetY;                             // +0x134
    float            BestX;                               // +0x138
    float            BestY;                               // +0x13c
    float            AdditionalX;                         // +0x140
    float            AdditionalY;                         // +0x144
    float            BetX;                                // +0x148
    float            BetY;                                // +0x14c
    float            MoneyX;                              // +0x150
    float            MoneyY;                              // +0x154
    float            WinningsX;                           // +0x158
    float            WinningsY;                           // +0x15c
    float            MainBetX;                            // +0x160
    float            MainBetY;                            // +0x164
    float            MainMoneyX;                          // +0x168
    float            MainMoneyY;                          // +0x16c
    bool             BestScoreHigh;                       // +0x170
    CPrizeScoreDefVector PrizeScores;                         // +0x174
    long             Prize;                               // +0x180
    long             PrizeRenown;                         // +0x184
    float            MainGameScoreBoxX;                   // +0x188
    float            MainGameScoreBoxY;                   // +0x18c
    float            MainGameScoreBoxWidthXbox;           // +0x190
    float            MainGameScoreBoxWidthPC;             // +0x194
    float            MainGameScoreBoxHeight;              // +0x198
    bool             DisplayErrata;                       // +0x19c
    float            PointerPhaseSpeed;                   // +0x1a0
    CTavernGameDef& operator=(const CTavernGameDef& other);
};

CTavernGameDef& CTavernGameDef::operator=(const CTavernGameDef& other)
{
    CDefClassBase::operator=(other);
    Banter = other.Banter;
    Greeting = other.Greeting;
    OptionsInitial = other.OptionsInitial;
    OptionsSubsequent = other.OptionsSubsequent;
    Instructions = other.Instructions;
    InstructionsPC = other.InstructionsPC;
    Betting = other.Betting;
    Play = other.Play;
    ReactionWin = other.ReactionWin;
    ReactionLose = other.ReactionLose;
    ReactionDraw = other.ReactionDraw;
    ReactionWinNewBestScore = other.ReactionWinNewBestScore;
    FarewellInitial = other.FarewellInitial;
    FarewellSubsequent = other.FarewellSubsequent;
    WinRoundPhrase = other.WinRoundPhrase;
    OutOfTimePhrase = other.OutOfTimePhrase;
    NoMoney = other.NoMoney;
    GreetingForward = other.GreetingForward;
    OptionsForward = other.OptionsForward;
    OptionsBack = other.OptionsBack;
    OptionsAlternative = other.OptionsAlternative;
    InstructionsBack = other.InstructionsBack;
    BettingForward = other.BettingForward;
    BettingBack = other.BettingBack;
    ReactionForward = other.ReactionForward;
    FarewellForward = other.FarewellForward;
    CameraName = other.CameraName;
    BoxGraphicL = other.BoxGraphicL;
    BoxGraphicC = other.BoxGraphicC;
    BoxGraphicR = other.BoxGraphicR;
    ClickToContinue = other.ClickToContinue;
    WinPhrase = other.WinPhrase;
    LosePhrase = other.LosePhrase;
    DrawPhrase = other.DrawPhrase;
    NewGame = other.NewGame;
    BestScore = other.BestScore;
    CurrentScore = other.CurrentScore;
    RequiredScore = other.RequiredScore;
    AdditionalInfo = other.AdditionalInfo;
    BlackjackBusted = other.BlackjackBusted;
    BlackjackSplit = other.BlackjackSplit;
    BlackjackDouble = other.BlackjackDouble;
    BlackjackHit = other.BlackjackHit;
    BlackjackStand = other.BlackjackStand;
    BlackjackDealerTakesCard = other.BlackjackDealerTakesCard;
    BlackjackSplitGUI = other.BlackjackSplitGUI;
    BlackjackDoubleGUI = other.BlackjackDoubleGUI;
    BlackjackHitGUI = other.BlackjackHitGUI;
    BlackjackStandGUI = other.BlackjackStandGUI;
    Bet = other.Bet;
    PlayersMoney = other.PlayersMoney;
    TotalWinnings = other.TotalWinnings;
    Continue = other.Continue;
    Quit = other.Quit;
    Yes = other.Yes;
    No = other.No;
    PrizeGiven = other.PrizeGiven;
    MoneyBagGraphic = other.MoneyBagGraphic;
    MinBet = other.MinBet;
    MaxBet = other.MaxBet;
    BetIncrement = other.BetIncrement;
    ScoreFont = other.ScoreFont;
    TargetFont = other.TargetFont;
    StatsFont = other.StatsFont;
    ScoreX = other.ScoreX;
    ScoreY = other.ScoreY;
    TargetX = other.TargetX;
    TargetY = other.TargetY;
    BestX = other.BestX;
    BestY = other.BestY;
    AdditionalX = other.AdditionalX;
    AdditionalY = other.AdditionalY;
    BetX = other.BetX;
    BetY = other.BetY;
    MoneyX = other.MoneyX;
    MoneyY = other.MoneyY;
    WinningsX = other.WinningsX;
    WinningsY = other.WinningsY;
    MainBetX = other.MainBetX;
    MainBetY = other.MainBetY;
    MainMoneyX = other.MainMoneyX;
    MainMoneyY = other.MainMoneyY;
    BestScoreHigh = other.BestScoreHigh;
    PrizeScores = other.PrizeScores;
    Prize = other.Prize;
    PrizeRenown = other.PrizeRenown;
    MainGameScoreBoxX = other.MainGameScoreBoxX;
    MainGameScoreBoxY = other.MainGameScoreBoxY;
    MainGameScoreBoxWidthXbox = other.MainGameScoreBoxWidthXbox;
    MainGameScoreBoxWidthPC = other.MainGameScoreBoxWidthPC;
    MainGameScoreBoxHeight = other.MainGameScoreBoxHeight;
    DisplayErrata = other.DisplayErrata;
    PointerPhaseSpeed = other.PointerPhaseSpeed;
    return *this;
}
