#ifndef FABLE_ENGINE_CTAVERNGAMEDEF_H
#define FABLE_ENGINE_CTAVERNGAMEDEF_H
// Retail model (hand-written 2026-11-03, row 0x005EB9E0 CTCTavernGame::FrameUpdate): the tavern-game definition CTavernGameDef.
// RTTI .?AVCTavernGameDef@@ : CSubDefClassBase; vtable 0x012424BC (slot 20 GetSizeofClass 0x004E1086 `mov eax,1A4h`: sizeof 0x1A4).
// Members: Ego_r.pdb (struct_layouts_egor.tsv) in order; the only container is PrizeScores (Ego_r 16-byte Dinkumware vector,
// STLport 12 in retail), so the members up to BestScoreHigh keep their PDB offsets and the ones after it sit 4 lower (Prize +0x180 ..
// PointerPhaseSpeed +0x1A0). FrameUpdate 0x005EB9E0 reads Greeting +0x2C .. MinBet +0x110 at the PDB offsets. Methods: the Ego_r.pdb /
// FableWin.pdb member set (StatGetTypeName is inline: GetSubDef<CTavernGameDef> 0x005ED4A0 builds the literal in place).
// Needs the retail def model: FABLE_RETAIL_DEF_CLASS_BASE, and FABLE_RETAIL_CDEFSTRINGTABLE / FABLE_RETAIL_CCHARSTRING for CDefString.
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
class CPersistContext;

