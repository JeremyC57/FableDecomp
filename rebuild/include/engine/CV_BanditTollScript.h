#ifndef FABLE_ENGINE_CV_BANDITTOLLSCRIPT_H
#define FABLE_ENGINE_CV_BANDITTOLLSCRIPT_H
// PDB Ego_r TPI 0x10d13 and retail ctor 00e478a0: CLevelScriptBase<NScript::CQ_SunnyvaleMasterData>.
// Layout: base 0x48; ten flags 0x48..0x51; BanditTollFee +0x54, GivenQuestCard +0x58; native Init proves these.
// Native owner ctor 00e478a0 and registered factory confirm this layout; donor containers are larger.
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CEntityScriptBase.h"
#include "engine/CTimer.h"
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_BanditTollScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    virtual void RegisterMain();
    virtual void Main();
    virtual void Init();
    virtual void OnPersist(CPersistContext&);
    void CheckForLeaderDead();
    void ResetToll();
#ifdef FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT
    // Retail opt-in (chunk 816): ?CheckForBanditTollPaid@CV_BanditTollScript@NScript@@QAEXXZ (Ego_r.pdb / FableWin.pdb), the
    // member the CTollBandit::Main 0x00E4B5B0 spawns as "ParentClass.CheckForBanditTollPaid" (CSpawnedFunc vtable 0x012E4B90).
    void CheckForBanditTollPaid();
#endif
    bool BanditsAngered;
    bool TollPaid;
    bool AssassinateReluctantTrader;
    bool TollBanditIsAfterHero;
    bool TraderDead;
    bool SafeTraderAppeared;
    bool LeaderDead;
    bool BanditsFlee;
    bool BanditsReachedMarker;
    bool IsFactionSet;
    long BanditTollFee;
    bool GivenQuestCard;
    class CBanditLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long TalkedToHero;
    private:
        CTimer TalkFrequency;
    };
    class CBlockingBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CGuildSealWarning : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CM_BanditLeader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CM_TauntingBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CM_TollBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CReluctantTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    };
    class CSafeTrader : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        bool WarnedHero;
    private:
        bool AttackedByHero;
    };
    class CTauntingBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long AngerCount;
    private:
        bool AngryWithHero;
    };
    class CTollBandit : public CEntityScriptBase {
    public:
        static CCountedPointer<CEntityScriptBase> FABLE_FASTCALL Alloc(CV_BanditTollScript&, CScriptDataBase*, const CScriptThing&);
        virtual CScriptBase* GetParentScript() const;
    private:
        virtual void Main();
#ifdef FABLE_RETAIL_CV_BANDITTOLLSCRIPT_TOLLBANDIT
        // Retail opt-in (chunk 816): Ego_r.pdb / FableWin.pdb ?SetUpBanditToll@CTollBandit@..@@AAEXXZ (0x00E4E0E0, called first by
        // Main 0x00E4B5B0) and ?Should_I_Flee@CTollBandit@..@@AAEXAAVCScriptGameResourceObjectScriptedThingBase@@@Z (0x00E4DF20,
        // called at the end of every Main state loop with seh_me).
        void SetUpBanditToll();
        void Should_I_Flee(CScriptGameResourceObjectScriptedThingBase&);
#endif
    protected:
        NScript::CV_BanditTollScript & ParentClass;
    protected:
        NScript::CQ_SunnyvaleMasterData * PMasterData;
    private:
        long TollState;
    private:
        bool TollBanditOnlyCollectsToll;
    private:
        bool KilledReluctantTrader;
    private:
        long TollBanditPayMeDistance;
    private:
        CScriptThing BanditEscapeMarker;
    };
#ifdef FABLE_RETAIL_CV_BANDITTOLLSCRIPT_ALLOC
    // Retail opt-in (NF436, row 0x00E4F550 Alloc): FableWin.pdb ??_7CV_BanditTollScript@NScript@@6B@ / LF_CLASS (all public: the user ctor, static
    //   Alloc, the virtual overrides) for what this header does not declare yet: CV_BanditTollScript, Alloc. Declarations only, no layout
    //   change; existing includers do not define the macro.
public:
    CV_BanditTollScript(CScriptDataBase* data, CGameScriptInterfaceBase& game); // FableWin.pdb ??0CV_BanditTollScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
    static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&);   // 0x00E4F550
#endif
};
}
FABLE_STATIC_ASSERT_AT(169, sizeof(NScript::CV_BanditTollScript) == 0x5c);
#endif
