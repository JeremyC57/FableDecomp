// CGuardOutsideWoods::Main() -- a guard outside the Guildmaster's woods: take scripted control, mark Me on the minimap, then every frame play the warning cutscene once when the hero first comes within 35, give three escalating warnings (within 30, 18, 10 units: "close", "closer" + HALT, "too close" -> attack), and attack the hero when hit by him or when the outside-wood guards already attack
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D874F0, 1418 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, RegisterFilter 0x00F35B10, IsActiveThreadTerminating 0x00F35B30,
//   CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsScripting 0x00CD23B9 / Release 0x00CD2770 / PlayAnimation
//   0x007E73D0 / dtor 0x007E74D0, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CQ_HeroSoulsGuildmasterScript::
//   PlayCutscene(CCharString) 0x00D872D0 (label "Script_InitializeComponents" is wrong; name by analogy with the landed
//   CQ_AwakeningTheOracleScript::PlayCutscene 0x00CFF9A0), CCharString 0x0099EBF0 / 0x0099EAE0; GameCalls slots 7, 8, 70, 350,
//   352, 364-366, 451; CScriptThing slots 21/41/42 (hit-by messages). Global bool 0x01375748 (animation default).
// Layout: Ego_r.pdb CGuardOutsideWoods (32 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CQ_HeroSoulsGuildmasterScript, retail = PDB - 0x14: OutsideWoodGuardsAttack +0x4b, GiveWarningCutscene +0x4c),
//   PMasterData +0x18, NumberOfWarnings +0x1c. Models from the landed CMuteGuardOutsideWoods::Main 0x00D86150.
enum EHeroAbility { HERO_ABILITY_NONE };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CCountedPointer()
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
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
struct C3DVector { float X, Y, Z; };
struct CScriptThing : CBase {                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const; virtual void s003() const; virtual void s004() const; virtual void s005() const; virtual void s006() const; virtual void s007() const; virtual void s008() const;
    virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const; virtual void s015() const; virtual void s016() const;
    virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const;
    virtual bool MsgIsHitBy(const CCharString& name) const;   // slot 21 (+0x54)
    virtual void s022() const; virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const;
    virtual void s030() const; virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const;
    virtual void s038() const; virtual void s039() const; virtual void s040() const;
    virtual bool MsgIsHitBySpecialAbilityFrom(EHeroAbility ability, const CCharString& name) const;   // slot 41 (+0xa4)
    virtual bool MsgIsHitByAnySpecialAbilityFrom(const CCharString& name) const;   // slot 42 (+0xa8)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    ~CScriptThing();                                    // 0x004AA840
};
struct CBaseIntelligentPointer {                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
    CBaseIntelligentPointer();                          // 0x0099A380
    virtual ~CBaseIntelligentPointer();                 // 0x0099A430
};
struct CScriptGameResourceObjectBase : CBaseIntelligentPointer {   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
struct CScriptThingCopy : CBase {                       // CScriptThing as the filter holds it: out-of-line copy and destructor
    virtual void s001();
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThingCopy(const CScriptThing& other);        // 0x004ABE90
    ~CScriptThingCopy();                                // 0x004AA840
};
struct CIsThingAlive {                                  // vtable 0x012C32F0
    virtual void v00();
    CScriptThingCopy m_thing;                           // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase {   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void PlayAnimation(const CCharString& anim, bool a, bool b, bool c, bool d, bool e, bool f, bool g);   // 0x007E73D0
};
struct CGameScriptInterface {                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual bool StartScriptingEntity(const CScriptThing& thing, CScriptGameResourceObjectScriptedThingBase& object, EScriptAIPriority priority);   // slot 8 (+0x20)
    virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const;
    virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const;
    virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const; virtual void v032() const;
    virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const; virtual void v040() const;
    virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const;
    virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const;
    virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const;
    virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
    virtual void v071() const; virtual void v072() const; virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const;
    virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const;
    virtual void v087() const; virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const;
    virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const; virtual void v099() const; virtual void v100() const; virtual void v101() const; virtual void v102() const;
    virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const;
    virtual void v111() const; virtual void v112() const; virtual void v113() const; virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const;
    virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const;
    virtual void v127() const; virtual void v128() const; virtual void v129() const; virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const;
    virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const; virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const;
    virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const; virtual void v147() const; virtual void v148() const; virtual void v149() const; virtual void v150() const;
    virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const;
    virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const;
    virtual void v167() const; virtual void v168() const; virtual void v169() const; virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const;
    virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const; virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const;
    virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const; virtual void v187() const; virtual void v188() const; virtual void v189() const; virtual void v190() const;
    virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const;
    virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const;
    virtual void v207() const; virtual void v208() const; virtual void v209() const; virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const;
    virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const; virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const;
    virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const; virtual void v227() const; virtual void v228() const; virtual void v229() const; virtual void v230() const;
    virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const;
    virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const;
    virtual void v247() const; virtual void v248() const; virtual void v249() const; virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const;
    virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const; virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const;
    virtual void v263() const; virtual void v264() const; virtual void v265() const; virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const;
    virtual void v271() const; virtual void v272() const; virtual void v273() const; virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const;
    virtual void v279() const; virtual void v280() const; virtual void v281() const; virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const;
    virtual void v287() const; virtual void v288() const; virtual void v289() const; virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const;
    virtual void v295() const; virtual void v296() const; virtual void v297() const; virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const;
    virtual void v303() const; virtual void v304() const; virtual void v305() const; virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const;
    virtual void v311() const; virtual void v312() const; virtual void v313() const; virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const;
    virtual void v319() const; virtual void v320() const; virtual void v321() const; virtual void v322() const; virtual void v323() const; virtual void v324() const; virtual void v325() const; virtual void v326() const;
    virtual void v327() const; virtual void v328() const; virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const;
    virtual void v335() const; virtual void v336() const; virtual void v337() const; virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const;
    virtual void v343() const; virtual void v344() const; virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const;
    virtual void MiniMapAddMarker(const CScriptThing& a0, const CCharString& a1) const;   // slot 350 (+0x578) 0x00891F50
    virtual void v351() const;
    virtual void MiniMapRemoveMarker(const CScriptThing& a0) const;   // slot 352 (+0x580) 0x00891FF0
    virtual void v353() const; virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const;
    virtual void v361() const; virtual void v362() const; virtual void v363() const;
    virtual long AddNewConversation(const CScriptThing& a0, bool a1, bool a2) const;   // slot 364 (+0x5b0) 0x008906C0
    virtual void AddPersonToConversation(long a0, const CScriptThing& a1) const;   // slot 365 (+0x5b4) 0x00890710
    virtual void AddLineToConversation(long a0, const CCharString& a1, bool a2, const CScriptThing& a3, const CScriptThing& a4) const;   // slot 366 (+0x5b8) 0x00890750
    virtual void v367() const; virtual void v368() const; virtual void v369() const; virtual void v370() const; virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const;
    virtual void v375() const; virtual void v376() const; virtual void v377() const; virtual void v378() const; virtual void v379() const; virtual void v380() const; virtual void v381() const; virtual void v382() const;
    virtual void v383() const; virtual void v384() const; virtual void v385() const; virtual void v386() const; virtual void v387() const; virtual void v388() const; virtual void v389() const; virtual void v390() const;
    virtual void v391() const; virtual void v392() const; virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const; virtual void v397() const; virtual void v398() const;
    virtual void v399() const; virtual void v400() const; virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const; virtual void v405() const; virtual void v406() const;
    virtual void v407() const; virtual void v408() const; virtual void v409() const; virtual void v410() const; virtual void v411() const; virtual void v412() const; virtual void v413() const; virtual void v414() const;
    virtual void v415() const; virtual void v416() const; virtual void v417() const; virtual void v418() const; virtual void v419() const; virtual void v420() const; virtual void v421() const; virtual void v422() const;
    virtual void v423() const; virtual void v424() const; virtual void v425() const; virtual void v426() const; virtual void v427() const; virtual void v428() const; virtual void v429() const; virtual void v430() const;
    virtual void v431() const; virtual void v432() const; virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const; virtual void v437() const; virtual void v438() const;
    virtual void v439() const; virtual void v440() const; virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const; virtual void v445() const; virtual void v446() const;
    virtual void v447() const; virtual void v448() const; virtual void v449() const; virtual void v450() const;
    virtual void GiveThingBestEnemyTarget(const CScriptThing& a0, const CScriptThing& a1) const;   // slot 451 (+0x70c) 0x008A0260
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
extern bool g_bScriptAnimationFlag;                     // 0x01375748
struct CQ_HeroSoulsGuildmasterScript {
    unsigned char _pad[0x4b];
    bool OutsideWoodGuardsAttack;                       // +0x4b (PDB +0x5f)
    bool GiveWarningCutscene;                           // +0x4c (PDB +0x60)
    void PlayCutscene(CCharString name);                // 0x00D872D0
};
struct CEntityScriptBase {
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    void RegisterFilter(CIsThingAlive& filter);         // 0x00F35B10
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
};
struct CGuardOutsideWoods : CEntityScriptBase {
    CQ_HeroSoulsGuildmasterScript* ParentClass;         // +0x14
    void* PMasterData;                                  // +0x18
    long NumberOfWarnings;                              // +0x1c
    void Main();
};

void CGuardOutsideWoods::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase guard;
    if (guard.IsScripting())
        guard.Release();
    while (!Game->StartScriptingEntity(Me, guard, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->MiniMapAddMarker(Me, "HUD_ORB_QUEST_CORE");
    while (!IsActiveThreadTerminating()) {
        if (ParentClass->GiveWarningCutscene == true && IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 35.0f)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->GiveWarningCutscene = false;
            ParentClass->PlayCutscene("CS_WARNEDBYGUARDS");
        }
        switch (NumberOfWarnings) {
        case 0:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 30.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_CLOSE_10", false, Me, Game->GetHero());
                ++NumberOfWarnings;
                guard.PlayAnimation("HALT", false, false, false, false, g_bScriptAnimationFlag, false, false);
            }
            break;
        case 1:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 18.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_CLOSER_10", false, Me, Game->GetHero());
                ++NumberOfWarnings;
                guard.PlayAnimation("HALT", false, false, false, false, g_bScriptAnimationFlag, false, false);
            }
            break;
        case 2:
            if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 10.0f)) {
                if (IsActiveThreadTerminating())
                    return;
                long conversation = Game->AddNewConversation(Me, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, "TEXT_QST_B04_GUILDWOODSGUARD_TOO_CLOSE_10", false, Me, Game->GetHero());
                ParentClass->OutsideWoodGuardsAttack = true;
                Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
                Game->MiniMapRemoveMarker(Me);
                if (guard.IsScripting())
                    guard.Release();
                do {
                    Game->NewScriptFrame();
                } while (!IsActiveThreadTerminating());
                return;
            }
            break;
        }
        if (Me.MsgIsHitBy("SCRIPT_NAME_HERO")
            || (Me.MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO") && !Me.MsgIsHitBySpecialAbilityFrom((EHeroAbility)14, "SCRIPT_NAME_HERO"))
            || ParentClass->OutsideWoodGuardsAttack) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->OutsideWoodGuardsAttack = true;
            Game->GiveThingBestEnemyTarget(Me, Game->GetHero());
            Game->MiniMapRemoveMarker(Me);
            if (guard.IsScripting())
                guard.Release();
            do {
                Game->NewScriptFrame();
            } while (!IsActiveThreadTerminating());
            return;
        }
        Game->NewScriptFrame();
    }
}