class CTavernGameDef : public CSubDefClassBase {
public:
    CTavernGameDef();                                                    // ??0CTavernGameDef@@QAE@XZ
    static CDefClassBase* __fastcall Alloc();                            // ?Alloc@CTavernGameDef@@SIPAVCDefClassBase@@XZ
    static CCharString __fastcall StatGetTypeName() { return CCharString("CTavernGameDef", -1); }   // ?StatGetTypeName@CTavernGameDef@@SI?AVCCharString@@XZ (inline)
    virtual void Transfer(CPersistContext&);                             // ?Transfer@CTavernGameDef@@UAEXAAVCPersistContext@@@Z
    virtual void Copy(const CDefClassBase*);                             // ?Copy@CTavernGameDef@@UAEXPBVCDefClassBase@@@Z
    virtual unsigned long GetSizeofClass();                              // ?GetSizeofClass@CTavernGameDef@@UAEKXZ (0x004E1086)
    unsigned long Banter;                           // +0x28
    unsigned long Greeting;                         // +0x2c
    unsigned long OptionsInitial;                   // +0x30
    unsigned long OptionsSubsequent;                // +0x34
    unsigned long Instructions;                     // +0x38
    unsigned long InstructionsPC;                   // +0x3c
    unsigned long Betting;                          // +0x40
    unsigned long Play;                             // +0x44
    unsigned long ReactionWin;                      // +0x48
    unsigned long ReactionLose;                     // +0x4c
    unsigned long ReactionDraw;                     // +0x50
    unsigned long ReactionWinNewBestScore;          // +0x54
    unsigned long FarewellInitial;                  // +0x58
    unsigned long FarewellSubsequent;               // +0x5c
    unsigned long WinRoundPhrase;                   // +0x60
    unsigned long OutOfTimePhrase;                  // +0x64
    unsigned long NoMoney;                          // +0x68
    unsigned long GreetingForward;                  // +0x6c
    unsigned long OptionsForward;                   // +0x70
    unsigned long OptionsBack;                      // +0x74
    unsigned long OptionsAlternative;               // +0x78
    unsigned long InstructionsBack;                 // +0x7c
    unsigned long BettingForward;                   // +0x80
    unsigned long BettingBack;                      // +0x84
    unsigned long ReactionForward;                  // +0x88
    unsigned long FarewellForward;                  // +0x8c
    CDefString    CameraName;                       // +0x90
    unsigned long BoxGraphicL;                      // +0x94
    unsigned long BoxGraphicC;                      // +0x98
    unsigned long BoxGraphicR;                      // +0x9c
    unsigned long ClickToContinue;                  // +0xa0
    unsigned long WinPhrase;                        // +0xa4
    unsigned long LosePhrase;                       // +0xa8
    unsigned long DrawPhrase;                       // +0xac
    unsigned long NewGame;                          // +0xb0
    unsigned long BestScore;                        // +0xb4
    unsigned long CurrentScore;                     // +0xb8
    unsigned long RequiredScore;                    // +0xbc
    unsigned long AdditionalInfo;                   // +0xc0
    unsigned long BlackjackBusted;                  // +0xc4
    unsigned long BlackjackSplit;                   // +0xc8
    unsigned long BlackjackDouble;                  // +0xcc
    unsigned long BlackjackHit;                     // +0xd0
    unsigned long BlackjackStand;                   // +0xd4
    unsigned long BlackjackDealerTakesCard;         // +0xd8
    unsigned long BlackjackSplitGUI;                // +0xdc
    unsigned long BlackjackDoubleGUI;               // +0xe0
    unsigned long BlackjackHitGUI;                  // +0xe4
    unsigned long BlackjackStandGUI;                // +0xe8
    unsigned long Bet;                              // +0xec
    unsigned long PlayersMoney;                     // +0xf0
    unsigned long TotalWinnings;                    // +0xf4
    unsigned long Continue;                         // +0xf8
    unsigned long Quit;                             // +0xfc
    unsigned long Yes;                              // +0x100
    unsigned long No;                               // +0x104
    unsigned long PrizeGiven;                       // +0x108
    unsigned long MoneyBagGraphic;                  // +0x10c
    long          MinBet;                           // +0x110
    long          MaxBet;                           // +0x114
    long          BetIncrement;                     // +0x118
    CDefString    ScoreFont;                        // +0x11c
    CDefString    TargetFont;                       // +0x120
    CDefString    StatsFont;                        // +0x124
    float         ScoreX;                           // +0x128
    float         ScoreY;                           // +0x12c
    float         TargetX;                          // +0x130
    float         TargetY;                          // +0x134
    float         BestX;                            // +0x138
    float         BestY;                            // +0x13c
    float         AdditionalX;                      // +0x140
    float         AdditionalY;                      // +0x144
    float         BetX;                             // +0x148
    float         BetY;                             // +0x14c
    float         MoneyX;                           // +0x150
    float         MoneyY;                           // +0x154
    float         WinningsX;                        // +0x158
    float         WinningsY;                        // +0x15c
    float         MainBetX;                         // +0x160
    float         MainBetY;                         // +0x164
    float         MainMoneyX;                       // +0x168
    float         MainMoneyY;                       // +0x16c
    bool          BestScoreHigh;                    // +0x170
    unsigned long PrizeScores[3];                   // +0x174 vector<CPrizeScoreDef> (STLport, 12 bytes; Ego_r 16)
    long          Prize;                            // +0x180
    long          PrizeRenown;                      // +0x184
    float         MainGameScoreBoxX;                // +0x188
    float         MainGameScoreBoxY;                // +0x18c
    float         MainGameScoreBoxWidthXbox;        // +0x190
    float         MainGameScoreBoxWidthPC;          // +0x194
    float         MainGameScoreBoxHeight;           // +0x198
    bool          DisplayErrata;                    // +0x19c
    float         PointerPhaseSpeed;                // +0x1a0
    static void CheckRetailLayout() {
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, Banter) == 0x28);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, Greeting) == 0x2c);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, NoMoney) == 0x68);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, ReactionForward) == 0x88);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, CameraName) == 0x90);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, ClickToContinue) == 0xa0);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, Continue) == 0xf8);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, PrizeGiven) == 0x108);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, MinBet) == 0x110);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, ScoreFont) == 0x11c);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, BestScoreHigh) == 0x170);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, PrizeScores) == 0x174);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, Prize) == 0x180);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, DisplayErrata) == 0x19c);
        FABLE_STATIC_ASSERT(offsetof(CTavernGameDef, PointerPhaseSpeed) == 0x1a0);
    }
};
FABLE_STATIC_ASSERT(sizeof(CTavernGameDef) == 0x1a4);

#endif // FABLE_ENGINE_CTAVERNGAMEDEF_H
