// CQ_NewOakValeIntroScript::PostAttackStuff() -- wait for the post-attack start marker to appear, then teleport the hero to it, put Oakvale in limbo, take the teddy bear, log the story, reset the camera and music, fade in, wait for the hero to reach his dad's trigger and play the "he's dead, Jim" cut-scene, then lift the limbo, restart the clock and close the post-attack quest
// Leaf: PostAttackStuff
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DBEB20, 1095 bytes.
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0, ~CScriptThing 0x004AA840 (and its inline body: counted-pimpl release,
//   operator delete 0x00BFE9BC, ~CBaseClass 0x0099A2E0), IsActiveThreadTerminating 0x00CB7940, AddLogbookStoryEntry
//   0x00CBE87F and IsDistanceBetweenThingsUnder 0x00CBE2FF and RunCutsceneMacro 0x00CBFB7D (all __fastcall); handle / cast
//   map / movie helpers as the landed CRockTrollTrigger::Main 0x00E0AD40; CGameScriptInterface slots 7, 8, 70, 72, 125, 280,
//   341, 370, 371, 374, 379, 416, 417, 440, 472, 648, 657, 697, 698.
// Layout: Game +0x40, TimeStoppedToken +0x4c, HesDeadJimStarted +0x51.
// Shape: the marker test's CScriptThing temporary is destroyed inline (VC7.1 inlines the dtor at that first site only).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
struct CWideString {
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
struct CBaseClass {                                     // vtable 0x01231710
    virtual void v00();
    ~CBaseClass();                                      // 0x0099A2E0
};
struct SCountBlock {
    long  m_references;                                 // +0
    void (__fastcall* m_pfnDestroy)(void*);             // +4
    void* m_pObject;                                    // +8
};
struct CScriptThingImp;
struct CScriptThingPImp {                               // CCountedPointer<CScriptThingImp>
    CScriptThingImp* Data;
    SCountBlock*     Info;
    __forceinline ~CScriptThingPImp()
    {
        if (Info) {
            --Info->m_references;
            if (Info->m_references == 0) {
                Info->m_pfnDestroy(Info->m_pObject);
                operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};

struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CScriptThingPImp PImp;
    virtual void t00();
    virtual void t01(); virtual void t02(); virtual void t03(); virtual void t04(); virtual void t05(); virtual void t06(); virtual void t07(); virtual void t08(); virtual void t09();
    virtual void t10(); virtual void t11(); virtual void t12(); virtual void t13(); virtual void t14(); virtual void t15(); virtual void t16(); virtual void t17(); virtual void t18(); virtual void t19();
    virtual void t20(); virtual void t21(); virtual void t22(); virtual void t23(); virtual void t24(); virtual void t25(); virtual void t26(); virtual void t27(); virtual void t28(); virtual void t29();
    virtual void t30(); virtual void t31(); virtual void t32(); virtual void t33(); virtual void t34(); virtual void t35(); virtual void t36(); virtual void t37(); virtual void t38(); virtual void t39();
    virtual void t40(); virtual void t41(); virtual void t42(); virtual void t43(); virtual void t44(); virtual void t45(); virtual void t46(); virtual void t47(); virtual void t48(); virtual void t49();
    virtual void t50(); virtual void t51(); virtual void t52(); virtual void t53(); virtual void t54(); virtual void t55(); virtual void t56(); virtual void t57(); virtual void t58(); virtual void t59();
    virtual void t60(); virtual void t61(); virtual void t62(); virtual void t63(); virtual void t64(); virtual void t65(); virtual void t66(); virtual void t67(); virtual void t68(); virtual void t69();
    virtual void t70(); virtual void t71(); virtual void t72(); virtual void t73();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c) (slot 0 is CBaseClass's v00)
    inline ~CScriptThing() {}                           // out of line: 0x004AA840
};
struct CScriptGameResourceObjectScriptedThingBase {    // vtable 0x0127094C, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x007E72A0 (out-of-line ctor)
    void Destroy();                                    // 0x007E74D0 (out-of-line dtor)
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
struct CScriptGameResourceObjectMovieBase {            // vtable 0x01260EF4, 0x10 bytes
    void* _body[4];
    void Construct();                                  // 0x006E7B60
    void Destroy();                                    // 0x006E7B80
};
struct CCutsceneCastMap {                              // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    void Construct();                                  // 0x00CDBF70
    void Destroy();                                    // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, long priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018();
    virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038();
    virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058();
    virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092();
    virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124();
    virtual void TakeObjectFromHero(const CCharString& object) const;   // slot 125 (+0x1f4)
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165();
    virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205();
    virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245();
    virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265();
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void DeactivateQuest(const CCharString& quest, unsigned long flags) const;   // slot 280 (+0x460)
    virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300();
    virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310();
    virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340();
    virtual void DisplayMoneyBag(bool show) const;   // slot 341 (+0x554)
    virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351();
    virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361();
    virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void FixMovieSequenceCamera(bool fix) const;   // slot 371 (+0x5cc)
    virtual void v372(); virtual void v373();
    virtual void FadeScreenIn();   // slot 374 (+0x5d8)
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool pause);   // slot 379 (+0x5ec)
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389();
    virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399();
    virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409();
    virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415();
    virtual void CameraDefault();   // slot 416 (+0x680)
    virtual void CameraResetToViewBehindHero(float time);   // slot 417 (+0x684)
    virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427();
    virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435(); virtual void v436(); virtual void v437();
    virtual void v438(); virtual void v439();
    virtual void SetVillageLimbo(const CScriptThing& village, bool on) const;   // slot 440 (+0x6e0)
    virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450();
    virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460();
    virtual void v461(); virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470();
    virtual void v471();
    virtual void EntityTeleportToThing(const CScriptThing& thing, const CScriptThing& target, bool a) const;   // slot 472 (+0x760)
    virtual void v473(); virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482();
    virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492();
    virtual void v493(); virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502();
    virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511(); virtual void v512();
    virtual void v513(); virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522();
    virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531(); virtual void v532();
    virtual void v533(); virtual void v534(); virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542();
    virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552();
    virtual void v553(); virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562();
    virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566(); virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572();
    virtual void v573(); virtual void v574(); virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582();
    virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590(); virtual void v591(); virtual void v592();
    virtual void v593(); virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600(); virtual void v601(); virtual void v602();
    virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607(); virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612();
    virtual void v613(); virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621(); virtual void v622();
    virtual void v623(); virtual void v624(); virtual void v625(); virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631(); virtual void v632();
    virtual void v633(); virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637(); virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641(); virtual void v642();
    virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647();
    virtual void SetTimeAsStopped(bool stopped, long& token) const;   // slot 648 (+0xa20)
    virtual void v649(); virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653(); virtual void v654(); virtual void v655(); virtual void v656();
    virtual void ResetToDefaultTheme(float time) const;   // slot 657 (+0xa44)
    virtual void v658(); virtual void v659(); virtual void v660(); virtual void v661(); virtual void v662(); virtual void v663(); virtual void v664(); virtual void v665(); virtual void v666(); virtual void v667();
    virtual void v668(); virtual void v669(); virtual void v670(); virtual void v671(); virtual void v672(); virtual void v673(); virtual void v674(); virtual void v675(); virtual void v676(); virtual void v677();
    virtual void v678(); virtual void v679(); virtual void v680(); virtual void v681(); virtual void v682(); virtual void v683(); virtual void v684(); virtual void v685(); virtual void v686(); virtual void v687();
    virtual void v688(); virtual void v689(); virtual void v690(); virtual void v691(); virtual void v692(); virtual void v693(); virtual void v694(); virtual void v695(); virtual void v696();
    virtual void StopOverrideMusic(bool immediate) const;   // slot 697 (+0xae4)
    virtual void CacheMusicSet(long set) const;   // slot 698 (+0xae8)
};
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_NewOakValeIntroScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[4];
    long                  TimeStoppedToken;             // +0x4c
    bool                  IntroDone;                    // +0x50
    bool                  HesDeadJimStarted;            // +0x51
    void PostAttackStuff();
};

void CQ_NewOakValeIntroScript::PostAttackStuff()
{
    while (!Game->GetThingWithScriptName("M_PostAttackStart").IsAlive()) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
#pragma inline_depth(0)
    Game->CacheMusicSet(0x2d);
    Game->EntityTeleportToThing(Game->GetHero(), Game->GetThingWithScriptName("M_PostAttackStart"), false);
    Game->SetVillageLimbo(Game->GetThingWithScriptName("V_OakVale"), true);
    Game->DisplayMoneyBag(false);
    Game->TakeObjectFromHero("OBJECT_TEDDY_BEAR_UNGIVEABLE");
    AddLogbookStoryEntry(0x14);
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    Game->CameraResetToViewBehindHero(0.0f);
    Game->CameraDefault();
    Game->CacheMusicSet(0x39);
    Game->FadeScreenIn();
    CScriptThing dadTrigger = Game->GetThingWithScriptName("MK_OVI_DADTRIGGER");
    while (!IsDistanceBetweenThingsUnder(Game->GetHero(), dadTrigger, 5.0f)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    HesDeadJimStarted = true;
    {
    CScriptGameResourceObjectScriptedThingBase hero;
    hero.Construct();
    Game->StartScriptingEntity(Game->GetHero(), hero, 4);
    CCutsceneCastMap cast;
    cast.Construct();
    cast["HERO"] = hero;
    CScriptGameResourceObjectMovieBase movie;
    movie.Construct();
    Game->StartMovieSequence("", movie);
    CGameScriptInterface* game = Game;
    game->PauseAllNonScriptedEntities(true);
    Game->FixMovieSequenceCamera(true);
    RunCutsceneMacro("CS_OAKVALEINTRO_HESDEADJIM", cast, 0, 0, false, true);
    Game->FixMovieSequenceCamera(false);
    game->PauseAllNonScriptedEntities(false);
    movie.Destroy();
    cast.Destroy();
    hero.Destroy();
    }
    Game->SetVillageLimbo(Game->GetThingWithScriptName("V_OakVale"), false);
    Game->SetTimeAsStopped(false, TimeStoppedToken);
    Game->DeactivateQuest("Q__OakValeIntro_PostAttack", 0);
    Game->ResetToDefaultTheme(0.0f);
    Game->StopOverrideMusic(false);
#pragma inline_depth()
}