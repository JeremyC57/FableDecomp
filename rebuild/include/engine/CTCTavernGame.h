#ifndef FABLE_ENGINE_CTCTAVERNGAME_H
#define FABLE_ENGINE_CTCTAVERNGAME_H
#ifdef FABLE_RETAIL_CTCTAVERNGAME
// Retail opt-in (2026-11-03, row 0x005EB9E0 CTCTavernGame::FrameUpdate): the polymorphic retail class. RTTI .?AVCTCTavernGame@@ :
// CTCBase (ghidra_out/rtti_bases.tsv). Vtable 0x012560BC, 51 slots: CTCBase's 28 with slot 0 ??_G 0x005EDA30, 1 OnSerialise
// 0x005EA8E0, 6 OnInitialActivate 0x005EA650, 8 HasFrameUpdate 0x005EDA10, 9 FrameUpdate 0x005EB9E0, 11
// DrawPreparePrimitivesForRendering 0x005ECA50, 21 GetInterfaceType 0x005EDA00 replaced (22 GetClassTypeName stays _purecall), then
// the class's own 23: 28 ProcessInput 0x005EC270, 29 EnterGame 0x005EC0F0, 30 LeaveGame 0x005EC650, 31 NewGame 0x005E9A70, 32
// NewRound 0x005E9A90, 33 EndRound 0x005E9AA0, 34 QuitMidGame 0x005EDA20, 35 Greeting 0x005E9AB0, 36 Options 0x005E9AC0, 37
// Instructions 0x005E9AD0, 38 Betting 0x005E9AE0, 39 ReactionAndPayout 0x005E9AF0, 40 UpdateValues 0x005EA010, 41 Farewell 0x005E9B00,
// 42 GetWinPhrase 0x005EA100, 43..45 _purecall (GetBestScore / GetCurrentScore / GetRequiredScore: the CTCTavernGameShoveHaPenny
// vtable 0x0126F2F4 fills them with its PDB overrides 0x007CF5B0 / 0x007CF690 / 0x007CF740), 46 GetAdditionalInfo 0x005E9BC0, 47
// GetBet 0x005EA1A0, 48 GetPlayersMoney 0x005EA250, 49 GetTotalWinnings 0x005EA310, 50 DrawBetAndMoney 0x005EA400. The empty bodies
// 35..39 / 41 sit in Ego_r.pdb definition order (NewGame, NewRound, EndRound, Greeting, Options, Instructions, Betting,
// ReactionAndPayout, Farewell) at retail 0x005E9A70..0x005E9B00. Signatures and access: Ego_r.pdb / FableWin.pdb (PlaySoundW is the
// PDB spelling: <windows.h> renamed the source's PlaySound). Members: the default branch's names and retail offsets, naturally
// aligned behind the CTCBase subobject (sizeof 0xF0); PGameMaster is the packed CIntelligentPointer<CThingAICreature> (+0xD0).
// ETavernGameStates: the enumerator names are in neither PDB name list; the values the rows use are written as numbers.
// GetDef is inline (Ego_r.pdb has an out-of-line copy, retail none: FrameUpdate reads ParentThing->PBaseDef in place and calls
// GetSubDef<CTavernGameDef> 0x005ED4A0). Needs the retail CTCBase / CThing / def / pointer models; defines the macros it needs.
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#ifndef FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#endif
#ifndef FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING
#endif
#include <stddef.h>
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCountedPointer.h"
class CEnginePrimitivePrepareInfo;
#ifdef FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
class C3DVector;                                                         // PositionMesh parameter types
class CRightHandedSet;
#endif
class CFontBank;
class CInputEvent;
class CInputProcessTavernGame;
class CPersistContext;
class CProcessedInput;
class CTCHeroStats;
class CTavernGameDef;
class CThingAICreature;
class CThingObject;
struct _func___cdecl_void;

