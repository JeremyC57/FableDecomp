// CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game) -- quest-script ctor: script base, interface/data pair, vtable 0x012EAF14, the two fighter timers, the SFighterOne/SFighterTwo/Village script things and the five RegionNames strings
// Leaf: ??0CV_FisticuffsClubScript
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E81F60, 167 bytes (padded to 176), `ret 8`.
// Identity: factory 0x00E8B700 (`push 0C8h`, this ctor) registered at 0x00CD8968; size 0xc8 = Ego_r.pdb 220 - 0x14
//   (the retail CScriptBase is 0x14 bytes smaller), the members line up at PDB - 0x14.
// Callees: CScriptBase::CScriptBase 0x00CB8110, CGameScriptInterface slot 87 RegisterTimer (+0x15c, through the
//   global interface 0x0143E8F8), CBaseClass::CBaseClass 0x0099A2D0 (CScriptThing base, CScriptThing vtable 0x01238C8C),
//   CCharString::CCharString() 0x0099E4B0 (the RegionNames array, an inline counted `dec ebx ; jne` loop).
// Shape: the two argument words stored before the derived vtable are the inlined `__declspec(novtable)` intermediate base
//   (landed CV_MurderTwistFinalScript / CV_BordelloScript model); timers are member objects whose inline ctor calls the
//   interface; script things are CBaseClass + zeroed counted pointer; plain scalars are left uninitialised.
// Layout (retail = PDB - 0x14): RelevantRegion +0x48, flags +0x4c..+0x58, Fighter1Timer +0x5c, Fighter2Timer +0x60,
//   SFighterOne +0x64, SFighterTwo +0x70, HitID..FightCount +0x7c..+0x88, flags +0x8c, AllowedHits +0x90,
//   flags +0x94..+0x98, Village +0x9c, PlayerLost +0xa8, CheerNumber +0xac, BooNumber +0xb0, RegionNames +0xb4.
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C (only slot 87 matters here)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();                  // slot 87 (+0x15c)
};
extern CGameScriptInterface* g_pGameScriptInterface;  // 0x0143E8F8
struct CTimer {                                    // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
};
struct CScriptBase {                               // 0x40 bytes, the quest-script root (vtable 0x012C1648)
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                     // +0x04..+0x3f
    CScriptBase();                                 // 0x00CB8110
};
struct __declspec(novtable) CQuestScriptObject : CScriptBase {   // the argument-carrying intermediate base
    CGameScriptInterface* Game;                    // +0x40 (second argument)
    void*                 Data;                    // +0x44 (first argument, the script data)
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CBaseClass { virtual ~CBaseClass(); CBaseClass(); };   // 0x0099A2D0 (vtable 0x01231710)
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass {                 // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThing> PImp;            // +0x04
    CScriptThing() : CBaseClass(), PImp() { }
    virtual ~CScriptThing();
};
struct CCharString {                               // 4 bytes (data block pointer)
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
};
struct CV_FisticuffsClubScript : CQuestScriptObject {
    long RelevantRegion;                           // +0x48
    bool FighterOneInRing, FighterTwoInRing, ChallengeAccepted, FightFinished, HeroInsideRing,
         HeroOfferedFisticuffsClubEntry, HeroPaidEntryFee, HeroInFisticuffsClub, TylerInFight, HeroBeatTyler,
         TimeOut, RingCleared, RoundStarted;       // +0x4c..+0x58
    CTimer Fighter1Timer;                          // +0x5c
    CTimer Fighter2Timer;                          // +0x60
    CScriptThing SFighterOne;                      // +0x64
    CScriptThing SFighterTwo;                      // +0x70
    long HitID;                                    // +0x7c
    long HitCount;                                 // +0x80
    long AttackWarnings;                           // +0x84
    long FightCount;                               // +0x88
    bool EverybodyAttack, LostWhileOutside, FisticuffsActive;   // +0x8c
    long AllowedHits;                              // +0x90
    bool CompletedThisPunchClub, CompletedKnotholePunchClub, CompletedBowerstonePunchClub,
         CompletedBanditPunchClub, CompletedOakvalePunchClub;   // +0x94..+0x98
    CScriptThing Village;                          // +0x9c
    bool PlayerLost;                               // +0xa8
    float CheerNumber;                             // +0xac
    float BooNumber;                               // +0xb0
    CCharString RegionNames[5];                    // +0xb4
    virtual ~CV_FisticuffsClubScript();
    CV_FisticuffsClubScript(void* data, CGameScriptInterface* game);
};
CV_FisticuffsClubScript::CV_FisticuffsClubScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
