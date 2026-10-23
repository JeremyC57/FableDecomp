// Grade: DIFFER(1360v1356) -- register allocation: retail promotes the constant 0 into ebx for the whole body (xor ebx,ebx; cmp cl,bl; colour bytes from bl) and keeps the timer index in edi; ours gives ebx to the timer index
// CM_QuestControl::Main() -- Haunted House quest control: wait for the hero to enter the house, show the undead-progress
//   bar and keep it updated (decaying the parent's progress every timer period) until it reaches the script-def maximum; then
//   remove the bar, play CS_HAUNTED_CELLAROPENS once (putting the tattoo card into the digging spot), give the renown, enable
//   the house's creature generators, spawn a lake nymph at every M_NymphHere marker, and complete the active quest
// Leaf: ?Main@CM_QuestControl@
// Layout: entity script {vptr, Game +4, Me +8, ParentClass +0x14} as the landed sibling CM_HeroInsideHouse::Main
//   (0x00E96660, same TU); CHauntedHouseScript +0x48 progress, +0x4c bar id, +0x50 cleared, +0x53 hero inside.
//   CTimer as CHS_Screamer::Main 0x00D83FA0; CRGBColour (BGRA) as CQHeroSoulsMotherScript::CreateScreamers 0x00D84630.
#define VPAD1(a) virtual void a() const;
#define VPAD2(a) VPAD1(a##0) VPAD1(a##1)
#define VPAD4(a) VPAD2(a##0) VPAD2(a##1)
#define VPAD8(a) VPAD4(a##0) VPAD4(a##1)
#define VPAD16(a) VPAD8(a##0) VPAD8(a##1)
#define VPAD32(a) VPAD16(a##0) VPAD16(a##1)
#define VPAD64(a) VPAD32(a##0) VPAD32(a##1)
#define VPAD128(a) VPAD64(a##0) VPAD64(a##1)
#define VPAD256(a) VPAD128(a##0) VPAD128(a##1)
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CScriptThingImp {
    virtual void v00() const; virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28)
    VPAD64(i11_0)                                        // slots 11..74
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
};
extern const C3DVector g_ZeroPos;                        // 0x0143E8E0
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    __forceinline CCountedPointer& operator=(const CCountedPointer& other)
    {
        T* data = other.Data;
        CCPPointerInfo* info = other.Info;
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    delete Info;
                }
            }
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
        return *this;
    }
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CBaseClass {
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    CScriptThing() { }
    CScriptThing(const CScriptThing& other);             // 0x004ABE90
    ~CScriptThing() { }                                  // out of line 0x004AA840
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const;
    virtual const C3DVector& GetPos() const;                                   // slot 6 (+0x18)
    __forceinline const C3DVector& PosOrZero() const { return PImp.Data == 0 ? g_ZeroPos : PImp.Data->GetPos(); }
};
struct CBaseObject {                                    // CBaseIntelligentPointer, vtable 0x0129A7D8
    virtual void b000();
    void* m_pIntelligentPointer;                        // +0x04
    CBaseObject();                                      // 0x0099A380
    CBaseObject& operator=(const CBaseObject& other);   // 0x0099A3B0
};
struct CBaseObjectDtor {                                // destructor-only view of CBaseObject
    void* m_pIntelligentPointer;
    virtual ~CBaseObjectDtor();                         // 0x0099A430
};
struct CScriptGameResourceObjectBaseDtor : CBaseObjectDtor {   // vtable 0x0126008C
    __forceinline virtual ~CScriptGameResourceObjectBaseDtor() { }
};
struct CHandlePImp {
    void*           Data;
    CCPPointerInfo* Info;
    CHandlePImp() : Data(0), Info(0) { }
};
struct CScriptGameResourceObjectScriptedThingBase : CBaseObject {   // vtable 0x0127094C, 0x10 bytes
    CHandlePImp PImp;                                   // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
};
struct CMovieImpPointer {
    void* Data;
    void* Info;
    ~CMovieImpPointer();                                // 0x006E7AB0
};
struct CMovieImpWords { void* Data; void* Info; };
struct CScriptGameResourceObjectMovieBase {            // vtable 0x01260EF4, 0x10 bytes; built and destroyed out of line here
    void* _body[4];
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
void __fastcall AddLogbookStoryEntry(long entry);       // 0x00CBE87F
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CRGBColour {                                      // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
struct CScriptThingVector {                             // std::vector<CScriptThing> (STLport)
    CScriptThing* _M_start;
    CScriptThing* _M_finish;
    CScriptThing* _M_end_of_storage;
    CScriptThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CScriptThingVector();                              // 0x008AC970
    unsigned int size() const { return _M_finish - _M_start; }
    CScriptThing& operator[](unsigned int i) { return _M_start[i]; }
};
struct CGameScriptInterface {
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006e7410
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString&) const;   // slot 72 (+0x120) 0x008a7d60
    virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString&, CScriptThingVector&) const;   // slot 75 (+0x12c) 0x008a8570
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c) 0x006e79e0
    virtual void DeregisterTimer(long);   // slot 88 (+0x160) 0x006e7690
    virtual void SetTimer(long, long);   // slot 89 (+0x164) 0x006e76e0
    virtual long GetTimer(long) const;   // slot 90 (+0x168) 0x006e7710
    virtual CScriptThing CreateCreature(const CCharString&, C3DVector const &, const CCharString&, bool) const;   // slot 91 (+0x16c) 0x008a9100
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void GiveHeroRenownPoints(long) const;   // slot 161 (+0x284) 0x008990c0
    virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169();
    virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177();
    virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185();
    virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193();
    virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201();
    virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209();
    virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217();
    virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225();
    virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233();
    virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241();
    virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249();
    virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265();
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280();
    virtual void DeactivateQuestLater(const CCharString&, unsigned long) const;   // slot 281 (+0x464) 0x00892f10
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void SetQuestAsCompleted(const CCharString&, bool, bool, bool) const;   // slot 288 (+0x480) 0x00892f80
    virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296();
    virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312();
    virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void DisplayQuestInfo(bool) const;   // slot 321 (+0x504) 0x00891920
    virtual void v322(); virtual void v323();
    virtual long AddQuestInfoBar(float, float, CRGBColour const &, CRGBColour const &, const CCharString&, const CCharString&, float) const;   // slot 324 (+0x510) 0x00891ac0
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331();
    virtual void UpdateQuestInfoBar(long, float, float, float) const;   // slot 332 (+0x530) 0x00891b80
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337();
    virtual void RemoveQuestInfoElement(long) const;   // slot 338 (+0x548) 0x00891c10
    virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346();
    virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362();
    virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369();
    virtual void StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase &);   // slot 370 (+0x5c8) 0x0089b110
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375();
    virtual void Pause(float) const;   // slot 376 (+0x5e0) 0x0088e4f0
    virtual void v377(); virtual void v378();
    virtual void PauseAllNonScriptedEntities(bool);   // slot 379 (+0x5ec) 0x00890ab0
    virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386(); virtual void v387();
    virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394(); virtual void v395();
    virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403();
    virtual void v404(); virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411();
    virtual void v412(); virtual void v413(); virtual void v414(); virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419();
    virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424(); virtual void v425(); virtual void v426(); virtual void v427();
    virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434(); virtual void v435();
    virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443();
    virtual void v444(); virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451();
    virtual void v452(); virtual void v453(); virtual void v454(); virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459();
    virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463(); virtual void v464(); virtual void v465(); virtual void v466(); virtual void v467();
    virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474(); virtual void v475();
    virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483();
    virtual void v484(); virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491();
    virtual void v492(); virtual void v493(); virtual void v494(); virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499();
    virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504(); virtual void v505(); virtual void v506(); virtual void v507();
    virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514(); virtual void v515();
    virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519(); virtual void v520(); virtual void v521(); virtual void v522(); virtual void v523();
    virtual void v524(); virtual void v525(); virtual void v526(); virtual void v527(); virtual void v528(); virtual void v529(); virtual void v530(); virtual void v531();
    virtual void v532(); virtual void v533(); virtual void v534(); virtual void v535(); virtual void v536(); virtual void v537(); virtual void v538(); virtual void v539();
    virtual void v540(); virtual void v541(); virtual void v542(); virtual void v543(); virtual void v544(); virtual void v545(); virtual void v546(); virtual void v547();
    virtual void v548(); virtual void v549(); virtual void v550(); virtual void v551(); virtual void v552(); virtual void v553(); virtual void v554(); virtual void v555();
    virtual void v556(); virtual void v557();
    virtual void SetCreatureGeneratorsEnabled(const CCharString&, bool) const;   // slot 558 (+0x8b8) 0x00892cf0
    virtual void v559(); virtual void v560(); virtual void v561(); virtual void v562(); virtual void v563(); virtual void v564(); virtual void v565(); virtual void v566();
    virtual void v567(); virtual void v568(); virtual void v569(); virtual void v570(); virtual void v571(); virtual void v572(); virtual void v573(); virtual void v574();
    virtual void v575(); virtual void v576(); virtual void v577(); virtual void v578(); virtual void v579(); virtual void v580(); virtual void v581(); virtual void v582();
    virtual void v583(); virtual void v584();
    virtual void AddItemToContainer(const CScriptThing&, const CCharString&) const;   // slot 585 (+0x924) 0x0089e780
    virtual void v586(); virtual void v587(); virtual void v588(); virtual void v589(); virtual void v590(); virtual void v591(); virtual void v592(); virtual void v593();
    virtual void v594(); virtual void v595(); virtual void v596(); virtual void v597(); virtual void v598(); virtual void v599(); virtual void v600(); virtual void v601();
    virtual void v602(); virtual void v603(); virtual void v604(); virtual void v605(); virtual void v606(); virtual void v607(); virtual void v608(); virtual void v609();
    virtual void v610(); virtual void v611(); virtual void v612(); virtual void v613(); virtual void v614(); virtual void v615(); virtual void v616(); virtual void v617();
    virtual void v618(); virtual void v619(); virtual void v620(); virtual void v621(); virtual void v622(); virtual void v623(); virtual void v624(); virtual void v625();
    virtual void v626(); virtual void v627(); virtual void v628(); virtual void v629(); virtual void v630(); virtual void v631(); virtual void v632(); virtual void v633();
    virtual void v634(); virtual void v635(); virtual void v636(); virtual void v637(); virtual void v638(); virtual void v639(); virtual void v640(); virtual void v641();
    virtual void v642(); virtual void v643(); virtual void v644(); virtual void v645(); virtual void v646(); virtual void v647(); virtual void v648(); virtual void v649();
    virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653(); virtual void v654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c) 0x00891880
};

extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
struct CScriptDef {
    unsigned char _pad[0x66c];
    float         HH_MaxProgress;                       // +0x66C (descriptive)
    float         HH_ProgressDecay;                     // +0x670
    float         HH_DecayInterval;                     // +0x674
    long          HH_RenownPoints;                      // +0x678
};
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
struct CTimer {                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer();                                           // 0x00CD4450
    ~CTimer();                                          // 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
CTimer::CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
struct CHauntedHouseScript {
    unsigned char _pad[0x48];
    float         Progress;                             // +0x48 (descriptive)
    long          ProgressBar;                          // +0x4c
    bool          HouseCleared;                         // +0x50
    unsigned char _pad51[2];
    bool          HeroInside;                           // +0x53
};
struct CM_QuestControl {
    void*                 __vftable;                    // +0x00
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    CHauntedHouseScript*  ParentClass;                  // +0x14
    void*                 PMasterData;                  // +0x18
    bool IsActiveThreadTerminating();                   // 0x00F35B30
    void Main();
};

void CM_QuestControl::Main()
{
    while (!ParentClass->HeroInside) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CTimer timer;
    timer.SetTime(1);
    Game->DisplayQuestInfo(true);
    ParentClass->ProgressBar = Game->AddQuestInfoBar(g_pScriptDef->HH_MaxProgress, 0.0f, CRGBColour(0, 255, 0, 255),
        CRGBColour(255, 0, 0, 255), "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
    while (ParentClass->Progress < g_pScriptDef->HH_MaxProgress) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->Progress < g_pScriptDef->HH_ProgressDecay) {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->Progress = 0.0f;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                ParentClass->Progress -= g_pScriptDef->HH_ProgressDecay;
            }
            timer.SetTime((long)g_pScriptDef->HH_DecayInterval);
        }
        Game->UpdateQuestInfoBar(ParentClass->ProgressBar, ParentClass->Progress, -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(ParentClass->ProgressBar);
    if (!ParentClass->HouseCleared) {
        if (IsActiveThreadTerminating())
            return;
        CCutsceneCastMap cast;
        CScriptGameResourceObjectMovieBase movie;
        Game->StartMovieSequence("", movie);
        CGameScriptInterface* game = Game;
        game->PauseAllNonScriptedEntities(true);
        ParentClass->HouseCleared = true;
        RunCutsceneMacro("CS_HAUNTED_CELLAROPENS", cast, 0, 0, false, true);
        {
        CScriptThing spot = Game->GetThingWithScriptName("HH_DiggingSpot");
        Game->AddItemToContainer(spot, "OBJECT_TATTOO_CARD_BACK_02");
        }
        game->PauseAllNonScriptedEntities(false);
    }
    Game->GiveHeroRenownPoints(g_pScriptDef->HH_RenownPoints);
    Game->SetCreatureGeneratorsEnabled("HauntedHouse", true);
    CScriptThingVector nymphs;
    Game->GetAllThingsWithScriptName("M_NymphHere", nymphs);
    for (unsigned int i = 0; i < nymphs.size(); i++) {
        if (IsActiveThreadTerminating())
            return;
        Game->CreateCreature("CREATURE_LAKE_NYMPH_01", nymphs[i].GetPos(), "", false);
        Game->Pause(1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetQuestAsCompleted(Game->GetActiveQuestName(), false, true, false);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