class CTCTavernGame : public CTCBase {
public:
    enum ETavernGameStates { ETGS_NONE=0,ETGS_GREETING=1,ETGS_OPTIONS=2,ETGS_INSTRUCTIONS=3,ETGS_BETTING=4,ETGS_GAME=5,ETGS_END_ROUND=6,ETGS_NEW_ROUND=7,ETGS_REACTION_AND_PAYOUT=8,ETGS_UPDATE_VALUES=9,ETGS_FAREWELL=10,ETGS_QUIT_MID_GAME=11,ETGS_GIVE_PRIZE=12,ETGS_WAIT_FOR_CLICK=13,ETGS_WAIT_FOR_TIME=14,ETGS_POSITION_COIN=15,ETGS_AIM_COIN=16,ETGS_WATCH_COINS=17,ETGS_DEAL_CARDS=18,ETGS_DEAL_DEALERS_CARDS=19,ETGS_CARDS_MOVING_OUT=20,ETGS_CARDS_FINISHED_MOVING=21,ETGS_CARDS_TURNING_UP=22,ETGS_CARDS_MAIN_GAME=23,ETGS_TURN_CARDS_BACK=24,ETGS_CARDS_TURNING_BACK=25,ETGS_CARDS_MOVING_BACK=26,ETGS_REPLACE_CARDS_IN_PACK=27,ETGS_BLACKJACK_SPLIT=28,ETGS_BLACKJACK_PRE_DOUBLE=29,ETGS_BLACKJACK_PRE_HIT=30,ETGS_BLACKJACK_PRE_STAND=31,ETGS_BLACKJACK_DOUBLE=32,ETGS_BLACKJACK_HIT=33,ETGS_BLACKJACK_STAND=34,ETGS_BLACKJACK_DEALER=35,ETGS_BLACKJACK_DEALER_TAKES_CARD=36,ETGS_BLACKJACK_NEW_CARD_DEALT=37,ETGS_BLACKJACK_TURN_PLAYERS_CARDS=38,ETGS_ADDITION_SETUP=39,ETGS_ADDITION_ADD=40,ETGS_ADDITION_WATCH=41,ETGS_ADDITION_SELECT=42,ETGS_ADDITION_RESPONSE=43,ETGS_WAIT_FOR_OK=44,ETGS_WAIT_FOR_YES_NO=45,ETGS_WAIT_FOR_YES_NO_ALT=46,ETGS_LEAVE=47 };                                          // W4ETavernGameStates@CTCTavernGame@@ (enumerator names unproven)
    CTCTavernGame(CThing&);                                              // ??0CTCTavernGame@@QAE@AAVCThing@@@Z 0x005EC8A0
    virtual ~CTCTavernGame();                                            // slot 0  ??1 0x005EDA50 (??_G 0x005EDA30)
    virtual void OnSerialise(CPersistContext&, bool);                    //  1 0x005EA8E0
    virtual void OnInitialActivate();                                    //  6 0x005EA650
    virtual bool HasFrameUpdate() const;                                 //  8 0x005EDA10
    virtual void FrameUpdate();                                          //  9 0x005EB9E0
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);   // 11 0x005ECA50
    virtual ETCInterfaceType GetInterfaceType() const;                   // 21 0x005EDA00
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // 28 0x005EC270
    virtual void EnterGame();                                            // 29 0x005EC0F0
    virtual void LeaveGame();                                            // 30 0x005EC650
    virtual void NewGame();                                              // 31 0x005E9A70
    virtual void NewRound();                                             // 32 0x005E9A90
    virtual void EndRound();                                             // 33 0x005E9AA0
    virtual void QuitMidGame();                                          // 34 0x005EDA20
    virtual void Greeting();                                             // 35 0x005E9AB0
    virtual void Options();                                              // 36 0x005E9AC0
    virtual void Instructions();                                         // 37 0x005E9AD0
    virtual void Betting();                                              // 38 0x005E9AE0
    virtual void ReactionAndPayout();                                    // 39 0x005E9AF0
    virtual bool UpdateValues();                                         // 40 0x005EA010
    virtual void Farewell();                                             // 41 0x005E9B00
    virtual CWideString GetWinPhrase();                                  // 42 0x005EA100
    virtual CWideString GetBestScore() = 0;                              // 43 _purecall
    virtual CWideString GetCurrentScore() = 0;                           // 44 _purecall
    virtual CWideString GetRequiredScore() = 0;                          // 45 _purecall
    virtual CWideString GetAdditionalInfo();                             // 46 0x005E9BC0
    virtual CWideString GetBet();                                        // 47 0x005EA1A0
    virtual CWideString GetPlayersMoney();                               // 48 0x005EA250
    virtual CWideString GetTotalWinnings();                              // 49 0x005EA310
    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float);   // 50 0x005EA400
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_TAVERN_GAME; }   // ?StatGetInterfaceType@CTCTavernGame@@SI?AW4ETCInterfaceType@@XZ
    void SetState(ETavernGameStates);                                    // 0x005E9B20 ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z
    void SetStateAfterTime(ETavernGameStates, float);                    // 0x005E9B70
    bool IsCurrentOrUpcomingState(ETavernGameStates);                    // 0x005E9BF0
    bool IsBetting();                                                    // 0x005E9C30
    CWideString GetText(unsigned long);                                  // 0x005E9E70 ?GetText@CTCTavernGame@@QAE?AVCWideString@@K@Z
    CWideString GetTextWithNarrator(unsigned long);                      // 0x005EB3A0 ?GetTextWithNarrator@CTCTavernGame@@QAE?AVCWideString@@K@Z
    void GetHeroStats(CTCHeroStats**);                                   // 0x005EB4B0
    bool PlayerHasBeenHit();                                             // 0x005EB510 ?PlayerHasBeenHit@CTCTavernGame@@QAE_NXZ
    void GivePrize();                                                    // 0x005EB590
    void ChangeBet(bool);                                                // 0x005EB810
    CThingObject* CreateGamePiece(long);                                 // 0x005EA9B0
    long GetBanter() const;                                              // 0x005EA3C0
