// CHTG_Prisoner::Main() -- the bound prisoner on the Hanging Tree (good) quest: take scripted control, bind his hands and make him unpushable, wait for the party to be set up, then until the quest state reaches 7: in state 2 follow the bandit leader; in state 5 find the nearest REGION_EXIT_POINT and, when it is a real route, try to escape towards it (the leader shouts for the guards when he gets 12 units away, he dies and fails the mission on reaching the exit, and goes back to following the leader when the hero catches him); in any other state stand still; finally release him
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D71B30, 1453 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840 / operator= 0x008AB980, CBase ctor 0x0099A2D0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CBaseIntelligentPointer ctor 0x0099A380, scripted-thing handle IsScripting 0x00CD23B9 /
//   Release 0x00CD2770 / MoveToPosition 0x007E72F0 / FollowThing 0x007E7320 / ClearCommands 0x007E7360 / IsPerformingScriptTask
//   0x007E7450 / dtor 0x007E74D0, IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA / FadeOutAndKillEntity 0x00CBE716
//   (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0 / operator!= 0x0099E960 / operator= 0x0099EFE0; GameCalls slots 7, 8, 70,
//   81 GetNearestWithDefName, 364-366 conversations, 376 Pause, 494, 745. Globals: empty string 0x0143E8EC, zero vector 0x0143E8E0.
// Layout: Ego_r.pdb CHTG_Prisoner: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CQ_HangingTreeGoodScript, the usual
//   0x14 quest-script shift: PartySuccessfullySetUp +0x49, MissionFailed +0x4b, BanditsFlee +0x57, PrisonerEscaping +0x58,
//   HTG_State +0x5c, Leader +0x68, ReasonForFailing +0x9c), PMasterData +0x18, ChopBlockMarker +0x1c, ReturningToParty +0x28,
//   GuardsCalledAfterMe +0x29.
namespace NScript { class CPredicateBase; }
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
    CCharString& operator=(const char* text);          // 0x0099EFE0
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
class C3DVector { public: float X, Y, Z; };
extern const CCharString g_EmptyString;                // 0x0143E8EC
extern const C3DVector g_ZeroVector;                    // 0x0143E8E0
struct CScriptThingImpl {                              // the game-side thing behind CScriptThing::PImp
    virtual void i000() const;
    virtual const CCharString& GetDataString() const;   // slot 1 (+0x4)
    virtual void i002() const; virtual void i003() const; virtual void i004() const; virtual void i005() const;
    virtual const C3DVector& GetPos() const;   // slot 6 (+0x18)
    virtual void i007() const; virtual void i008() const; virtual void i009() const; virtual void i010() const; virtual void i011() const; virtual void i012() const; virtual void i013() const; virtual void i014() const;
    virtual void i015() const; virtual void i016() const; virtual void i017() const; virtual void i018() const; virtual void i019() const; virtual void i020() const; virtual void i021() const; virtual void i022() const;
    virtual void i023() const; virtual void i024() const; virtual void i025() const; virtual void i026() const; virtual void i027() const; virtual void i028() const; virtual void i029() const; virtual void i030() const;
    virtual void i031() const; virtual void i032() const; virtual void i033() const; virtual void i034() const; virtual void i035() const; virtual void i036() const; virtual void i037() const; virtual void i038() const;
    virtual void i039() const; virtual void i040() const; virtual void i041() const; virtual void i042() const; virtual void i043() const; virtual void i044() const; virtual void i045() const; virtual void i046() const;
    virtual void i047() const; virtual void i048() const; virtual void i049() const; virtual void i050() const; virtual void i051() const; virtual void i052() const; virtual void i053() const; virtual void i054() const;
    virtual void i055() const; virtual void i056() const; virtual void i057() const; virtual void i058() const; virtual void i059() const; virtual void i060() const; virtual void i061() const; virtual void i062() const;
    virtual void i063() const; virtual void i064() const; virtual void i065() const; virtual void i066() const; virtual void i067() const; virtual void i068() const; virtual void i069() const; virtual void i070() const;
    virtual void i071() const; virtual void i072() const; virtual void i073() const; virtual void i074() const;
    virtual bool IsAlive() const;   // slot 75 (+0x12c)
};
namespace N_0099a2d0 { struct CBase {                                          // vtable 0x01231710
    CBase();                                            // 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
}; } using N_0099a2d0::CBase;

