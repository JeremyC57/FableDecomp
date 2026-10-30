// NScript::CQ_AmbushTradersScript::EntityCountDecrease(CCharString defName) -- one of the Ambush Traders' enemies died: a trader (CREATURE_TRADER_01) lowers TraderAliveCount, any Bowerstone guard def (plain, red, black, crossbow) lowers GuardAliveCount; every kill raises the master data's AmbushTradersKillCount
// Leaf: ?EntityCountDecrease@CQ_AmbushTradersScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F088E0, 325 bytes, thiscall, `ret 4` (the by-value CCharString is destroyed here).
// Identity (proven): the row's only caller is the landed NScript::CQ_AmbushTradersScript::WatchForEnemyDeath 0x00F08590 (site
//   0x00F0884B, the def name copied into the argument slot), which already binds it as
//   ?EntityCountDecrease@CQ_AmbushTradersScript@NScript@@QAEXVCCharString@@@Z; that is the only CQ_AmbushTradersScript member
//   taking a CCharString by value in Ego_r.pdb (0x00F83C70), and FableWin.pdb places it right after WatchForEnemyDeath
//   (0x0362FB70 -> 0x0362FE20), the retail order. Catalog label CScriptBase_AmbushTradersFilter is invented.
// Callees: CBasicString<char>::Compare 0x00411570 (inline CCharString::operator==), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940, ~CCharString 0x0099EAE0.
// Literals: "CREATURE_TRADER_01" 0x012E15A4, "CREATURE_BS_GUARD" 0x012D2548, "CREATURE_BS_GUARD_RED" 0x012DE8DC,
//   "CREATURE_BS_GUARD_BLACK" 0x012DE8C4, "CREATURE_BS_GUARD_CROSSBOW" 0x012D22D4, "" 0x0122D70E.
// Layout (retail = Ego_r.pdb - 0x14): MasterScriptData +0x44, TraderAliveCount +0x6c (Ego_r 0x80), GuardAliveCount +0x70
//   (Ego_r 0x84); CQ_SunnyvaleMasterData::AmbushTradersKillCount +0xcc (no shift).
// Shape: the inline CCharString::operator== of the landed CQ_TraderConflictGoodScript::UpdateLiveEnemies 0x00DFC320 (strcmp
//   intrinsic on the null arm, Compare otherwise); VC7.1 threads the null test once through all five compares (two chains)
//   only when it can see Compare's body (below): with Compare declared only, every compare reloads and re-tests the
//   argument's PStringData (355v325).
#include <string.h>
template <class T> class CBasicString { public:        // 13 bytes, pack(1)
    T*            PData;                                // +0x00
    unsigned long StringLength;                         // +0x04
    unsigned long DataLength : 31;                      // +0x08
    bool          UseFastExtend : 1;                    // +0x0c
    __forceinline bool operator==(const T* text) const { return Compare(PData, text) ? false : true; }
protected:
    static long __fastcall Compare(const T* a, const T* b);   // 0x00411570 for <char>
};
#pragma pack(push, 1)
class CCharStringData { public:                         // 17 bytes
    CBasicString<char> Data;                            // +0x00
    long               NoRefs;                          // +0x0d
};
#pragma pack(pop)
// The Compare body (landed 0x00411570) compiled in this TU under auto_inline(off), as the landed
// CWorldMap::IsSectionToHaveNavigationMap 0x004FDCB0 does: VC7.1 then sees that the comparison writes no memory, keeps
// PStringData / PData in registers across the five calls and threads the null test once (two compare chains).
#pragma auto_inline(off)
template <> long __fastcall CBasicString<char>::Compare(const char* a, const char* b)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}
#pragma auto_inline(on)
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return PStringData->Data == text;
    }
};
class CGameScriptInterfaceBase;
class CPersistContext;
// CScriptBase / CLevelScriptBase<T>: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40; CLevelScriptBase<T> Game +0x40, MasterScriptData +0x44).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CLevelScriptBase.h"

namespace NScript {
class CQ_SunnyvaleMasterData {                          // Ego_r layout (no shift up to here)
public:
    unsigned char _before[0xcc];
    long AmbushTradersKillCount;                        // +0xcc
};
class CQ_AmbushTradersScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool QuestFailed;                                   // +0x48
    bool QuestFinished;                                 // +0x49
    bool QuestSucceeded;                                // +0x4a
    bool QuestOver;                                     // +0x4b
    bool RegionInvalid;                                 // +0x4c
    bool BanditIntroDone;                               // +0x4d
    bool TraderEscaped;                                 // +0x4e
    bool NoEnemiesLeft;                                 // +0x4f
    bool PhaseComplete;                                 // +0x50
    bool ConvoyTrigger2;                                // +0x51
    bool AllBanditsDead;                                // +0x52
    bool AllTradersDead;                                // +0x53
    bool AllGuardsDead;                                 // +0x54
    bool ContactDead;                                   // +0x55
    bool ContactAttackedByHero;                         // +0x56
    bool HeroDead;                                      // +0x57
    bool SpyDead;                                       // +0x58
    bool SentryAttackedByHero;                          // +0x59
    bool SpySeenBySentry;                               // +0x5a
    bool SpyFollowing;                                  // +0x5b
    bool SpyFollowingHero;                              // +0x5c
    bool SpyHeroProximity;                              // +0x5d
    bool SpyWanderReset;                                // +0x5e
    bool SpyContactMade;                                // +0x5f
    bool SpyEscaped;                                    // +0x60
    bool SpyExited;                                     // +0x61
    bool SpyReachedNoEntryMarker;                       // +0x62
    bool AnyTraderEscaped;                              // +0x63
    bool RegionClear;                                   // +0x64
    bool QuestSuccessSpeechComplete;                    // +0x65
    bool ActivatedPhase3;                               // +0x66
    bool BanditContactIntroComplete;                    // +0x67
    bool BanditContactKilledBeforeQuestEnd;             // +0x68
    bool EnableSpeechSpy;                               // +0x69
    bool EnableSpeechBandit;                            // +0x6a
    long TraderAliveCount;                              // +0x6c
    long GuardAliveCount;                               // +0x70
    void EntityCountDecrease(CCharString defName);
};

void CQ_AmbushTradersScript::EntityCountDecrease(CCharString defName)
{
    if (defName == "CREATURE_TRADER_01") {
        if (IsActiveThreadTerminating())
            return;
        --TraderAliveCount;
    } else if (defName == "CREATURE_BS_GUARD" || defName == "CREATURE_BS_GUARD_RED" || defName == "CREATURE_BS_GUARD_BLACK"
               || defName == "CREATURE_BS_GUARD_CROSSBOW") {
        if (IsActiveThreadTerminating())
            return;
        --GuardAliveCount;
    }
    ++MasterScriptData->AmbushTradersKillCount;
}
}   // namespace NScript