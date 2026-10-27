// NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game) -- quest-script constructor: the CScriptBase root, the interface and master-data pair, vtable 0x012DAF78, then the members' constructors: the comment timer, an empty crate list, four fail-reason wide strings, the objectives-reminder timer, the text-system script code string and the two crate teams (timer, two empty script things, state counters and member count zeroed)
// Leaf: ??0CQ_OrchardFarmRaidScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCC040, 242 bytes, thiscall `ret 8`, returns this.
// Identity (proven): it stores the quest vtable 0x012DAF78 whose slot 2 is the landed CQ_OrchardFarmRaidScript::Main 0x00DCC770
//   (and slot 1 the landed RegisterMain 0x00DCC660); its only caller 0x00DD1F0B is the quest's factory. Ego_r.pdb and
//   FableWin.pdb have ??0CQ_OrchardFarmRaidScript@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z.
// Label: the catalog's "_global::CHeroMorphDef_CopyOrInit" is a propagated label.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CWideString::CWideString() 0x0099AED0 (x4, counted loop), CCharString::CCharString()
//   0x0099E4B0, CBaseClass::CBaseClass 0x0099A2D0 (the CScriptThing default ctors); CTimer() inline: the global script interface
//   0x0143E8F8 slot 87 RegisterTimer.
// Layout: Ego_r.pdb CQ_OrchardFarmRaidScript, retail = Ego_r - 0x14 (CScriptBase 0x40), 4 lower again after the STLport CrateList
//   vector: CrateCount +0x48, CommentTimer +0x4c, CrateList +0x54, MissionSucceeded +0x60, MissionFailed +0x64, FailReasons +0x68,
//   HeroTeam +0x78, RemindHeroOfObjectivesTimer +0x80, TextSystemScriptCode +0x84, Teams (CCrateTeamManager[2], 0x40 each) +0x8c;
//   0x10c bytes. CCrateTeamManager (Ego_r.pdb, no STL): StateCounter[6], MemberCount +0x18, TeamReinforcementsTimer +0x1c,
//   MaxTeamMembers +0x20, CrateDropPos +0x24, TeamCrateCarrier +0x30, EnemyTeam +0x3c; its ctor (?? 0CCrateTeamManager in
//   Ego_r.pdb) is inlined: member ctors, then the six counters and the member count zeroed.
// Shape: the Teams loop counter lives in the dead `game` argument slot.
class CScriptDataBase;
class CGameScriptInterfaceBase {
public:
    virtual ~CGameScriptInterfaceBase();
    virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008();
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071(); virtual void v072();
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                         // slot 87 (+0x15c)
};
class CGameScriptInterface : public CGameScriptInterfaceBase { };
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface, bound name)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
};
class CCharString {                                     // 4 bytes
public:
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {                                     // 4 bytes
public:
    void* PStringData;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
};
class CBaseClass {                                      // vtable 0x01231710
public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();                            // 0x004AA840
    CCountedPointer<CScriptThing> PImp;                 // +0x04
};
namespace std {
template <class T> class allocator { };
template <class T, class A = allocator<T> > class vector {   // STLport 4.6 layout (0x0C)
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();
};
}
class CScriptBase {                                     // retail 0x40 bytes (Ego_r.pdb 0x54 with Dinkumware containers)
public:
    CScriptBase();                                      // 0x00CB8110
    virtual ~CScriptBase();
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CLevelScriptBase(CScriptDataBase* data, CGameScriptInterfaceBase& game)
        : CScriptBase(), Game(&game), MasterScriptData((TData*)data) { }
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData*                    MasterScriptData;         // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_OrchardFarmRaidScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CCrateTeamManager {                           // Ego_r.pdb, 0x40 bytes
    public:
        CCrateTeamManager()
        {
            for (int i = 0; i < 6; ++i)
                StateCounter[i] = 0;
            MemberCount = 0;
        }
        long StateCounter[6];                           // +0x00
        long MemberCount;                               // +0x18
        CTimer TeamReinforcementsTimer;                 // +0x1c
        long MaxTeamMembers;                            // +0x20
        CScriptThing CrateDropPos;                      // +0x24
        CScriptThing TeamCrateCarrier;                  // +0x30
        CCrateTeamManager* EnemyTeam;                   // +0x3c
    };
    CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game);
    virtual ~CQ_OrchardFarmRaidScript();
    long CrateCount;                                    // +0x48 (Ego_r.pdb +0x5c)
    CTimer CommentTimer;                                // +0x4c
    bool DoneIntroduction;                              // +0x50
    bool HeroAtWrongEntrance;                           // +0x51
    bool WhisperSpawned;                                // +0x52
    std::vector<CScriptThing> CrateList;                // +0x54 (Ego_r.pdb +0x68)
    bool MissionSucceeded;                              // +0x60
    long MissionFailed;                                 // +0x64 (Ego_r.pdb unnamed enum type)
    CWideString FailReasons[4];                         // +0x68
    long HeroTeam;                                      // +0x78
    long BanditWavesSpawned;                            // +0x7c
    CTimer RemindHeroOfObjectivesTimer;                 // +0x80
    CCharString TextSystemScriptCode;                   // +0x84
    bool HeroMetWhisperBeforeFarm;                      // +0x88
    bool ShownCombatMultiplierTutorial;                 // +0x89
    bool ShownCombatFlourishTutorial;                   // +0x8a
    bool WhisperInCutscene;                             // +0x8b
    CCrateTeamManager Teams[2];                         // +0x8c (Ego_r.pdb +0xa4)
};
}

NScript::CQ_OrchardFarmRaidScript::CQ_OrchardFarmRaidScript(CScriptDataBase* data, CGameScriptInterfaceBase& game)
    : CLevelScriptBase<CQ_SunnyvaleMasterData>(data, game)
{
}