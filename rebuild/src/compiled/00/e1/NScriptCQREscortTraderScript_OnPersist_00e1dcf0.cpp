// NScript::CQR_EscortTraderScript::OnPersist(CPersistContext& context) -- save/load the Escort Trader quest: the quest stage, the greeting / hiring / departure / intro / expression / bonus flags, the two loaded-region names, the trader hit count, initial health and health-bar id, each under its member name
// Leaf: ?OnPersist@CQR_EscortTraderScript@NScript@@UAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E1DCF0, 603 bytes, thiscall, `ret 4`.
// Identity (proven): the row is referenced once, from slot 4 (OnPersist, 0x012E12F0) of the RTTI-less level-script vtable
//   0x012E12E0 [dtor 0x00E24C60, RegisterMain 0x00E1DC30 (landed NScript::CQR_EscortTraderScript::RegisterMain), Main 0x00E1E110
//   (landed CQR_EscortTraderScript::Main), Init 0x00E247F0, OnPersist 0x00E1DCF0]; Ego_r.pdb
//   ?OnPersist@CQR_EscortTraderScript@NScript@@UAEXAAVCPersistContext@@@Z. Every key string is the Ego_r.pdb member at that
//   retail offset + 0x14 (+ 0x2c past the six STLport route vectors). Catalog label sub_00e1dcf0.
// Callees: CPersistContext::GetDefaultVal<CQR_EscortTraderScript::EscortTraderQuestStage> 0x00E1DF50 (`xor eax,eax ; ret`;
//   FableWin.pdb ??$GetDefaultVal@W4EscortTraderQuestStage@CQR_EscortTraderScript@NScript@@@CPersistContext@@KI..; catalog
//   label ReturnZero_ret0_00e1df50), Transfer<EscortTraderQuestStage> 0x00E1DF60 (Ego_r/FableWin ??$Transfer@W4Escort..),
//   GetDefaultVal<CCharString> 0x0040E1E0, Transfer<bool> 0x004045C0, Transfer<CCharString> 0x004109A0, Transfer<long>
//   0x00410BE0, Transfer<float> 0x00410620, ~CCharString 0x0099EAE0.
// Strings (paired by hand with the members): "QuestStage" 0x012E1490 (+0x11c), "GreetedHero" 0x012E1484 (+0x4d),
//   "PreviousLoadedRegion" 0x012E146C (+0x110), "CurrentLoadedRegion" 0x012E1458 (+0x114), "HiredByTrader" 0x012E1448 (+0x4a),
//   "TraderDeparted" 0x012E1438 (+0x51), "TraderIntroDone" 0x012E1428 (+0x5c), "TraderDepartedLost" 0x012E1414 (+0x4c),
//   "DoneExpressions" 0x012E1404 (+0x5d), "TraderHit" 0x012E13F8 (+0x4f), "TraderHitCount" 0x012E13E8 (+0x64),
//   "TraderCreated" 0x012E13D8 (+0x5e), "WrongTurnDetected" 0x012E13C4 (+0x5f), "TraderInitialHealth" 0x012E13B0 (+0x6c),
//   "TraderHealthBarID" 0x012E139C (+0x68), "TheBlackGateIsClosed" 0x012E1384 (+0x60), "ExpressionDetectedFollow" 0x012E1368
//   (+0x70), "ExpressionDetectedWait" 0x012E1350 (+0x71), "AddedQuestInfo" 0x012E1340 (+0x61), "GoneWrongWay" 0x012E1330 (+0x62),
//   "TotallyLost" 0x012E1324 (+0x63), "AddedBonusObject" 0x012E1310 (+0x72).
// Layout: retail NScript headers (FABLE_RETAIL_NSCRIPT, CScriptBase 0x40, CLevelScriptBase 0x48). Members at Ego_r.pdb - 0x14
//   up to the CCharString arrays; RouteDynamic[3] / RouteStatic[3] are STLport vector<CCharString> (0xC each, Ego_r 0x10), so
//   PreviousLoadedRegion / CurrentLoadedRegion / QuestStage sit at Ego_r - 0x2c (+0x110 / +0x114 / +0x11c). Members after the
//   last one transferred are omitted.
// Shape: the landed CV_BeardyBaldyScript::OnPersist 0x00E4F7F0 model; the enum and CCharString members use the two-argument
//   Transfer inline (default from the out-of-line GetDefaultVal<T>), every other default is a temporary.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                     // 0x0099EAE0
};
class CPersistContext {
protected:
    template <class T> static T __fastcall GetDefaultVal();
public:
    template <class T> void Transfer(const char* name, T& value, const T& defaultValue);
    template <class T> void Transfer(const char* name, T& value) { Transfer(name, value, GetDefaultVal<T>()); }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    void* Info;
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CTimer.h"
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQR_EscortTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum EscortTraderQuestStage { };
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool HiredByTrader;                                 // +0x4a
    bool TraderAtDestination;                           // +0x4b
    bool TraderDepartedLost;                            // +0x4c
    bool GreetedHero;                                   // +0x4d
    bool TraderKilled;                                  // +0x4e
    bool TraderHit;                                     // +0x4f
    bool TraderFollowing;                               // +0x50
    bool TraderDeparted;                                // +0x51
    long BanditSpawnCount;                              // +0x54
    long BanditKillCount;                               // +0x58
    bool TraderIntroDone;                               // +0x5c
    bool DoneExpressions;                               // +0x5d
    bool TraderCreated;                                 // +0x5e
    bool WrongTurnDetected;                             // +0x5f
    bool TheBlackGateIsClosed;                          // +0x60
    bool AddedQuestInfo;                                // +0x61
    bool GoneWrongWay;                                  // +0x62
    bool TotallyLost;                                   // +0x63
    long TraderHitCount;                                // +0x64
    long TraderHealthBarID;                             // +0x68
    float TraderInitialHealth;                          // +0x6c
    bool ExpressionDetectedFollow;                      // +0x70
    bool ExpressionDetectedWait;                        // +0x71
    bool AddedBonusObject;                              // +0x72
    CCharString BonusObject;                            // +0x74
    CTimer TraderComplainTimer;                         // +0x78
    CTimer TraderExpressionDetectTimer;                 // +0x7c
    CTimer TraderHitDetectTimer;                        // +0x80
    CTimer TraderRandomSpeechTimer;                     // +0x84
    CCharString RandomSpeechFollow[10];                 // +0x88
    CCharString RandomSpeechWait[5];                    // +0xb0
    long TradeRoute;                                    // +0xc4 ETradeRoutes
    unsigned char RouteDynamic[3][0xc];                 // +0xc8 std::vector<CCharString>[3]
    unsigned char RouteStatic[3][0xc];                  // +0xec std::vector<CCharString>[3]
    CCharString PreviousLoadedRegion;                   // +0x110
    CCharString CurrentLoadedRegion;                    // +0x114
    bool RegionChangeDetected;                          // +0x118
    EscortTraderQuestStage QuestStage;                  // +0x11c
    virtual void OnPersist(CPersistContext& context);
};

