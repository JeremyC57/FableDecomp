// Grade: DIFFER(979v956) -- loop/exit layout: retail keeps ebp pushed at entry and merges the bottom-of-loop terminate return into the entry return block (je reload; jmp A); ours shrink-wraps push ebp and emits a separate inline-destructor exit copy after the loop
// NScript::CQR_EscortTraderScript::WatchForTraderHit(CScriptThing) -- watch thread while the escorted trader lives: note when the hero hits him, and when the hero is the attacker count the hit and (timer permitting) have the trader complain
// Leaf: WatchForTraderHit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E20210, length 956 bytes (padded to 960), ret 0xc (CScriptThing by value).
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 /
// operator==(const CCharString&) const 0x0099E900; CScriptThing (vtable 0x01238C8C) GetDefName slot 2 0x004AA8D0,
// MsgWhoHitMe slot 26 0x004ABEC0, ~CScriptThing 0x004AA840 (out of line), ~CBaseClass 0x0099A2E0, operator delete
// 0x00BFE9BC; the impl slots 21 MsgIsHitBy, 43 MsgIsHitByAnyAggressiveSpecialAbilityFrom, 75 IsAlive behind the inline
// null-safe wrappers; CGameScriptInterface slots 7, 70, 264 GetHealth, 364, 365, 366; the timer service
// g_pGameScriptInterface (0x0143E8F8) slots 89 SetTimer / 90 GetTimer through CTimer. Float 0.0001f 0x0129BA3C.
// Layout: Ego_r.pdb - 0x14 (TraderHit +0x4f, TraderHitCount +0x64, TraderComplainTimer +0x78); g_pScriptDef (0x0143E90C)
// EscortTraderAttackComplaintTime is PDB 0xa04, retail +0x96c.
// Shape: as WatchForDeputy1Killed 0x00D16570: __forceinline ~CScriptThing, with the returns after the first hit test
// in inline_depth(0) regions (out-of-line destructor); the `||` literal temporaries are flag-guarded.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const CCharString& other) const;   // 0x0099E900
};
void operator delete(void* p);                         // 0x00BFE9BC
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
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
struct CBaseClass {
    virtual ~CBaseClass();                             // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                     // vtable 0x01238C8C, 12 bytes
    virtual void s001();
    virtual CCharString GetDefName() const;            // slot 2 (+0x08) 0x004AA8D0
    virtual void s003(); virtual void s004(); virtual void s005(); virtual void s006(); virtual void s007(); virtual void s008(); virtual void s009(); virtual void s010(); 
    virtual void s011(); virtual void s012(); virtual void s013(); virtual void s014(); virtual void s015(); virtual void s016(); virtual void s017(); virtual void s018(); 
    virtual void s019(); virtual void s020(); 
    virtual bool MsgIsHitBy(const CCharString& attacker) const   // slot 21 (+0x54) 0x004AAB30
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitBy(attacker);
    }
    virtual void s022(); virtual void s023(); virtual void s024(); virtual void s025();
    virtual CScriptThing MsgWhoHitMe() const;          // slot 26 (+0x68) 0x004ABEC0
    virtual void s027(); virtual void s028(); virtual void s029(); virtual void s030(); virtual void s031(); virtual void s032(); virtual void s033(); virtual void s034(); 
    virtual void s035(); virtual void s036(); virtual void s037(); virtual void s038(); virtual void s039(); virtual void s040(); virtual void s041(); virtual void s042(); 
    
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const   // slot 43 (+0xac) 0x004AAD60
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
    }
    virtual void s044(); virtual void s045(); virtual void s046(); virtual void s047(); virtual void s048(); virtual void s049(); virtual void s050(); virtual void s051(); 
    virtual void s052(); virtual void s053(); virtual void s054(); virtual void s055(); virtual void s056(); virtual void s057(); virtual void s058(); virtual void s059(); 
    virtual void s060(); virtual void s061(); virtual void s062(); virtual void s063(); virtual void s064(); virtual void s065(); virtual void s066(); virtual void s067(); 
    virtual void s068(); virtual void s069(); virtual void s070(); virtual void s071(); virtual void s072(); virtual void s073(); virtual void s074(); 
    virtual bool IsAlive() const                       // slot 75 (+0x12c) 0x004AB130
    {
        return PImp.Data && PImp.Data->IsAlive();
    }
    CCountedPointer<CScriptThing> PImp;                // +0x04 Data, +0x08 Info
    __forceinline ~CScriptThing() { }                  // out-of-line copy 0x004AA840
};
struct CGameScriptInterface {                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410 public: virtual void __thiscall CGameScriptInterface::NewScriptFrame(void) const
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057();
    virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067();
    virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0 public: virtual class CScriptThing & __thiscall CGameScriptInterface::GetHero(void)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164) 0x006E76E0 public: virtual void __thiscall CGameScriptInterface::SetTimer(long, long)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168) 0x006E7710 public: virtual long __thiscall CGameScriptInterface::GetTimer(long) const
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140();
    virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
    virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180();
    virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
    virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
    virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260();
    virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420) 0x00890400 public: virtual float __thiscall CGameScriptInterface::GetHealth(class CScriptThing const &) const
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
    virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363();
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5b0) 0x008906C0 public: virtual long __thiscall CGameScriptInterface::AddNewConversation(class CScriptThing const &, bool, bool) const
    virtual void AddPersonToConversation(long conversation, const CScriptThing& person) const;   // slot 365 (+0x5b4) 0x00890710 public: virtual void __thiscall CGameScriptInterface::AddPersonToConversation(long, class CScriptThing const &) const
    virtual void AddLineToConversation(long conversation, const CCharString& line, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8) 0x00890750 public: virtual void __thiscall CGameScriptInterface::AddLineToConversation(long, class CCharString const &, bool, class CScriptThing const &, class CScriptThing const &) const
};

