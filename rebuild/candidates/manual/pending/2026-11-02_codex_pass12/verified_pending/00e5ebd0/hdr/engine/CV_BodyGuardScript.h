#ifndef FABLE_ENGINE_CV_BODYGUARDSCRIPT_H
#define FABLE_ENGINE_CV_BODYGUARDSCRIPT_H
// Retail opt-in (Codex pass 12): Ego_r/FableWin complete original class.
// Donor base92 becomes72, vector16 becomes12. Native idle/comment fields prove
// HenchmanList +0x64, ConversationIndex +0x70, and complete native size0x94.
#include "engine/CLevelScriptBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CCounter.h"
#include "engine/CTimer.h"
namespace NScript {
class CV_BodyGuardScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CV_BodyGuardScript(CScriptDataBase*,CGameScriptInterfaceBase&);
    static CScriptBase* __fastcall Alloc(CScriptDataBase*,CGameScriptInterfaceBase&);
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    long TotalWages;
    bool HeroCannotAffordHelp;
    bool HeroWieldingWeapon;
    CCounter HeroStoodStillCounter;
    CTimer HenchmanLastIdleChatTimer;
    CTimer HenchmanLastBoredChatTimer;
    CTimer CommentaryTimer;
    CTimer HenchmanFreakedOutThePlayerTimer;
    std::vector<CScriptThing> HenchmanList;
    long ConversationIndex;
    enum CONVERSATION_STATES { CS_IDLE=1, CS_COMMENT_SCRIPTED=2 };
    enum EHENCHMAN_COMMENT { HM_COMMENT_NONE=0, HM_COMMENT_MULTIPLE=1, HM_COMMENT_SINGLE=2, HM_COMMENT_MULTIPLE_RANDOM=3 };
    CTimer HenchmanDeadRespawnTimer[4];
    long HenchmanGlobalStatus[4];
    long GetHenchmanTypeFromString(const CCharString&);
    bool MakeHenchmanPerformIdleComment();
    bool MakeHenchmanComment(const CCharString&,long,EHENCHMAN_COMMENT,const CScriptThing& = CScriptThing());
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BodyGuardScript)==0x94);
#endif