void CQR_EscortTraderScript::OnPersist(CPersistContext& context)
{
    context.Transfer("QuestStage", QuestStage);
    context.Transfer("GreetedHero", GreetedHero, false);
    context.Transfer("PreviousLoadedRegion", PreviousLoadedRegion);
    context.Transfer("CurrentLoadedRegion", CurrentLoadedRegion);
    context.Transfer("HiredByTrader", HiredByTrader, false);
    context.Transfer("TraderDeparted", TraderDeparted, false);
    context.Transfer("TraderIntroDone", TraderIntroDone, false);
    context.Transfer("TraderDepartedLost", TraderDepartedLost, false);
    context.Transfer("DoneExpressions", DoneExpressions, false);
    context.Transfer("TraderHit", TraderHit, false);
    context.Transfer("TraderHitCount", TraderHitCount, 0L);
    context.Transfer("TraderCreated", TraderCreated, false);
    context.Transfer("WrongTurnDetected", WrongTurnDetected, false);
    context.Transfer("TraderInitialHealth", TraderInitialHealth, 0.0f);
    context.Transfer("TraderHealthBarID", TraderHealthBarID, 0L);
    context.Transfer("TheBlackGateIsClosed", TheBlackGateIsClosed, false);
    context.Transfer("ExpressionDetectedFollow", ExpressionDetectedFollow, false);
    context.Transfer("ExpressionDetectedWait", ExpressionDetectedWait, false);
    context.Transfer("AddedQuestInfo", AddedQuestInfo, false);
    context.Transfer("GoneWrongWay", GoneWrongWay, false);
    context.Transfer("TotallyLost", TotallyLost, false);
    context.Transfer("AddedBonusObject", AddedBonusObject, false);
}
}   // namespace NScript