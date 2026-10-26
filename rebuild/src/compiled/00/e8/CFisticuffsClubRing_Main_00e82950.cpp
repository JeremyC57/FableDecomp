// CFisticuffsClubRing::Main() -- the Fisticuffs Club ring trigger: every frame track whether the hero is inside the ring radius; inside, cancel the "step outside" warning and, while an active club fight is idle, have a club member tell the hero to stop interrupting (rate-limited); stepping out while in the club triggers Tyler's "step outside" warning with a HUD clock; while in the club, leaving the ring during a fight raises the boo meter and times the hero out, otherwise fights raise the cheer meter; both meters are clamped to 0..12
// Leaf: ?Main@CFisticuffsClubRing@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E82950, 1574 bytes (+ int3 padding), plain `ret`; entity-script vtable slot 1 (Main).
// Callees: CScriptThing copy 0x004ABE90 / dtor 0x004AA840, CIsThingAlive dtor 0x00E241B0, RegisterFilter 0x00F35B10,
//   IsActiveThreadTerminating 0x00F35B30, CFisticuffsClubRing helper 0x00E83230 (manifest label "HandleVillageSelection" is a
//   propagated one), CV_FisticuffsClubScript::InitialiseVariables 0x00E82650 / GetQuestText 0x00E82F80, CTimer dtor 0x00CD4470,
//   IsDistanceBetweenThingsUnder 0x00CBE2FF / Over 0x00CBE3EA, __ftol2 0x00BFEA70, CCharString 0x0099EBF0 / 0x0099EAE0;
//   GameCalls slots 7, 70, 72, 77, 87-90 (timers through g_pGameScriptInterface 0x0143E8F8), 326 AddQuestInfoTimer,
//   338 RemoveQuestInfoElement, 364-366.
// Layout: Ego_r.pdb CFisticuffsClubRing: CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14 (CV_FisticuffsClubScript,
//   retail = PDB - 0x14), PMasterData +0x18. CScriptDef (pGlobalScriptDef 0x0143E90C, retail = PDB - 0x5c here): FC_OutsideRingRange
//   +0x5e4, FC_RingTimeOut +0x5e8, FC_RingRadius +0x61c, FC_InsideRingMessageDelay +0x620.
namespace NScript { class CPredicateBase; }
class CTimer;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
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
struct CScriptThingImpl;
struct CBase {                                          // vtable 0x01231710
    CBase() { }                                         // out-of-line copy 0x0099A2D0
    virtual ~CBase();                                   // 0x0099A2E0
};
class C3DVector { public: float X, Y, Z; };
class CScriptThing : public CBase { public:                           // vtable 0x01238C8C, 12 bytes
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
class CScriptGameResourceObjectBase : public CBaseIntelligentPointer { public:   // vtable 0x0126008C
    CScriptGameResourceObjectBase() { }
    __forceinline ~CScriptGameResourceObjectBase() { }
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
class CGameScriptInterface { public:                          // vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const;
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410
    virtual void v008() const; virtual void v009() const; virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const;
    virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const; virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const;
    virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const; virtual void v030() const; virtual void v031() const;
    virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const;
    virtual void v048() const; virtual void v049() const; virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const;
    virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const; virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const;
    virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0
    virtual void v071() const;
    virtual CScriptThing GetThingWithScriptName(const CCharString& a0) const;   // slot 72 (+0x120) 0x008A7D60
    virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const;
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& a0, const CCharString& a1) const;   // slot 77 (+0x134) 0x0089A510
    virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const; virtual void v083() const; virtual void v084() const; virtual void v085() const;
    virtual void v086() const;
    virtual long RegisterTimer();   // slot 87 (+0x15c) 0x006E79E0
    virtual void DeregisterTimer(long a0);   // slot 88 (+0x160) 0x006E7690
    virtual void SetTimer(long a0, long a1);   // slot 89 (+0x164) 0x006E76E0
    virtual long GetTimer(long a0) const;   // slot 90 (+0x168) 0x006E7710
    virtual void v091() const; virtual void v092() const; virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const; virtual void v097() const; virtual void v098() const;
    virtual void v099() const; virtual void v100() const; virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const;
    virtual void v107() const; virtual void v108() const; virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const; virtual void v114() const;
    virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const; virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const;
    virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const; virtual void v129() const; virtual void v130() const;
    virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const;
    virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const;
    virtual void v147() const; virtual void v148() const; virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const;
    virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const; virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const;
    virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const; virtual void v169() const; virtual void v170() const;
    virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const;
    virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const;
    virtual void v187() const; virtual void v188() const; virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const;
    virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const; virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const;
    virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const; virtual void v209() const; virtual void v210() const;
    virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const;
    virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const;
    virtual void v227() const; virtual void v228() const; virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const;
    virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const; virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const;
    virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const; virtual void v249() const; virtual void v250() const;
    virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const;
    virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const; virtual void v264() const; virtual void v265() const; virtual void v266() const;
    virtual void v267() const; virtual void v268() const; virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const; virtual void v274() const;
    virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const; virtual void v279() const; virtual void v280() const; virtual void v281() const; virtual void v282() const;
    virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const; virtual void v289() const; virtual void v290() const;
    virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const; virtual void v297() const; virtual void v298() const;
    virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const; virtual void v305() const; virtual void v306() const;
    virtual void v307() const; virtual void v308() const; virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const; virtual void v314() const;
    virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const; virtual void v319() const; virtual void v320() const; virtual void v321() const; virtual void v322() const;
    virtual void v323() const; virtual void v324() const; virtual void v325() const;
    virtual long AddQuestInfoTimer(const CTimer& a0, const CCharString& a1, float a2) const;   // slot 326 (+0x518) 0x00891B20
    virtual void v327() const; virtual void v328() const; virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const;
    virtual void v335() const; virtual void v336() const; virtual void v337() const;
    virtual void RemoveQuestInfoElement(long a0) const;   // slot 338 (+0x548) 0x00891C10
    virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const; virtual void v345() const; virtual void v346() const;
    virtual void v347() const; virtual void v348() const; virtual void v349() const; virtual void v350() const; virtual void v351() const; virtual void v352() const; virtual void v353() const; virtual void v354() const;
    virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const; virtual void v359() const; virtual void v360() const; virtual void v361() const; virtual void v362() const;
    virtual void v363() const;
    virtual long AddNewConversation(const CScriptThing& a0, bool a1, bool a2) const;   // slot 364 (+0x5b0) 0x008906C0
    virtual void AddPersonToConversation(long a0, const CScriptThing& a1) const;   // slot 365 (+0x5b4) 0x00890710
    virtual void AddLineToConversation(long a0, const CCharString& a1, bool a2, const CScriptThing& a3, const CScriptThing& a4) const;   // slot 366 (+0x5b8) 0x00890750
};
struct CScriptThingCopy : CBase {                       // CScriptThing as the filter holds it: out-of-line copy and destructor
    virtual void s001();
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThingCopy(const CScriptThing& other);        // 0x004ABE90
    ~CScriptThingCopy();                                // 0x004AA840
};
namespace NScript {
class CPredicateBase
{
public:
    virtual void v00();
};
}
struct CIsThingAlive : public NScript::CPredicateBase {                                  // vtable 0x012C32F0
    CScriptThingCopy m_thing;                           // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                                   // 0x00E241B0
};
extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }   // out-of-line copy 0x00CD4470
    __forceinline void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    __forceinline long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