#ifdef FABLE_RETAIL_CTCTAVERNGAME_POSITIONMESH
    // Opt-in (w22 agent1016, row 0x005EABC0): Ego_r.pdb / FableWin.pdb
    // ?PositionMesh@CTCTavernGame@@SIXPAVCThing@@ABVC3DVector@@ABVCRightHandedSet@@1MM@Z (public static __fastcall). Declaration only.
    static void __fastcall PositionMesh(CThing* thing, const C3DVector& position, const CRightHandedSet& rhSet, const C3DVector& offset,
                                        float forwardAngle, float upAngle);   // 0x005EABC0
#endif
#ifdef FABLE_RETAIL_CTCTAVERNGAME_CAMERAPOINT
    // Nested opt-in (chunk 768, row 0x005EBFA0): Ego_r.pdb / FableWin.pdb ?CameraUseCameraPoint@CTCTavernGame@@QAEXABVCCharString@@@Z
    // (public; the landed CTCTavernGameShoveHaPenny::EnterGame 0x007CF530 calls it). Declaration only.
    void CameraUseCameraPoint(const CCharString& name);                  // 0x005EBFA0
#endif
#ifdef FABLE_RETAIL_CTCTAVERNGAME_REGISTERGAMEMASTER
    // Nested opt-in (agentFAM12, row 0x0094D430): Ego_r.pdb / FableWin.pdb ?RegisterGameMaster@CTCTavernGame@@QAEXPAVCThingAICreature@@@Z
    // (public; PGameMaster = CIntelligentPointer<CThingAICreature>(master) at +0xD0; the landed caller 0x0094D020 binds it). The row
    // defines it.
    void RegisterGameMaster(CThingAICreature* master);                   // 0x0094D430
#endif
#ifdef FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
    // Per-TU opt-in (chunk 765, row 0x005EA000): retail does keep an out-of-line GetDef at 0x005EA000 (`mov ecx,[ecx+4] ; mov ecx,[ecx+70h] ;
    // jmp GetSubDef<CTavernGameDef> 0x005ED4A0`), called 55 times from the CTCTavernGameShoveHaPenny / CoinGolf TUs; FrameUpdate expands
    // it in place because it shares GetDef's TU. The defining TU declares it out of line; the inline body below stays the default.
    void GetDef(CDefPointer<const CTavernGameDef>* def);                 // 0x005EA000
#else
    void GetDef(CDefPointer<const CTavernGameDef>* def) { ParentThing->PeekBaseDef().GetSubDef(def); }   // ?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z (inline)
#endif
protected:
    void SetStateAfterInstructions(unsigned long, CWideString, ETavernGameStates);            // 0x005EB180
    void SetStateAfterOK(CWideString, CWideString, ETavernGameStates);                        // 0x005EB080
    void SetStateAfterYesNo(CWideString, CWideString, ETavernGameStates, CWideString, ETavernGameStates);   // 0x005EAF30
    void SetStateAfterYesNoAlt(CWideString, CWideString, ETavernGameStates, CWideString, ETavernGameStates,
                               CWideString, ETavernGameStates);                               // 0x005EADC0
    unsigned long PlaySoundW(const CCharString&);                        // 0x005EA950 ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z