extern CGameScriptInterface* g_pGameScriptInterface;   // 0x0143E8F8
struct CTimer {                                        // 4 bytes: an index into the global script timers
    long TimerIndex;
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CScriptDef {
    unsigned char _pad[0x96c];
    long EscortTraderAttackComplaintTime;              // +0x96c
};
extern CScriptDef* g_pScriptDef;                       // 0x0143E90C
struct CScriptBase {                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
namespace NScript {
struct CQR_EscortTraderScript : CScriptBase {
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    bool MissionSucceeded;                             // +0x48
    bool MissionFailed;                                // +0x49
    bool HiredByTrader;                                // +0x4a
    bool TraderAtDestination;                          // +0x4b
    bool TraderDepartedLost;                           // +0x4c
    bool GreetedHero;                                  // +0x4d
    bool TraderKilled;                                 // +0x4e
    bool TraderHit;                                    // +0x4f
    unsigned char _pad_0x50[0x64 - 0x50];
    long TraderHitCount;                               // +0x64
    long TraderHealthBarID;                            // +0x68
    float TraderInitialHealth;                         // +0x6c
    unsigned char _pad_0x70[0x78 - 0x70];
    CTimer TraderComplainTimer;                        // +0x78
    void WatchForTraderHit(CScriptThing trader);
};

void CQR_EscortTraderScript::WatchForTraderHit(CScriptThing trader)
{
    if (IsActiveThreadTerminating())
        return;
    while (trader.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        bool hitByHero = trader.MsgIsHitBy("SCRIPT_NAME_HERO") || trader.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (hitByHero) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (Game->GetHealth(trader) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                TraderHit = true;
            }
        }
        if (trader.MsgIsHitBy("")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            if (Game->GetHealth(trader) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                CScriptThing attacker = trader.MsgWhoHitMe();
                if (attacker.GetDefName() == Game->GetHero().GetDefName()) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    ++TraderHitCount;
                    if (TraderComplainTimer.GetTime() == 0) {
#pragma inline_depth(0)
                        if (IsActiveThreadTerminating())
                            return;
#pragma inline_depth()
                        long conversation = Game->AddNewConversation(trader, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, "TEXT_QST_053_COMPLAIN", false, trader, Game->GetHero());
                        TraderComplainTimer.SetTime(g_pScriptDef->EscortTraderAttackComplaintTime);
                    }
                }
#pragma inline_depth(0)
            }
#pragma inline_depth()
        }
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            break;
    }
    if (IsActiveThreadTerminating())
        return;
}
}
