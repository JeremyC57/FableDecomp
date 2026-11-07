#ifndef FABLE_ENGINE_CV_BODYGUARDSCRIPT_H
#define FABLE_ENGINE_CV_BODYGUARDSCRIPT_H
// Retail opt-in (Codex pass 12): Ego_r/FableWin complete original class.
// Donor base92 becomes72, vector16 becomes12. Native idle/comment fields prove
// HenchmanList +0x64, ConversationIndex +0x70, and complete native size0x94.
#include "engine/CLevelScriptBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
#include "engine/CCounter.h"
#include "engine/CTimer.h"
#ifdef FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN
// Retail opt-in (chunk 815, CBodyGuard::Main 0x00E5FD40): the nested entity script CBodyGuard (Ego_r.pdb, 60 bytes; struct_layouts_egor.tsv):
// CEntityScriptBase (0x14), ParentClass +0x14, PMasterData +0x18, HenchManType +0x1c, BrainState +0x20, CurrentHealthState +0x24,
// PreviousHealthState +0x28, FreakedOut +0x2c, AngerTimer +0x30, AttackedBySummonersTimer +0x34, MyHealthBarID +0x38 -- the offsets
// Main reads. Entity vtable 0x012E701C (stored by the ctor 0x00E5DDE0 at 0x00E5DE01): 0 scalar deleting dtor 0x00E5DE50, 1 Main
// 0x00E5FD40, 2 Init 0x00E5DE80, 3 GetParentScript 0x00E5DE40, 4 OnPersist 0x00E5F190, 5 OnPredicateFail 0x00E5FB80. Spellings:
// Ego_r.pdb / FableWin.pdb ?<Name>@CBodyGuard@CV_BodyGuardScript@NScript@@ (Alloc SI, Main / Init / OnPersist / OnPredicateFail EAE,
// SetHenchmanAsHired AAEX_N, GetGoldCost AAEJXZ).
#include "rebuild_abi.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CEntityScriptBase.h"
#endif
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
#ifdef FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN
    class CBodyGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&); // 0x00E5DD60
        CBodyGuard(CV_BodyGuardScript&, CScriptDataBase*, const CScriptThing&); // 0x00E5DDE0
        virtual CScriptBase* GetParentScript() const;                    // 0x00E5DE40
    private:
        virtual void Main();                                             // 0x00E5FD40
        virtual void Init();                                             // 0x00E5DE80
        virtual void OnPersist(CPersistContext&);                        // 0x00E5F190
        virtual void OnPredicateFail();                                  // 0x00E5FB80
        void SetHenchmanAsHired(bool);                                   // 0x00E621F0
        long GetGoldCost();                                              // 0x00E5FC70
    protected:
        CV_BodyGuardScript& ParentClass;                                 // +0x14
        CQ_SunnyvaleMasterData* PMasterData;                             // +0x18
    private:
        long HenchManType;                                               // +0x1c
        long BrainState;                                                 // +0x20
        long CurrentHealthState;                                         // +0x24
        long PreviousHealthState;                                        // +0x28
        bool FreakedOut;                                                 // +0x2c
        CTimer AngerTimer;                                               // +0x30
        CTimer AttackedBySummonersTimer;                                 // +0x34
        long MyHealthBarID;                                              // +0x38
    };
#endif
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BodyGuardScript)==0x94);
#ifdef FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN
FABLE_STATIC_ASSERT(sizeof(NScript::CV_BodyGuardScript::CBodyGuard)==0x3c);
#endif
#endif
