// NScript::CQ_BanditCampBossBattleScript::WatchForBanditCampGates() -- until the thread ends: whenever BanditCampCentre is loaded, open
// Gate2Inner and wait until the region unloads
// Leaf: ?WatchForBanditCampGates@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D04770.
// Identity: Ego_r.pdb + FableWin.pdb ?WatchForBanditCampGates@CQ_BanditCampBossBattleScript@NScript@@QAEXXZ; its only reference is the
//   CSpawnedFunc spawn in CQ_BanditCampBossBattleScript's 0x00D038E0 with the literal "WatchForBanditCampGates" (that landed row binds
//   this spelling); engine/CQ_BanditCampBossBattleScript.h declares it at 0x00D04770. Was: ?EndMission@CV_ChapelOfEvilScript@@QAEXXZ
//   (a label propagated across bodies).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface IsRegionLoaded / GetThingWithScriptName / OpenDoor /
//   NewScriptFrame; ~CScriptThing 0x004AA840; CCharString 0x0099EBF0 / ~ 0x0099EAE0.
#include "fable_compiler.h"
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CQ_BanditCampBossBattleScript.h"
#include "engine/CGameScriptInterface.h"

void NScript::CQ_BanditCampBossBattleScript::WatchForBanditCampGates()
{
    while (!IsActiveThreadTerminating()) {
        if (Game->IsRegionLoaded("BanditCampCentre")) {
            if (IsActiveThreadTerminating())
                return;
            CScriptThing gate = Game->GetThingWithScriptName("Gate2Inner");
            Game->OpenDoor(gate);
            while (!(!Game->IsRegionLoaded("BanditCampCentre"))) {   // WAIT_UNTIL(!loaded)
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
        Game->NewScriptFrame();
    }
}