class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
    virtual void s001() const; virtual void s002() const;
    virtual const CCharString& GetDataString() const { return !PImp.Data ? g_EmptyString : PImp.Data->GetDataString(); }   // slot 3 (+0xc)
    virtual void s004() const; virtual void s005() const;
    virtual const C3DVector& GetPos() const { return !PImp.Data ? g_ZeroVector : PImp.Data->GetPos(); }   // slot 6 (+0x18)
    virtual void s007() const; virtual void s008() const; virtual void s009() const; virtual void s010() const; virtual void s011() const; virtual void s012() const; virtual void s013() const; virtual void s014() const;
    virtual void s015() const; virtual void s016() const; virtual void s017() const; virtual void s018() const; virtual void s019() const; virtual void s020() const; virtual void s021() const; virtual void s022() const;
    virtual void s023() const; virtual void s024() const; virtual void s025() const; virtual void s026() const; virtual void s027() const; virtual void s028() const; virtual void s029() const; virtual void s030() const;
    virtual void s031() const; virtual void s032() const; virtual void s033() const; virtual void s034() const; virtual void s035() const; virtual void s036() const; virtual void s037() const; virtual void s038() const;
    virtual void s039() const; virtual void s040() const; virtual void s041() const; virtual void s042() const; virtual void s043() const; virtual void s044() const; virtual void s045() const; virtual void s046() const;
    virtual void s047() const; virtual void s048() const; virtual void s049() const; virtual void s050() const; virtual void s051() const; virtual void s052() const; virtual void s053() const; virtual void s054() const;
    virtual void s055() const; virtual void s056() const; virtual void s057() const; virtual void s058() const; virtual void s059() const; virtual void s060() const; virtual void s061() const; virtual void s062() const;
    virtual void s063() const; virtual void s064() const; virtual void s065() const; virtual void s066() const; virtual void s067() const; virtual void s068() const; virtual void s069() const; virtual void s070() const;
    virtual void s071() const; virtual void s072() const; virtual void s073() const; virtual void s074() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c)
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing(const CScriptThing& other);            // 0x004ABE90
    struct InlineCopy { };                              // tag: the inline copy retail expands at by-value call sites
    CScriptThing(const CScriptThing& other, InlineCopy) : CBase(), PImp(other.PImp) { }
    CScriptThing() { }
    ~CScriptThing();                                    // 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);   // 0x008AB980
};
class CBaseObject { public:                        // vtable 0x0129A7D8
    void* m_pIntelligentPointer;                        // +0x04
protected:
    CBaseObject();                                       // 0x0099A380
    virtual ~CBaseObject();                              // 0x0099A430
};
class CScriptGameResourceObjectBase : public CBaseObject { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                  // vtable 0x012C32F0
    CScriptThing m_thing;                               // +0x04 (copied and destroyed out of line)
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive() { }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:   // vtable 0x0127094C, 0x10 bytes
    CCountedPointer<void> PImp;                         // +0x08 Data, +0x0c Info
    CScriptGameResourceObjectScriptedThingBase() { }
    ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    bool IsScripting();                                 // 0x00CD23B9
    void Release();                                     // 0x00CD2770
    void MoveToPosition(const C3DVector& pos, float speed, long moveType, bool a, bool b);   // 0x007E72F0
    void FollowThing(const CScriptThing& thing, float distance, bool run);   // 0x007E7320
    void ClearCommands();                               // 0x007E7360
    bool IsPerformingScriptTask() const;                // 0x007E7450
};
class CGameScriptInterface { public:                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void v079() const; virtual void v080() const;
    virtual CScriptThing GetNearestWithDefName(const CScriptThing& a0, const CCharString& a1) const;   // slot 81 (+0x144) 0x0089A880
    virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const;
    virtual void v090() const; virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const;
    virtual void v098() const; virtual void v099() const; virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const;
    virtual void v106() const; virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const;
    virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const; virtual void v119() const; virtual void v120() const; virtual void v121() const;
    virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const; virtual void v129() const;
    virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const;
    virtual void v138() const; virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const;
    virtual void v146() const; virtual void v147() const; virtual void v148() const; virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const;
    virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const; virtual void v159() const; virtual void v160() const; virtual void v161() const;
    virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const; virtual void v169() const;
    virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const;
    virtual void v178() const; virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const;
    virtual void v186() const; virtual void v187() const; virtual void v188() const; virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const;
    virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const; virtual void v199() const; virtual void v200() const; virtual void v201() const;
    virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const; virtual void v209() const;
    virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const;
    virtual void v218() const; virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const;
    virtual void v226() const; virtual void v227() const; virtual void v228() const; virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const;
    virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const; virtual void v239() const; virtual void v240() const; virtual void v241() const;
    virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const; virtual void v249() const;
    virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const;
    virtual void v258() const; virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const; virtual void v264() const; virtual void v265() const;
    virtual void v266() const; virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const;
    virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const; virtual void v281() const;
    virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const; virtual void v289() const;
    virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const; virtual void v297() const;
    virtual void v298() const; virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const; virtual void v305() const;
    virtual void v306() const; virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const;
    virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const; virtual void v321() const;
    virtual void v322() const; virtual void v323() const; virtual void v324() const; virtual void v325() const; virtual void v326() const; virtual void v327() const; virtual void v328() const; virtual void v329() const;
    virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const; virtual void v335() const; virtual void v336() const; virtual void v337() const;
    virtual void v338() const; virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const; virtual void v345() const;
    virtual void v346() const; virtual void v347() const; virtual void v348() const; virtual void v349() const; virtual void v350() const; virtual void v351() const; virtual void v352() const; virtual void v353() const;
    virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const; virtual void v361() const;
    virtual void v362() const; virtual void v363() const;
    virtual long AddNewConversation(const CScriptThing& a0, bool a1, bool a2) const;   // slot 364 (+0x5b0) 0x008906C0
    virtual void AddPersonToConversation(long a0, const CScriptThing& a1) const;   // slot 365 (+0x5b4) 0x00890710
    virtual void AddLineToConversation(long a0, const CCharString& a1, bool a2, const CScriptThing& a3, const CScriptThing& a4) const;   // slot 366 (+0x5b8) 0x00890750
    virtual void v367() const; virtual void v368() const; virtual void v369() const; virtual void v370() const; virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const;
    virtual void v375() const;
    virtual void Pause(float a0) const;   // slot 376 (+0x5e0) 0x0088E4F0
    virtual void v377() const; virtual void v378() const; virtual void v379() const; virtual void v380() const; virtual void v381() const; virtual void v382() const; virtual void v383() const; virtual void v384() const;
    virtual void v385() const; virtual void v386() const; virtual void v387() const; virtual void v388() const; virtual void v389() const; virtual void v390() const; virtual void v391() const; virtual void v392() const;
    virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const; virtual void v397() const; virtual void v398() const; virtual void v399() const; virtual void v400() const;
    virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const; virtual void v405() const; virtual void v406() const; virtual void v407() const; virtual void v408() const;
    virtual void v409() const; virtual void v410() const; virtual void v411() const; virtual void v412() const; virtual void v413() const; virtual void v414() const; virtual void v415() const; virtual void v416() const;
    virtual void v417() const; virtual void v418() const; virtual void v419() const; virtual void v420() const; virtual void v421() const; virtual void v422() const; virtual void v423() const; virtual void v424() const;
    virtual void v425() const; virtual void v426() const; virtual void v427() const; virtual void v428() const; virtual void v429() const; virtual void v430() const; virtual void v431() const; virtual void v432() const;
    virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const; virtual void v437() const; virtual void v438() const; virtual void v439() const; virtual void v440() const;
    virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const; virtual void v445() const; virtual void v446() const; virtual void v447() const; virtual void v448() const;
    virtual void v449() const; virtual void v450() const; virtual void v451() const; virtual void v452() const; virtual void v453() const; virtual void v454() const; virtual void v455() const; virtual void v456() const;
    virtual void v457() const; virtual void v458() const; virtual void v459() const; virtual void v460() const; virtual void v461() const; virtual void v462() const; virtual void v463() const; virtual void v464() const;
    virtual void v465() const; virtual void v466() const; virtual void v467() const; virtual void v468() const; virtual void v469() const; virtual void v470() const; virtual void v471() const; virtual void v472() const;
    virtual void v473() const; virtual void v474() const; virtual void v475() const; virtual void v476() const; virtual void v477() const; virtual void v478() const; virtual void v479() const; virtual void v480() const;
    virtual void v481() const; virtual void v482() const; virtual void v483() const; virtual void v484() const; virtual void v485() const; virtual void v486() const; virtual void v487() const; virtual void v488() const;
    virtual void v489() const; virtual void v490() const; virtual void v491() const; virtual void v492() const; virtual void v493() const;
    virtual void EntitySetAsHavingBoundHands(CScriptThing& a0, bool a1) const;   // slot 494 (+0x7b8) 0x0089D420
    virtual void v495() const; virtual void v496() const; virtual void v497() const; virtual void v498() const; virtual void v499() const; virtual void v500() const; virtual void v501() const; virtual void v502() const;
    virtual void v503() const; virtual void v504() const; virtual void v505() const; virtual void v506() const; virtual void v507() const; virtual void v508() const; virtual void v509() const; virtual void v510() const;
    virtual void v511() const; virtual void v512() const; virtual void v513() const; virtual void v514() const; virtual void v515() const; virtual void v516() const; virtual void v517() const; virtual void v518() const;
    virtual void v519() const; virtual void v520() const; virtual void v521() const; virtual void v522() const; virtual void v523() const; virtual void v524() const; virtual void v525() const; virtual void v526() const;
    virtual void v527() const; virtual void v528() const; virtual void v529() const; virtual void v530() const; virtual void v531() const; virtual void v532() const; virtual void v533() const; virtual void v534() const;
    virtual void v535() const; virtual void v536() const; virtual void v537() const; virtual void v538() const; virtual void v539() const; virtual void v540() const; virtual void v541() const; virtual void v542() const;
    virtual void v543() const; virtual void v544() const; virtual void v545() const; virtual void v546() const; virtual void v547() const; virtual void v548() const; virtual void v549() const; virtual void v550() const;
    virtual void v551() const; virtual void v552() const; virtual void v553() const; virtual void v554() const; virtual void v555() const; virtual void v556() const; virtual void v557() const; virtual void v558() const;
    virtual void v559() const; virtual void v560() const; virtual void v561() const; virtual void v562() const; virtual void v563() const; virtual void v564() const; virtual void v565() const; virtual void v566() const;
    virtual void v567() const; virtual void v568() const; virtual void v569() const; virtual void v570() const; virtual void v571() const; virtual void v572() const; virtual void v573() const; virtual void v574() const;
    virtual void v575() const; virtual void v576() const; virtual void v577() const; virtual void v578() const; virtual void v579() const; virtual void v580() const; virtual void v581() const; virtual void v582() const;
    virtual void v583() const; virtual void v584() const; virtual void v585() const; virtual void v586() const; virtual void v587() const; virtual void v588() const; virtual void v589() const; virtual void v590() const;
    virtual void v591() const; virtual void v592() const; virtual void v593() const; virtual void v594() const; virtual void v595() const; virtual void v596() const; virtual void v597() const; virtual void v598() const;
    virtual void v599() const; virtual void v600() const; virtual void v601() const; virtual void v602() const; virtual void v603() const; virtual void v604() const; virtual void v605() const; virtual void v606() const;
    virtual void v607() const; virtual void v608() const; virtual void v609() const; virtual void v610() const; virtual void v611() const; virtual void v612() const; virtual void v613() const; virtual void v614() const;
    virtual void v615() const; virtual void v616() const; virtual void v617() const; virtual void v618() const; virtual void v619() const; virtual void v620() const; virtual void v621() const; virtual void v622() const;
    virtual void v623() const; virtual void v624() const; virtual void v625() const; virtual void v626() const; virtual void v627() const; virtual void v628() const; virtual void v629() const; virtual void v630() const;
    virtual void v631() const; virtual void v632() const; virtual void v633() const; virtual void v634() const; virtual void v635() const; virtual void v636() const; virtual void v637() const; virtual void v638() const;
    virtual void v639() const; virtual void v640() const; virtual void v641() const; virtual void v642() const; virtual void v643() const; virtual void v644() const; virtual void v645() const; virtual void v646() const;
    virtual void v647() const; virtual void v648() const; virtual void v649() const; virtual void v650() const; virtual void v651() const; virtual void v652() const; virtual void v653() const; virtual void v654() const;
    virtual void v655() const; virtual void v656() const; virtual void v657() const; virtual void v658() const; virtual void v659() const; virtual void v660() const; virtual void v661() const; virtual void v662() const;
    virtual void v663() const; virtual void v664() const; virtual void v665() const; virtual void v666() const; virtual void v667() const; virtual void v668() const; virtual void v669() const; virtual void v670() const;
    virtual void v671() const; virtual void v672() const; virtual void v673() const; virtual void v674() const; virtual void v675() const; virtual void v676() const; virtual void v677() const; virtual void v678() const;
    virtual void v679() const; virtual void v680() const; virtual void v681() const; virtual void v682() const; virtual void v683() const; virtual void v684() const; virtual void v685() const; virtual void v686() const;
    virtual void v687() const; virtual void v688() const; virtual void v689() const; virtual void v690() const; virtual void v691() const; virtual void v692() const; virtual void v693() const; virtual void v694() const;
    virtual void v695() const; virtual void v696() const; virtual void v697() const; virtual void v698() const; virtual void v699() const; virtual void v700() const; virtual void v701() const; virtual void v702() const;
    virtual void v703() const; virtual void v704() const; virtual void v705() const; virtual void v706() const; virtual void v707() const; virtual void v708() const; virtual void v709() const; virtual void v710() const;
    virtual void v711() const; virtual void v712() const; virtual void v713() const; virtual void v714() const; virtual void v715() const; virtual void v716() const; virtual void v717() const; virtual void v718() const;
    virtual void v719() const; virtual void v720() const; virtual void v721() const; virtual void v722() const; virtual void v723() const; virtual void v724() const; virtual void v725() const; virtual void v726() const;
    virtual void v727() const; virtual void v728() const; virtual void v729() const; virtual void v730() const; virtual void v731() const; virtual void v732() const; virtual void v733() const; virtual void v734() const;
    virtual void v735() const; virtual void v736() const; virtual void v737() const; virtual void v738() const; virtual void v739() const; virtual void v740() const; virtual void v741() const; virtual void v742() const;
    virtual void v743() const; virtual void v744() const;
    virtual void SetIsThingForcePushable(const CScriptThing& a0, bool a1) const;   // slot 745 (+0xba4) 0x0088F9F0
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
void __fastcall FadeOutAndKillEntity(const CScriptThing& thing, bool fade, float time, bool kill);           // 0x00CBE716
struct CQ_HangingTreeGoodScript {
    unsigned char _pad00[0x49];
    bool PartySuccessfullySetUp;                        // +0x49 (PDB +0x5d)
    unsigned char _pad4a;
    bool MissionFailed;                                 // +0x4b (PDB +0x5f)
    unsigned char _pad4c[0x57 - 0x4c];
    bool BanditsFlee;                                   // +0x57 (PDB +0x6b)
    bool PrisonerEscaping;                              // +0x58 (PDB +0x6c)
    unsigned char _pad59[3];
    long HTG_State;                                     // +0x5c (PDB +0x70)
    unsigned char _pad60[0x68 - 0x60];
    CScriptThing Leader;                                // +0x68 (PDB +0x7c)
    unsigned char _pad74[0x9c - 0x74];
    CCharString ReasonForFailing;                       // +0x9c (PDB +0xb0)
};
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CHTG_Prisoner : CEntityScriptBase {
    CQ_HangingTreeGoodScript* ParentClass;              // +0x14
    void*                     PMasterData;              // +0x18
    CScriptThing              ChopBlockMarker;          // +0x1c
    bool                      ReturningToParty;         // +0x28
    bool                      GuardsCalledAfterMe;      // +0x29
    void Main();
};

void CHTG_Prisoner::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    CScriptGameResourceObjectScriptedThingBase prisoner;
    if (prisoner.IsScripting())
        prisoner.Release();
    while (!Game->StartScriptingEntity(Me, prisoner, (EScriptAIPriority)4)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing exitPoint;
    Game->EntitySetAsHavingBoundHands(Me, true);
    Game->SetIsThingForcePushable(Me, false);
    while (!ParentClass->PartySuccessfullySetUp) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (ParentClass->HTG_State != 7) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ParentClass->HTG_State == 2) {
            if (IsActiveThreadTerminating())
                return;
            prisoner.FollowThing(ParentClass->Leader, 1.0f, true);
            while (ParentClass->HTG_State == 2) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        } else if (ParentClass->HTG_State == 5) {
            if (IsActiveThreadTerminating())
                return;
            exitPoint = Game->GetNearestWithDefName(Me, "REGION_EXIT_POINT");
            GuardsCalledAfterMe = false;
            prisoner.ClearCommands();
            if (exitPoint.IsAlive() && exitPoint.GetDataString() != "NoQuestRoute") {
                if (IsActiveThreadTerminating())
                    return;
                while (ParentClass->HTG_State == 5) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                    if (!GuardsCalledAfterMe && !ReturningToParty
                        && IsDistanceBetweenThingsOver(Me, ParentClass->Leader, 12.0f)) {
                        long conversation = Game->AddNewConversation(ParentClass->Leader, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_030_GUARD_PRISONER_ESCAPING", false, ParentClass->Leader, Game->GetHero());
                        GuardsCalledAfterMe = true;
                    } else if (!prisoner.IsPerformingScriptTask()) {
                        prisoner.MoveToPosition(exitPoint.GetPos(), 0.0f, 0, false, true);
                        ParentClass->PrisonerEscaping = true;
                        ReturningToParty = false;
                    } else if (IsDistanceBetweenThingsUnder(Me, exitPoint, 1.5f)) {
                        FadeOutAndKillEntity(Me, true, 0.5f, true);
                        ParentClass->BanditsFlee = true;
                        Game->Pause(10.0f);
                        ParentClass->ReasonForFailing = "TEXT_QST_030_FAILURE_PRISONER_ESCAPED";
                        ParentClass->MissionFailed = true;
                    } else {
                        bool closeToHero = IsDistanceBetweenThingsUnder(Me, Game->GetHero(), 3.0f);
                        if (closeToHero && !ReturningToParty) {
                            prisoner.ClearCommands();
                            prisoner.FollowThing(ParentClass->Leader, 1.0f, true);
                            if (GuardsCalledAfterMe) {
                                long conversation = Game->AddNewConversation(Me, false, false);
                                Game->AddLineToConversation(conversation, "TEXT_QST_030_PRISONER_RETURNING", false, Me, Game->GetHero());
                                GuardsCalledAfterMe = false;
                            }
                            ReturningToParty = true;
                            ParentClass->PrisonerEscaping = false;
                        } else if (ReturningToParty && IsDistanceBetweenThingsOver(Me, Game->GetHero(), 4.0f)) {
                            prisoner.ClearCommands();
                            ReturningToParty = false;
                        }
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
                prisoner.ClearCommands();
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            prisoner.ClearCommands();
            while (ParentClass->HTG_State != 2 && ParentClass->HTG_State != 7) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    prisoner.ClearCommands();
    if (prisoner.IsScripting())
        prisoner.Release();
    for (;;) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
}