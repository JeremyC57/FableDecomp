// Grade: DIFFER(986v986) -- one scheduling difference: the fstp of the inline GetAngleXY() result into the outgoing float argument
//   of EntityTeleportToPosition lands before the inline GetPos() null test; retail places it after `mov ecx,[esp+0x20]; cmp ecx,edi`.
//   Tried a named angle local, ternary and if-forms of GetAngleXY (both polarities), the reversed GetPos form, and the QFE 4035
//   compiler; none moves the store (reversed forms grow to 990). Label: retail label says CQ_SecretPassageScript::PrisonEscape,
//   but it is spawned from CQ_PrisonEscapeRescueMotherScript::Main 0x00DD2CE0 (CSpawnedFunc vtable 0x012DB954).
// CQ_PrisonEscapeRescueMotherScript::PrisonEscape() -- once the mother follows and the graveyard is loaded: teleports the hero to the splitting-up outro marker, plays the prison video and the CS_MOTHER_SPLITTING_UP cut-scene, then completes and retires the prison-escape quests
// Leaf: PrisonEscape
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
extern C3DVector g_ZeroVector;                          // 0x0143E8E0

struct CScriptThingImp {                                // the implementation object's vtable view
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009();
    virtual float GetAngleXY() const;                   // slot 10 (+0x28)
    virtual void i011();
    virtual void i012();
    virtual void i013();
    virtual void i014();
    virtual void i015();
    virtual void i016();
    virtual void i017();
    virtual void i018();
    virtual void i019();
    virtual void i020();
    virtual void i021();
    virtual void i022();
    virtual void i023();
    virtual void i024();
    virtual void i025();
    virtual void i026();
    virtual void i027();
    virtual void i028();
    virtual void i029();
    virtual void i030();
    virtual void i031();
    virtual void i032();
    virtual void i033();
    virtual void i034();
    virtual void i035();
    virtual void i036();
    virtual void i037();
    virtual void i038();
    virtual void i039();
    virtual void i040();
    virtual void i041();
    virtual void i042();
    virtual void i043();
    virtual void i044();
    virtual void i045();
    virtual void i046();
    virtual void i047();
    virtual void i048();
    virtual void i049();
    virtual void i050();
    virtual void i051();
    virtual void i052();
    virtual void i053();
    virtual void i054();
    virtual void i055();
    virtual void i056();
    virtual void i057();
    virtual void i058();
    virtual void i059();
    virtual void i060();
    virtual void i061();
    virtual void i062();
    virtual void i063();
    virtual void i064();
    virtual void i065();
    virtual void i066();
    virtual void i067();
    virtual void i068();
    virtual void i069();
    virtual void i070();
    virtual void i071();
    virtual void i072();
    virtual void i073();
    virtual void i074();
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};

struct CBaseClass {
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};

struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CScriptThingImp* PImpData;                          // +0x04 CCountedPointer Data
    void*            PImpInfo;                          // +0x08 CCountedPointer Info
    virtual ~CScriptThing();                            // 0x004AA840
    const C3DVector& GetPos() const                     // 0x004AA980
    {
        if (!PImpData)
            return g_ZeroVector;
        return PImpData->GetPos();
    }
    float GetAngleXY() const                            // 0x004AAA40
    {
        if (!PImpData)
            return 0.0f;
        return PImpData->GetAngleXY();
    }
    bool IsAlive() const                                // 0x004AB130
    {
        if (!PImpData)
            return false;
        return PImpData->IsAlive();
    }
};

struct CBaseObject {                                    // vtable 0x0129A7D8
    CBaseObject();                                      // 0x0099A380
    virtual ~CBaseObject();                             // 0x0099A430
    void* IntelligentPointer;                           // +0x04
};

struct CHandlePImp { void* Data; void* Info; CHandlePImp() : Data(0), Info(0) {} };

struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // 16 bytes, vtable 0x0127094C
    CHandlePImp PImp;                                   // +0x08
    CScriptGameResourceObjectScriptedThingBase() {}
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    void Release();                                     // 0x00CD2770
};

struct CScriptGameResourceObjectMovieBase : CBaseObject {   // 16 bytes, vtable 0x01260EF4
    CHandlePImp PImp;                                   // +0x08
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};

struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase> (12 bytes)
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D

struct CScriptDef {
    unsigned char _pad000[0xde4];
    long          PrisonEscapeExperienceFromMother;     // +0xde4 (Ego_r PDB +0xE94; the -0xB0 shift)
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C

enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };

