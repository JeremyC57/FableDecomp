// NScript::CQ_AmbushTradersScript::ProcessInitialSpyExtraction(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&) -- the spy has made contact: update the quest card, release the spy from his scripted follow, make him follow the hero with a health bar, and spawn WatchForArcherSpawnConditions
// Leaf: ProcessInitialSpyExtraction
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0F680, length 644 bytes (padded to 656), ret 8.
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// CScriptGameResourceObjectScriptedThingBase::IsScripting 0x00CD23B9 / Release 0x00CD2770 and vtable 0x0127094C slots
// 8 StopFollowingThing, 27 IsFollowingThing; RefreshSpyAllegiences 0x00F05A10; operator new 0x00BFEA1A;
// CSpawnedFuncBase(const CCharString&, unsigned long) 0x00CDD450 (CSpawnedFunc<CQ_AmbushTradersScript> vtable 0x012C6CC0,
// func 0x00F0F910 = WatchForArcherSpawnConditions; the manifest's "CQ_GuildTrainingScript::WatchForSparrowKilled" label
// is propagated); CScriptBase::AddSpawnedFunction 0x00CB7E50; CGameScriptInterface slots 7, 8, 70, 229, 235, 296, 321,
// 325, 480, 530, 655.
// Layout: Ego_r.pdb - 0x14 (SpyContactMade +0x5f, SpeechBanditCount +0x74, SpyHealthBarID +0x84).
// Shape: as the landed CV_ChapelOfEvilScript::DoMission 0x00E6A7F0 spawn (entry-zeroed guard flag for the thread name
// temporary); the CRGBColour temporary is built by a ctor that assigns R, G, B, A in that order.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CScriptThing;
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_4 = 4 };
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void StopFollowingThing(const CScriptThing& thing);   // slot 8 (+0x20) 0x007E7330
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026();
    virtual bool IsFollowingThing() const;             // slot 27 (+0x6c) 0x007E7460
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& regionName);   // 0x00CB7E50
    __forceinline void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    bool QuestFailed;                                  // +0x48
    unsigned char _pad_0x49[0x5f - 0x49];
    bool SpyContactMade;                               // +0x5f
    unsigned char _pad_0x60[0x74 - 0x60];
    long SpeechBanditCount;                            // +0x74
    long SpeechSpyCount;                               // +0x78
    long SpeechSpyProximityCount;                      // +0x7c
    long SpeechSpySentryCount;                         // +0x80
    long SpyHealthBarID;                               // +0x84
    void RefreshSpyAllegiences();                      // 0x00F05A10
    void WatchForArcherSpawnConditions();              // 0x00F0F910
    void ProcessInitialSpyExtraction(const CScriptThing& spy, CScriptGameResourceObjectScriptedThingBase& spyObject);
};

void CQ_AmbushTradersScript::ProcessInitialSpyExtraction(const CScriptThing& spy, CScriptGameResourceObjectScriptedThingBase& spyObject)
{
    SpyContactMade = true;
    SpeechBanditCount = 0;
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_AMBUSH_TRADERS_OBJECTIVE_04", "BowerstonePosh", "BowerstoneSlums");
    if (spyObject.IsScripting() && spyObject.IsFollowingThing()) {
        if (IsActiveThreadTerminating()) return;
        if (spyObject.IsScripting()) spyObject.Release();
        while (!Game->StartScriptingEntity(spy, spyObject, SCRIPT_AI_PRIORITY_4)) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return;
        }
        if (IsActiveThreadTerminating()) return;
        spyObject.StopFollowingThing(Game->GetHero());
        if (spyObject.IsScripting()) spyObject.Release();
    }
    Game->EntitySetAsAllowedToFollowHero(spy, true);
    Game->SetEntityAsRegionFollowing(Game->GetHero(), spy, true);
    Game->EntityFollowThing(spy, Game->GetHero(), 1.0f, true);
    Game->EntitySetAsDisplayingEmoteIcon(spy, true);
    RefreshSpyAllegiences();
    SpyHealthBarID = Game->AddQuestInfoBarHealth(spy, CRGBColour(0xf0, 0, 0x78, 0xff), "HUD_QUEST_ICON_TRADER", 1.0f);
    Game->DisplayQuestInfo(true);
    SpawnFunction(new CSpawnedFunc<CQ_AmbushTradersScript>("WatchForArcherSpawnConditions", 0, this, &CQ_AmbushTradersScript::WatchForArcherSpawnConditions));
}
}