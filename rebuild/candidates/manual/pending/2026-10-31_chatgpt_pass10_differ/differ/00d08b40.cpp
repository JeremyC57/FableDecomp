// 00d08b40 NScript::CQ_BanditCampBossBattleScript::CCampHostageDoor::Main
// Leaf: ?Main@CCampHostageDoor@CQ_BanditCampBossBattleScript@NScript@@EAEXXZ
// Flags:
#include <new>
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCountedPointer.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CIsThingAlive.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CDefPointer.h"
#include "engine/CParentDefClassBase.h"
class CCharStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
__forceinline CScriptThing::~CScriptThing(){}
inline bool CScriptThing::IsAlive()const{return PImp.Get()&&PImp->IsAlive();}
inline NScript::CIsThingAlive::CIsThingAlive(const CScriptThing&thing):Thing(thing){}
__forceinline NScript::CIsThingAlive::~CIsThingAlive(){
#pragma inline_depth(0)
}
#pragma inline_depth(255)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing&,const CScriptThing&,float);
// Layout: donor CParentDefClassBase3d becomes retail39; first scalar donor40 becomes
// native3c. EXPERIENCE_GRANT_LARGE donor48 is native44, independently used here.
class CScriptDef:public CParentDefClassBase{unsigned char unobservedSmallAndMediumGrants[0x44-0x39];public:long EXPERIENCE_GRANT_LARGE;};
extern CDefPointer<const CScriptDef>pGlobalScriptDef;
namespace NScript{
class CQ_SunnyvaleMasterData;
// Layout: shared level-script base48 versus donor5c. Original flags retain their
// relative positions: HostagesRescued4e, PlayGuardTooCloseCutscene63.
class CQ_BanditCampBossBattleScript:public CLevelScriptBase<CQ_SunnyvaleMasterData>{
 unsigned char unobservedGateFlags[6];public:bool HostagesRescued;
 private:unsigned char unobservedQuestState[0x63-0x4f];public:bool PlayGuardTooCloseCutscene;
 class CCampHostageDoor;
};
class CQ_BanditCampBossBattleScript::CCampHostageDoor:public CEntityScriptBase{
 protected:CQ_BanditCampBossBattleScript&ParentClass;CQ_SunnyvaleMasterData*PMasterData;
 private:virtual void Main();
};
void CQ_BanditCampBossBattleScript::CCampHostageDoor::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptThing guard = Game->GetThingWithScriptName("CampHostageGuard");
    if (!IsActiveThreadTerminating()) {
        do {
            if (!ParentClass.HostagesRescued && Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
                if (IsActiveThreadTerminating()) {
                #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                }
                if (Game->IsObjectInThingsPossession("OBJECT_BANDIT_CAMP_HOSTAGE_KEY", Game->GetHero())) {
                    if (IsActiveThreadTerminating()) {
                        #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                    }
                    if (guard.IsAlive() && IsDistanceBetweenThingsUnder(Game->GetHero(), guard, 5.0f)) {
                        if (IsActiveThreadTerminating()) {
                                #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                        }
                        ParentClass.PlayGuardTooCloseCutscene = true;
                        while (ParentClass.PlayGuardTooCloseCutscene) {
                            Game->NewScriptFrame();
                            if (IsActiveThreadTerminating()) {
                                        #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                            }
                        }
                    } else {
                        if (IsActiveThreadTerminating()) {
                                #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                        }
                        Game->GiveHeroExperience(pGlobalScriptDef->EXPERIENCE_GRANT_LARGE);
                        ParentClass.HostagesRescued = true;
                        Game->TakeObjectFromHero("OBJECT_BANDIT_CAMP_HOSTAGE_KEY");
                        #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                    }
                } else {
                    if (IsActiveThreadTerminating()) {
                        #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                    }
                    Game->DisplayGameInfo("TEXT_QST_009_NEED_KEY");
                    while (!Game->MsgIsGameInfoClickedPast()) {
                        Game->NewScriptFrame();
                        if (IsActiveThreadTerminating()) {
                                #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                        }
                    }
                }
                if (IsActiveThreadTerminating()) {
                #pragma inline_depth(0)
                        return;
#pragma inline_depth(255)
                }
            }
            Game->NewScriptFrame();
        } while (!IsActiveThreadTerminating());
    }
}

}