struct CGameScriptInterface {                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009(); virtual void v010(); virtual void v011();
    virtual bool IsRegionLoaded(const CCharString& region) const;   // slot 12 (+0x30)
    virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020();
    virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028();
    virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036();
    virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052();
    virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060();
    virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068();
    virtual void v069();
    virtual CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104();
    virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116();
    virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127();
    virtual void GiveHeroExperience(long experience) const;   // slot 128 (+0x200)
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void SetGuardsIgnoreCrimes(bool ignore) const;   // slot 134 (+0x218)
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void ResetPlayerCreatureCombatMultiplier() const;   // slot 151 (+0x25c)
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
    virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280();
    virtual void DeactivateQuestLater(const CCharString& quest, unsigned long delay) const;   // slot 281 (+0x464)
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void SetQuestAsCompleted(const CCharString& quest, bool a, bool b, bool c) const;   // slot 288 (+0x480)
    virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296();
    virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
    virtual void v313(); virtual void v314();
    virtual void ResetDeathRecoveryMarkerNameToDefault() const;   // slot 315 (+0x4ec)
    virtual void v316(); virtual void v317();
    virtual void ResetWhetherToFailQuestOnDeathToDefault() const;   // slot 318 (+0x4f8)
    virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326();
    virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342();
    virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350();
    virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358();
    virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366();
    virtual void v367(); virtual void v368();
    virtual void PlayAVIMovie(const CCharString& movie) const;   // slot 369 (+0x5c4)
    virtual void StartMovieSequence(const CCharString& name, CScriptGameResourceObjectMovieBase& movie);   // slot 370 (+0x5c8)
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386();
    virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394();
    virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402();
    virtual void v403(); virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410();
    virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418();
    virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426();
    virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434();
    virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442();
    virtual void v443(); virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450();
    virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458();
    virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466();
    virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472();
    virtual void EntityTeleportToPosition(const CScriptThing& thing, const C3DVector& pos, float angle, bool a, bool b) const;   // slot 473 (+0x764)
    virtual void v474(); virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481();
    virtual void v482(); virtual void v483(); virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489();
    virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497();
    virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505();
    virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511(); virtual void v512(); virtual void v513();
    virtual void v514(); virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521();
    virtual void v522(); virtual void v523(); virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529();
    virtual void v530(); virtual void v531(); virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535(); virtual void v536(); virtual void v537();
    virtual void v538(); virtual void v539(); virtual void v540(); virtual void v541(); virtual void v542(); virtual void v543(); virtual void v544(); virtual void v545();
    virtual void v546(); virtual void v547(); virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552(); virtual void v553();
    virtual void v554(); virtual void v555(); virtual void v556(); virtual void v557(); virtual void v558(); virtual void v559(); virtual void v560(); virtual void v561();
    virtual void v562(); virtual void v563(); virtual void v564();
    virtual void SetTeleportingAsActive(bool active) const;   // slot 565 (+0x8d4)
    virtual void v566(); virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573();
    virtual void v574(); virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581();
    virtual void v582(); virtual void v583(); virtual void v584(); virtual void v585(); virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589();
    virtual void v590(); virtual void v591(); virtual void v592(); virtual void v593(); virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597();
    virtual void v598(); virtual void v599(); virtual void v600(); virtual void v601(); virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605();
    virtual void v606(); virtual void v607(); virtual void v608(); virtual void v609(); virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613();
    virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617(); virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621();
    virtual void v622(); virtual void v623(); virtual void v624(); virtual void v625(); virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629();
    virtual void v630(); virtual void v631(); virtual void v632(); virtual void v633(); virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637();
    virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641(); virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645();
    virtual void v646(); virtual void v647(); virtual void v648(); virtual void v649(); virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653();
    virtual void v654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_PrisonEscapeRescueMotherScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    long                  CurrentTask;                  // +0x48 (retail = PDB - 0x14)
    long                  TasksDone[3];                 // +0x4c
    long                  TargetNumber[3];              // +0x58
    bool                  HeroAgreesToTask;             // +0x64
    bool                  HeroHasFoundWeapons;          // +0x65
    bool                  MotherCellDoorOpen;           // +0x66
    bool                  MotherFollowing;              // +0x67
    void PrisonEscape();
};

void CQ_PrisonEscapeRescueMotherScript::PrisonEscape()
{
    while (!MotherFollowing) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!GSI->IsRegionLoaded("Graveyard")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing outro = GSI->GetThingWithScriptName("OUTRO_SPLITTING_UP");
    GSI->EntityTeleportToPosition(GSI->GetHero(), outro.GetPos(), outro.GetAngleXY(), true, false);
    GSI->PlayAVIMovie("Data\\Video\\8_prison_2_comp.xmv");
    CScriptGameResourceObjectScriptedThingBase hero;
    GSI->StartScriptingEntity(GSI->GetHero(), hero, (EScriptAIPriority)4);
    CScriptGameResourceObjectScriptedThingBase unusedHandle;
    CScriptThing mother = GSI->GetThingWithScriptName("MotherTorture");
    if (mother.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        GSI->RemoveThing(mother, false, true);
    }
    CScriptGameResourceObjectMovieBase movie;
    GSI->StartMovieSequence("", movie);
    CCutsceneCastMap cast;
    cast["Hero"] = hero;
    RunCutsceneMacro("CS_MOTHER_SPLITTING_UP", cast, 0, 0, false, true);
    hero.Release();
    GSI->ResetPlayerCreatureCombatMultiplier();
    GSI->GiveHeroExperience(g_pScriptDef->PrisonEscapeExperienceFromMother);
    GSI->SetQuestAsCompleted(GSI->GetActiveQuestName(), true, false, false);
    GSI->SetGuardsIgnoreCrimes(false);
    GSI->SetTeleportingAsActive(true);
    GSI->ResetWhetherToFailQuestOnDeathToDefault();
    GSI->ResetDeathRecoveryMarkerNameToDefault();
    GSI->DeactivateQuestLater(GSI->GetActiveQuestName(), 0);
    GSI->DeactivateQuestLater("Q_PrisonEscapeFromCell", 0);
}
}
