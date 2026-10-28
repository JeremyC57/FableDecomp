// CMurderTwistSheriff::Main() -- the Murder Twist sheriff: alive filter on Me, melee-guard brain, scripted control (priority 4, retried each frame), registers himself with the parent quest, then runs his state machine: ask the hero to kill the assassin, remind him, play the being-rumbled cutscene once the hero is within 7 m, grow suspicious, and finally turn on the hero
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EAEF30, 675 bytes (+ int3 padding); entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90, ~CIsThingAlive 0x00E241B0 (out of line), SetPredicate 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, the scripted-thing
//   handle's IsScripting 0x00CD23B9 / Release 0x00CD2770 / operator= 0x008ABD10, CScriptThing::operator= 0x008AB980,
//   GetSquaredDistanceBetweenThings 0x00CBE583 (__fastcall, float); own state handlers 0x00EAF810, 0x00EB0830, 0x00EB10C0, 0x00EB15C0,
//   0x00EB19C0, 0x00EB1EC0; CV_MurderTwistFinalScript::PlayBeingRumbledCutscene 0x00EAF1E0; GameCalls slots 7, 8, 70, 589.
// Layout: CMurderTwistSheriff (Ego_r.pdb 0x38, matches retail; see the landed Init 0x00EAEE80). The parent quest script
//   CV_MurderTwistFinalScript is 0x14 smaller in retail than in the PDB view (SheriffState PDB +0x5c = retail +0x48,
//   AssassinState +0x60 = +0x4c, Sheriff +0x68 = +0x54, seh_Sheriff +0x84 = +0x6c after the 12-byte Guards vector).
// Shape: generated script code -- `if (IsActiveThreadTerminating()) return;` ahead of each statement that follows a wait.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // 12 bytes, vtable 0x01238C8C
    virtual void v00();
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    ~CScriptThing();                                   // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
class CScriptGameResourceObjectScriptedThingBase { public:    // 0x10 bytes
    unsigned char _body[0x10];
    bool IsScripting();                                // 0x00CD23B9
    void Release();                                    // 0x00CD2770
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
float __fastcall GetSquaredDistanceBetweenThings(const CScriptThing& a, const CScriptThing& b);   // 0x00CBE583
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CGameScriptInterface { public:                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20) 0x0089B5B0
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016();
    virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032();
    virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040();
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206();
    virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246();
    virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278();
    virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286();
    virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302();
    virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318();
    virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366();
    virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374();
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382();
    virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390();
    virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398();
    virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406();
    virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414();
    virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422();
    virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430();
    virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438();
    virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446();
    virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454();
    virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462();
    virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470();
    virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478();
    virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486();
    virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494();
    virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502();
    virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510();
    virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518();
    virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526();
    virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534();
    virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542();
    virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550();
    virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558();
    virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566();
    virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574();
    virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582();
    virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588();
    virtual void SetCreatureBrain(const CScriptThing& thing, const CCharString& brain) const;   // slot 589 (+0x934) 0x00894B90
};

namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                 // vtable 0x012C32F0: a predicate holding a script thing
    CScriptThing m_thing;                              // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                  // 0x00E241B0 (out of line here)
};
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void v00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
enum ESheriffState { SHERIFF_STATE_0, SHERIFF_STATE_1, SHERIFF_STATE_2, SHERIFF_STATE_3 };   // enumerator names not in the PDB dump
enum EAssassinState { ASSASSIN_STATE_0, ASSASSIN_STATE_1, ASSASSIN_STATE_2, ASSASSIN_STATE_3, ASSASSIN_STATE_4 };
struct CV_MurderTwistFinalScript {
    unsigned char _pad00[0x48];
    ESheriffState  SheriffState;                       // +0x48
    EAssassinState AssassinState;                      // +0x4c
    bool           TaskDeclined;                       // +0x50
    bool           PreemptiveAttackInProgress;         // +0x51
    CScriptThing   Sheriff;                            // +0x54
    unsigned char  Guards[0xc];                        // +0x60 vector<CScriptThing> (retail STLport, 12 bytes)
    CScriptGameResourceObjectScriptedThingBase seh_Sheriff;   // +0x6c
    void PlayBeingRumbledCutscene();                   // 0x00EAF1E0
};
class CTimer { public: unsigned long _handle; };
struct CMurderTwistSheriff : CEntityScriptBase {
    CV_MurderTwistFinalScript* ParentClass;            // +0x14
    void*                      PMasterData;            // +0x18
    CScriptGameResourceObjectScriptedThingBase MyEntityHandle;   // +0x1c
    CTimer                     AttractTimer;           // +0x2c
    unsigned long              NextLine;               // +0x30
    bool HasGivenReminderToHero;                       // +0x34
    bool HasBeingRumbledCutscenePlayed;                // +0x35
    bool HasExpressedSuspicionToHero;                  // +0x36
    bool HasSaidWellDoneToHero;                        // +0x37
    void Main();
    void DoAskingHeroToKillAssassin();                 // 0x00EAF810
    void DoRemindingHeroToKillAssassin();              // 0x00EB0830
    void DoBecomingSuspiciousOfHero();                 // 0x00EB10C0
    void DoRemindingHeroToKillSheriff();               // 0x00EB15C0 (catalog files it under CMurderTwistAssassin; called on the sheriff here)
    void DoSuspicious();                               // 0x00EB19C0 (catalog: a second DoBecomingSuspiciousOfHero label)
    void DoFightHero();                                // 0x00EB1EC0 (catalog "Init": GiveThingBestEnemyTarget hero, killable, damageable)
};

void CMurderTwistSheriff::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating()) return;
    Game->SetCreatureBrain(Me, "BRAIN_MELEE_GUARD");
    if (MyEntityHandle.IsScripting())
        MyEntityHandle.Release();
    while (!Game->StartScriptingEntity(Me, MyEntityHandle, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
    }
    if (IsActiveThreadTerminating()) return;
    ParentClass->Sheriff = Me;
    ParentClass->seh_Sheriff = MyEntityHandle;
    while (ParentClass->SheriffState == SHERIFF_STATE_0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoAskingHeroToKillAssassin();
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->AssassinState == ASSASSIN_STATE_0 || ParentClass->AssassinState == ASSASSIN_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoRemindingHeroToKillAssassin();
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->SheriffState == SHERIFF_STATE_1) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        if (!HasBeingRumbledCutscenePlayed && GetSquaredDistanceBetweenThings(Me, Game->GetHero()) <= 49.0f) {
            if (IsActiveThreadTerminating()) return;
            ParentClass->PlayBeingRumbledCutscene();
            HasBeingRumbledCutscenePlayed = true;
        }
        if (ParentClass->AssassinState == ASSASSIN_STATE_3 || ParentClass->AssassinState == ASSASSIN_STATE_2) {
            if (IsActiveThreadTerminating()) return;
            DoBecomingSuspiciousOfHero();
        }
        if (ParentClass->AssassinState == ASSASSIN_STATE_4) {
            if (IsActiveThreadTerminating()) return;
            DoRemindingHeroToKillSheriff();
        }
    }
    if (IsActiveThreadTerminating()) return;
    while (ParentClass->SheriffState == SHERIFF_STATE_3) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating()) return;
        DoSuspicious();
    }
    if (IsActiveThreadTerminating()) return;
    if (ParentClass->SheriffState == SHERIFF_STATE_2) {
        if (IsActiveThreadTerminating()) return;
        DoFightHero();
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}