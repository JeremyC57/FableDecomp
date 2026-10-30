// NScript::CQ_WaspBossScript::GuildmasterHelp() -- the "GuildmasterHelp" spawned thread of the wasp boss quest (QST_072): wait for the intro cutscene, look up the queen hornet, keep running while she lives, and have the guildmaster call out a hint at each of her boss phases 3, 4 and 6 and when her health reaches the low-health threshold
// Leaf: ?GuildmasterHelp@CQ_WaspBossScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E12B30, 993 bytes (`ret`, __fastcall).
// Identity: Ego_r.pdb / FableWin.pdb ?GuildmasterHelp@CQ_WaspBossScript@NScript@@QAEXXZ; the landed DoMission 0x00E12580
//   spawns it as CSpawnedFunc<CQ_WaspBossScript>("GuildmasterHelp", 0, this, &GuildmasterHelp) (stores 0x00E12B30 at +0x34,
//   string 0x012E05B4 "GuildmasterHelp"); the hints are TEXT_QST_072_GUILDMASTER_GUIDANCE_10..50 (0x012E0698..0x012E05F8).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptThing copy ctor 0x004ABE90 /
//   ~CScriptThing 0x004AA840, CScriptBase::SetPredicate 0x00CB7920, CScriptBase::IsActiveThreadTerminating 0x00CB7940;
//   Game (CGameScriptInterfaceBase) slots 7 NewScriptFrame, 72 GetThingWithScriptName, 264 GetHealth,
//   274 HeroReceiveMessageFromGuildMaster, 376 Pause, 522 EntityGetBossPhase; NScript::CIsThingAlive vtable 0x012C32F0.
// Data: pGlobalScriptDef 0x0143E90C; +0xE34 = Ego_r CScriptDef::WB_LowHealth (Ego_r +0xEE4; the WB_ block sits 0xB0
//   lower in retail: DoMission's +0xE3C/+0xE40 def strings are Ego_r WB_CreaturesSpawned +0xEEC / WB_WaspBossName +0xEF0).
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, CutsceneFinished +0x4C (Ego_r +0x60).
// Shape: the low-health test reads pGlobalScriptDef into edi BEFORE the GetHealth call: a named `def` local re-read at the
//   end of the loop body (the FisticuffsChallenge 0x00E86E60 form); reading the def inside the compare spills the fild.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:        // 12 bytes, vtable 0x01238C8C
    void*           PImp_Data;                          // +0x04 CCountedPointer<CScriptThing>
    CCPPointerInfo* PImp_Info;                          // +0x08
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    virtual ~CScriptThing();                            // 0x004AA840
};
namespace NScript {
class CPredicateBase { public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() {}
};
class CIsThingAlive : public CPredicateBase { public:  // vtable 0x012C32F0
    CScriptThing Thing;
    __forceinline CIsThingAlive(const CScriptThing& t) : Thing(t) {}
    virtual bool operator()() const;
    virtual CPredicateBase* Clone() const;
    ~CIsThingAlive() {}
};
}
// CGameScriptInterfaceBase is the retail abstract script interface from the shared headers (FABLE_RETAIL_NSCRIPT
//   opt-in: CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x0126009C, 917 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterfaceBase.h"
class CScriptDef { public:
    unsigned char _pad[0xe34];
    long WB_LowHealth;                                  // +0xe34
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_WaspBossScript : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    void*                     PMasterData;              // +0x44
    bool MissionSucceeded;                              // +0x48
    bool MissionFailed;                                 // +0x49
    bool DronesCreated;                                 // +0x4a
    bool QueenHornetAttacks;                            // +0x4b
    bool CutsceneFinished;                              // +0x4c
    void GuildmasterHelp();
};
}

void NScript::CQ_WaspBossScript::GuildmasterHelp()
{
    while (!CutsceneFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing queen = Game->GetThingWithScriptName("QueenHornet");
    SetPredicate(CIsThingAlive(queen));
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_10", "", true, true);
    while (Game->EntityGetBossPhase(queen) != 3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_20", "", true, true);
    while (Game->EntityGetBossPhase(queen) != 4) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_30", "", true, true);
    while (Game->EntityGetBossPhase(queen) != 6) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_40", "", true, true);
    CScriptDef* def = pGlobalScriptDef;
    while (Game->GetHealth(queen) != def->WB_LowHealth) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        def = pGlobalScriptDef;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->Pause(1.0f);
    Game->HeroReceiveMessageFromGuildMaster("TEXT_QST_072_GUILDMASTER_GUIDANCE_50", "", true, true);
}