class CScriptDef { public:
    unsigned char _pad0[0x5e4];
    float FC_OutsideRingRange;                          // +0x5e4
    float FC_RingTimeOut;                               // +0x5e8
    unsigned char _pad1[0x61c - 0x5ec];
    float FC_RingRadius;                                // +0x61c
    long  FC_InsideRingMessageDelay;                    // +0x620
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);    // 0x00CBE3EA
struct CV_FisticuffsClubScript {
    unsigned char _pad0[0x4c];
    bool  FighterOneInRing;                             // +0x4c
    bool  FighterTwoInRing;                             // +0x4d
    unsigned char _pad1[0x50 - 0x4e];
    bool  HeroInsideRing;                               // +0x50
    unsigned char _pad2;
    bool  HeroPaidEntryFee;                             // +0x52
    bool  HeroInFisticuffsClub;                         // +0x53
    bool  TylerInFight;                                 // +0x54
    unsigned char _pad3;
    bool  TimeOut;                                      // +0x56
    unsigned char _pad4;
    bool  RoundStarted;                                 // +0x58
    unsigned char _pad5[0x8c - 0x59];
    bool  EverybodyAttack;                              // +0x8c
    bool  LostWhileOutside;                             // +0x8d
    bool  FisticuffsActive;                             // +0x8e
    unsigned char _pad6[0xac - 0x8f];
    float CheerNumber;                                  // +0xac
    float BooNumber;                                    // +0xb0
    void InitialiseVariables();                         // 0x00E82650
    CCharString GetQuestText(CCharString suffix);       // 0x00E82F80
};
inline void ClampMeter(float& meter)
{
    if (meter < 0.0f)
        meter = 0.0f;
    else if (meter > 12.0f)
        meter = 12.0f;
}
class CEntityScriptBase { public:
    virtual void e00();
    CGameScriptInterface* Game;                         // +0x04
    CScriptThing          Me;                           // +0x08
    bool IsActiveThreadTerminating() const;             // 0x00F35B30
public:
    void SetPredicate(const NScript::CPredicateBase&);   // 0x00F35B10
};
struct CFisticuffsClubRing : CEntityScriptBase {
    CV_FisticuffsClubScript* ParentClass;               // +0x14
    void*                PMasterData;                   // +0x18
    void Main();
    void SetUpRing();                                   // 0x00E83230
};

