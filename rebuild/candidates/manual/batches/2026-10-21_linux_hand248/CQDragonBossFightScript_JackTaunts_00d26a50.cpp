// CQ_DragonBossFightScript::JackTaunts() -- script thread: every 20 seconds of script timer, Jack taunts the hero with the next of the TEXT_QST_B05_JACK_TAUNT_10 .. _50 lines (a one-line conversation spoken by and to the hero), five taunts in all
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D26A50, length 325 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; the global game script interface 0x0143E8F8 (the CTimer
// local): RegisterTimer (slot 87), DeregisterTimer (88), SetTimer (89), GetTimer (90); IntToCharString(long)
// 0x0099F830 and operator+(const char*, const CCharString&) 0x0099F690 (both __fastcall with the hidden result in
// ecx), ~CCharString 0x0099EAE0; through the quest's interface (+0x40): NewScriptFrame (7), GetHero (70),
// AddNewConversation (364), AddLineToConversation (366).
// Strings: "TEXT_QST_B05_JACK_TAUNT_" 0x012CB9B8.
// Shape: the landed CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary (0x00EB0700) CTimer local (index kept in
// edi and its frame slot, deregistered on every exit); the taunt counter only advances when a taunt is spoken, so the
// loop is `while (taunt < 60)` with the step inside the timer arm (the first test folds away); the line is
// `"TEXT_..." + IntToCharString(taunt)` built as two argument temporaries; the counter is declared before the first
// SetTime (VC then schedules its `mov ebx,10` ahead of that call).
struct CScriptThing;
struct CCharString {
    void* m_pStringData;
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall IntToCharString(long value);       // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

struct CGameScriptInterface {                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual const CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078();
    virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086();
    virtual long RegisterTimer();   // slot 87 (+0x15c)
    virtual void DeregisterTimer(long timer);   // slot 88 (+0x160)
    virtual void SetTimer(long timer, long seconds) const;   // slot 89 (+0x164)
    virtual long GetTimer(long timer) const;   // slot 90 (+0x168)
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098();
    virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122();
    virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138();
    virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146();
    virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162();
    virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170();
    virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186();
    virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210();
    virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218();
    virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
    virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234();
    virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242();
    virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250();
    virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258();
    virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266();
    virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298();
    virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322();
    virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338();
    virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346();
    virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362();
    virtual void v363();
    virtual long AddNewConversation(const CScriptThing& speaker, bool a, bool b) const;   // slot 364 (+0x5b0)
    virtual void v365();
    virtual void AddLineToConversation(long conversation, const CCharString& line, bool a, const CScriptThing& speaker, const CScriptThing& listener) const;   // slot 366 (+0x5b8)
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

struct CTimer {                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_DragonBossFightScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void JackTaunts();
};
void CQ_DragonBossFightScript::JackTaunts()
{
    CTimer timer;
    long taunt = 10;
    timer.SetTime(20);
    while (taunt < 60) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = GSI->AddNewConversation(GSI->GetHero(), false, false);
            GSI->AddLineToConversation(conversation, "TEXT_QST_B05_JACK_TAUNT_" + IntToCharString(taunt), false,
                                       GSI->GetHero(), GSI->GetHero());
            timer.SetTime(20);
            taunt += 10;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}
