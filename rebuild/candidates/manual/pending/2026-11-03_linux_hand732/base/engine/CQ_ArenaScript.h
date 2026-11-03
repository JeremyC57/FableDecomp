#ifndef FABLE_ENGINE_CQ_ARENASCRIPT_H
#define FABLE_ENGINE_CQ_ARENASCRIPT_H
// Original Ego_r TPI11b55/11bed; retail base and vector contraction, owner11c, CCellWhisper1c.
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
#include "stlport/fable_containers.h"
class CArenaRoundDef;
namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    enum MAIN_ARENA_STATES { MAS_ON_THE_WAY=0, MAS_INTRO_HALL=1, MAS_CELLS_PRE_ARENA_INTRO=2, MAS_CELLS_PRE_ARENA_NORMAL=3, MAS_CELLS_PRE_ARENA_CHAM_CUTSCENE=4, MAS_CELLS_PRE_ARENA_NORMAL_GO=5, MAS_IN_ARENA=6, MAS_CELLS_MID_ARENA=7, MAS_IN_ARENA_WHISPER=8, MAS_OUTRO_HALL=9, MAS_FINISHING=10 };
    CCharString CrowdLoopTags[4][5];
    std::vector<CArenaRoundDef> Rounds;
    long ArenaState;
    long ArenaRound;
    long ArenaRoundWave;
    long GoldMultiplier;
    bool ChamLeaving;
    long GoldTotal;
    bool InHitCutsceneAlready;
    bool NeedBertForSpeech;
    CScriptThing CellsVillage;
    bool PlayerLeaving;
    bool PlayerWon;
    long TotalCreatures[3];
    long ExtraCreatures;
    bool MissionSucceeded;
    bool MissionFailed;
    bool ArenaSpawnNeeded[16];
    long NewCrowdPoints;
    long NewCrowdHappiness;
    long NewCrowdBaseLevel;
    CTimer GlobalCrowdTimer;
    bool FinalBattleCS;
    CScriptThing Whisper;
    bool PauseCrowdChecker;
    bool WhisperNeededForCutscene;
    long StopTimeIndex;
    virtual void RegisterMain();
    virtual void Init();
    virtual void Main();
    virtual void OnPersist(CPersistContext&);
    void InitialiseVariables();
    void GivePrizeFund();
    void CrowdChecker();
    CCharString GetEndRoundQuestion();
    EMusicSetType GetFanfareMusic(int);
    void AnimateCrowd();
    void PlayWave();
    void WatchForTermination();
    class CCagedBalverine : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CArenaCellExitGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CWhisperAlly : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CArenaObject : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CArenaSpawn : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CSUMMONED_CREATURE : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CArenaEnemy : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        long HealthBarIndex;
        bool BigCreature;
        long MyCreatureType;
        virtual void Init();
        virtual void OnPredicateFail();
        virtual void Main();
    };
    class CArenaCellDoorGuard2 : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CArenaCellDoorGuard : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CCellWhisper : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CShadow : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        long HintNumber;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CNeedle : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        long HintNumber;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CFlick : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        long HintNumber;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
    class CCham : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        virtual void Init();
        virtual void Main();
    };
    class CRoth : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CQ_ArenaScript&,CScriptDataBase*,const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
        CQ_SunnyvaleMasterData* GetMasterData();
    protected:
        CQ_ArenaScript& ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
    private:
        bool EarlyTalk;
        bool ChamTalk;
        long HintNumber;
        virtual void Init();
        virtual void OnPersist(CPersistContext&);
        virtual void Main();
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_ArenaScript)==0x11c);
FABLE_STATIC_ASSERT(sizeof(NScript::CQ_ArenaScript::CCellWhisper)==0x1c);
#endif