void CFisticuffsClubRing::Main()
{
    {
        CIsThingAlive filter(Me);
        SetPredicate(filter);
    }
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    SetUpRing();
    ParentClass->InitialiseVariables();
    CTimer warningTimer;
    CTimer interruptTimer;
    float ringRadius = pGlobalScriptDef->FC_RingRadius;
    long warningInfo = 0;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsUnder(Me, Game->GetHero(), ringRadius)) {
            if (IsActiveThreadTerminating())
                return;
            ParentClass->HeroInsideRing = true;
            if (warningTimer.GetTime() != 0) {
                if (IsActiveThreadTerminating())
                    return;
                Game->RemoveQuestInfoElement(warningInfo);
                warningTimer.SetTime(0);
            }
            if (ParentClass->FisticuffsActive && !ParentClass->HeroInFisticuffsClub && !ParentClass->HeroPaidEntryFee
                && !ParentClass->EverybodyAttack && interruptTimer.GetTime() < 1) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                CScriptThing member = Game->GetNearestWithScriptName(Me, "FisticuffsMember");
                long conversation = Game->AddNewConversation(member, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("INTERRUPT_FIGHT_10"), false, member, Game->GetHero());
                interruptTimer.SetTime(pGlobalScriptDef->FC_InsideRingMessageDelay);
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->HeroInFisticuffsClub && ParentClass->HeroInsideRing) {
                if (IsActiveThreadTerminating())
                    return;
                CScriptThing tyler = Game->GetThingWithScriptName("Tyler");
                long conversation = Game->AddNewConversation(tyler, false, false);
                Game->AddPersonToConversation(conversation, Game->GetHero());
                Game->AddLineToConversation(conversation, ParentClass->GetQuestText("STEP_OUTSIDE_RING_10"), false, tyler, Game->GetHero());
                warningTimer.SetTime((long)pGlobalScriptDef->FC_RingTimeOut);
                warningInfo = Game->AddQuestInfoTimer(warningTimer, "HUD_CLOCK_ICON", 1.0f);
            }
            ParentClass->HeroInsideRing = false;
        }
        if (ParentClass->HeroInFisticuffsClub) {
            if (IsActiveThreadTerminating())
                return;
            if ((ParentClass->RoundStarted || ParentClass->TylerInFight)
                && IsDistanceBetweenThingsOver(Me, Game->GetHero(), ringRadius)) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->BooNumber += 1.0f / 15.0f;
                if (warningTimer.GetTime() == 0
                    || IsDistanceBetweenThingsOver(Me, Game->GetHero(), pGlobalScriptDef->FC_OutsideRingRange)) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    ParentClass->LostWhileOutside = true;
                    ParentClass->TimeOut = true;
                    Game->RemoveQuestInfoElement(warningInfo);
                }
            } else if (ParentClass->TylerInFight) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 15.0f;
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 30.0f;
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (ParentClass->FighterTwoInRing) {
                if (ParentClass->FighterOneInRing) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    ParentClass->CheerNumber += 1.0f / 75.0f;
                }
            } else if (ParentClass->FighterOneInRing) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                ParentClass->CheerNumber += 1.0f / 150.0f;
            }
        }
        ClampMeter(ParentClass->CheerNumber);
        ClampMeter(ParentClass->BooNumber);
        Game->NewScriptFrame();
        continue;
    }
}