public:
    unsigned char       UIComponents[0xc];                   // +0x0c vector<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>
    double              LastRenderTime;                      // +0x18
    bool                IsBeingPlayed;                       // +0x20
    bool                GameWon;                             // +0x21
    long                NumRounds;                           // +0x24
    bool                GameFinished;                        // +0x28
    bool                GameInSession;                       // +0x29
    float               Score;                               // +0x2c
    float               Target;                              // +0x30
    long                BaseBet;                             // +0x34
    long                LastBet;                             // +0x38
    long                TotalWinnings;                       // +0x3c
    long                CurrentMoney;                        // +0x40
    unsigned char       Handles[0xc];                        // +0x44 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       BoxHandles[0xc];                     // +0x50 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       BetHandles[0xc];                     // +0x5c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       MoneyHandles[0xc];                   // +0x68 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       WinningsHandles[0xc];                // +0x74 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    CCountedPointer<CFontBank> PScoreFont;                   // +0x80
    CCountedPointer<CFontBank> PTargetFont;                  // +0x88
    CCountedPointer<CFontBank> PStatsFont;                   // +0x90
    float               TargetTime;                          // +0x98
    long                TargetRound;                         // +0x9c
    bool                NewBestScore;                        // +0xa0
    bool                Draw;                                // +0xa1
    bool                GameStarted;                         // +0xa2
    long                NumConsecutiveRoundsWon;             // +0xa4
    ETavernGameStates   YesState;                            // +0xa8
    ETavernGameStates   NoState;                             // +0xac
    ETavernGameStates   AltState;                            // +0xb0
    _func___cdecl_void* YesFunc;                             // +0xb4
    _func___cdecl_void* NoFunc;                              // +0xb8
    _func___cdecl_void* AltFunc;                             // +0xbc
    ETavernGameStates   TavernGameState;                     // +0xc0
    ETavernGameStates   PreviousTavernGameState;             // +0xc4
    ETavernGameStates   NextTavernGameState;                 // +0xc8
    float               StateTimer;                          // +0xcc
    CIntelligentPointer<CThingAICreature> PGameMaster;       // +0xd0
    bool                GamePlayed;                          // +0xd8
    long                FrameBetWasLastChanged;              // +0xdc
    long                FrameBetChangeStarted;               // +0xe0
    long                Mult;                                // +0xe4
    bool                BetIncreasing;                       // +0xe8
    bool                HasPrizeBeenGiven;                   // +0xe9
};
FABLE_STATIC_ASSERT(sizeof(CTCTavernGame) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, UIComponents) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, LastRenderTime) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, IsBeingPlayed) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NumRounds) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Score) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, CurrentMoney) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PScoreFont) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NewBestScore) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, YesState) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, TavernGameState) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, StateTimer) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PGameMaster) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GamePlayed) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, HasPrizeBeenGiven) == 0xe9);
#else // flattened layout (the landed default)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CTCTavernGame = 0x108.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CTCTavernGame = 0xf0.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class CCPPointerInfo;
class CFontBank;
struct _func___cdecl_void;

