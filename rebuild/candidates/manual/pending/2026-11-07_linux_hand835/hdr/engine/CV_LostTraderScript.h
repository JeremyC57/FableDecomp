#ifndef FABLE_ENGINE_CV_LOSTTRADERSCRIPT_H
#define FABLE_ENGINE_CV_LOSTTRADERSCRIPT_H
// PDB Ego_r TPI 0x11aaf and retail ctor 00ea2cf0: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: base 0x48; five flags 0x48..0x4c; DistanceForBrotherToReunite +0x50, ReasonForFailure +0x54; native Init 00ea2d20.
// Native owner ctor 00ea2cf0 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_LostTraderScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void WatchForQuestFailure();
#ifdef FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BANDITNAME
    // Retail opt-in (chunk 835): Ego_r.pdb ?GetRandomBanditName@CV_LostTraderScript@NScript@@QAE?AVCCharString@@XZ, the landed
    // 0x00EA5690 (hidden CCharString result); CLostTraderBanditSpawn::Main 0x00EA5230 calls it on ParentClass.
    CCharString GetRandomBanditName();                                   // 0x00EA5690
#endif
#ifdef FABLE_RETAIL_CV_LOSTTRADERSCRIPT_BROTHER
    // Retail opt-in (chunk 812): the thread CLostTraderBrother::Main 0x00EA59E0 spawns (CSpawnedFunc1<CV_LostTraderScript,CScriptThing>
    // vtable 0x012EE360, Func +0x34 = 0x00EA7460); Ego_r.pdb ?WatchForQuestFailureState@CV_LostTraderScript@NScript@@QAEXVCScriptThing@@@Z.
    void WatchForQuestFailureState(CScriptThing);                        // 0x00EA7460
#endif
    bool g_bLostTraderKilled;
    bool g_bTraderBrotherKilled;
    bool g_bQuestFailed;
    bool g_bTraderHit;
    bool ReachedTrader;
    long DistanceForBrotherToReunite;
    long ReasonForFailure;
    class CLostTraderBanditSpawn : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_LostTraderScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CLostTraderBrother : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_LostTraderScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long stateBrain;
    private:
        bool HasToldHeroWhereBrotherIs;
    private:
        long TalkCounter;
    private:
        bool DoneIntro;
    };
    class CLostTraderTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_LostTraderScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_LostTraderScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long stateBrain;
    private:
        long YellForHelp;
    private:
        long YellAtHero;
    private:
        long RandomComment;
#ifdef FABLE_RETAIL_CV_LOSTTRADERSCRIPT_TRADER
        // Retail opt-in (chunk 805): the trader's private helpers that Main 0x00EA36B0 calls (Ego_r.pdb
        // ?TraderYellForHelp / ?TraderYellAtHero / ?MakeRandomComment@CLostTraderTrader@CV_LostTraderScript@NScript@@AAEXXZ).
        void TraderYellForHelp();                                        // 0x00EA4980
        void TraderYellAtHero();                                         // 0x00EA4BB0
        void MakeRandomComment();                                        // 0x00EA4DE0
#endif
    };
};
}
FABLE_STATIC_ASSERT(sizeof(NScript::CV_LostTraderScript) == 0x58);
#endif