#pragma pack(push, 1)
class CTCTavernGame {
public:
    void*               __vftable;                           // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char       _pad_0x04[0x4];                      // +0x04
    unsigned char       _pad_0x08[0x4];                      // +0x08
    unsigned char       UIComponents[0xc];                   // +0x0c vector<NUISystem::CComponent_*,std::allocator<NUISystem::CComponent_*>_>
    double              LastRenderTime;                      // +0x18
    bool                IsBeingPlayed;                       // +0x20
    bool                GameWon;                             // +0x21
    unsigned char       _pad_0x22[0x2];                      // +0x22
    long                NumRounds;                           // +0x24
    bool                GameFinished;                        // +0x28
    bool                GameInSession;                       // +0x29
    unsigned char       _pad_0x2a[0x2];                      // +0x2a
    float               Score;                               // +0x2c
    float               Target;                              // +0x30
    long                BaseBet;                             // +0x34
    long                LastBet;                             // +0x38
    long                TotalWinnings;                       // +0x3c
    long                CurrentMoney;                        // +0x40
    unsigned char       Handles[0xc];                        // +0x44 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       BoxHandles[0xc];                     // +0x50 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       BetHandles[0xc];                     // +0x5c vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       MoneyHandles[0xc];                   // +0x68 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    unsigned char       WinningsHandles[0xc];                // +0x74 vector<CEnginePrimitiveHandle,std::allocator<CEnginePrimitiveHandle>_>
    CFontBank*          PScoreFont_Data;                     // +0x80 CCountedPointer<CFontBank>::Data
    CCPPointerInfo*     PScoreFont_Info;                     // +0x84 CCountedPointer<CFontBank>::Info
    CFontBank*          PTargetFont_Data;                    // +0x88 CCountedPointer<CFontBank>::Data
    CCPPointerInfo*     PTargetFont_Info;                    // +0x8c CCountedPointer<CFontBank>::Info
    CFontBank*          PStatsFont_Data;                     // +0x90 CCountedPointer<CFontBank>::Data
    CCPPointerInfo*     PStatsFont_Info;                     // +0x94 CCountedPointer<CFontBank>::Info
    float               TargetTime;                          // +0x98
    long                TargetRound;                         // +0x9c
    bool                NewBestScore;                        // +0xa0
    bool                Draw;                                // +0xa1
    bool                GameStarted;                         // +0xa2
    unsigned char       _pad_0xa3[0x1];                      // +0xa3
    long                NumConsecutiveRoundsWon;             // +0xa4
    unsigned char       YesState[0x4];                       // +0xa8 ETavernGameStates
    unsigned char       NoState[0x4];                        // +0xac ETavernGameStates
    unsigned char       AltState[0x4];                       // +0xb0 ETavernGameStates
    _func___cdecl_void* YesFunc;                             // +0xb4
    _func___cdecl_void* NoFunc;                              // +0xb8
    _func___cdecl_void* AltFunc;                             // +0xbc
    unsigned char       TavernGameState[0x4];                // +0xc0 ETavernGameStates
    unsigned char       PreviousTavernGameState[0x4];        // +0xc4 ETavernGameStates
    unsigned char       NextTavernGameState[0x4];            // +0xc8 ETavernGameStates
    float               StateTimer;                          // +0xcc
    unsigned char       PGameMaster[0x8];                    // +0xd0 CIntelligentPointer<CThingAICreature>
    bool                GamePlayed;                          // +0xd8
    unsigned char       _pad_0xd9[0x3];                      // +0xd9
    long                FrameBetWasLastChanged;              // +0xdc
    long                FrameBetChangeStarted;               // +0xe0
    long                Mult;                                // +0xe4
    bool                BetIncreasing;                       // +0xe8
    bool                HasPrizeBeenGiven;                   // +0xe9
    unsigned char       _pad_0xea[0x6];                      // +0xea
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CTCTavernGame) == 0xf0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, UIComponents) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, LastRenderTime) == 0x18);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, IsBeingPlayed) == 0x20);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GameWon) == 0x21);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NumRounds) == 0x24);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GameFinished) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GameInSession) == 0x29);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Score) == 0x2c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Target) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, BaseBet) == 0x34);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, LastBet) == 0x38);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, TotalWinnings) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, CurrentMoney) == 0x40);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Handles) == 0x44);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, BoxHandles) == 0x50);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, BetHandles) == 0x5c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, MoneyHandles) == 0x68);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, WinningsHandles) == 0x74);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PScoreFont_Data) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PScoreFont_Info) == 0x84);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PTargetFont_Data) == 0x88);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PTargetFont_Info) == 0x8c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PStatsFont_Data) == 0x90);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PStatsFont_Info) == 0x94);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, TargetTime) == 0x98);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, TargetRound) == 0x9c);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NewBestScore) == 0xa0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Draw) == 0xa1);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GameStarted) == 0xa2);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NumConsecutiveRoundsWon) == 0xa4);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, YesState) == 0xa8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NoState) == 0xac);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, AltState) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, YesFunc) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NoFunc) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, AltFunc) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, TavernGameState) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PreviousTavernGameState) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, NextTavernGameState) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, StateTimer) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, PGameMaster) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, GamePlayed) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, FrameBetWasLastChanged) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, FrameBetChangeStarted) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, Mult) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, BetIncreasing) == 0xe8);
FABLE_STATIC_ASSERT(offsetof(CTCTavernGame, HasPrizeBeenGiven) == 0xe9);

#endif // FABLE_RETAIL_CTCTAVERNGAME
#endif // FABLE_ENGINE_CTCTAVERNGAME_H
