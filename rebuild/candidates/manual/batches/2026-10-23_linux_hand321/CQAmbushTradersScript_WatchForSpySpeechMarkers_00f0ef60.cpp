// CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy) -- while the spy lives, each time EnableSpeechSpy is raised pick the spy's next line (sentry-seen, hero-proximity or, near the M_AT_EntitySpeech marker, the normal lines; the first five in order, then random ones without repeating the last) and play it as a conversation with the hero, restarting SpeechSpyTimer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F0EF60, length 1824 bytes (ret 0xc: the CScriptThing argument by value).
// Callees: CCharString() 0x0099E4B0 / CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0 / operator=
//   0x0099EFB0, ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC, rand 0x00BFEB16, CScriptThing::IsAlive 0x004AB130 /
//   ~CScriptThing 0x004AA840, IsDistanceBetweenThingsUnder 0x00CBE2FF (__fastcall), CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; g_pGameScriptInterface 0x0143E8F8 slot 89 SetTimer (the inline CTimer::SetTime); the game script interface
//   (+0x40) slots 7, 70, 77, 364, 365, 366. Data: g_pScriptDef 0x0143E90C (+0xA40 .. +0xA58 spy speech distances/delays).
// Strings: "M_AT_EntitySpeech" 0x012F8080.
// Layout (retail = Ego_r.pdb - 0x14, -4 per preceding vector): SpySeenBySentry +0x5a, SpyHeroProximity +0x5d, SpyContactMade
//   +0x5f, EnableSpeechSpy +0x69, SpeechSpyCount +0x78, SpeechSpyProximityCount +0x7c, SpeechSpySentryCount +0x80,
//   SpeechSpyTimer +0xa0, lastRandomSpySpeechIdx +0xa4, SpySpeechNormal +0x140, SpySpeechRandom +0x154,
//   SpySpeechProximityNormal +0x168, SpySpeechProximityRandom +0x17c, SpySpeechSentryNormal +0x190, SpySpeechSentryRandom +0x1a4.
// Shape: generated script code; the outer wait is `for (;;) { if (terminating) return; ... NewScriptFrame(); }` (VC
//   duplicates the head test after the NewScriptFrame). Exits before the marker exists expand the spy argument's destructor
//   inline; exits after it call ~CScriptThing out of line (in-body inline_depth(0), as the landed CMeleeThunder::Main 0x00D58080).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString();                                      // 0x0099E4B0
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
extern "C" int __cdecl rand();                          // 0x00BFEB16
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                 // CCountedPointer's shared block
    long RefCount;                                      // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4
    void* Data;                                         // +8
};
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
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
#define SLOTS_2_74 \
    virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); \
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); \
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); \
    virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); \
    virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); \
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); \
    virtual void t048(); virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); \
    virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); \
    virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); \
    virtual void t072(); virtual void t073(); virtual void t074();
struct CScriptThingImpl {                               // the game-side object behind a CScriptThing (PImp)
    virtual void t000(); virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const;                       // slot 75 (+0x12c)
};
struct CBaseClass {                                     // vtable 0x01231710
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};
struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    virtual void t001(); SLOTS_2_74
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
    CCountedPointer<CScriptThingImpl> PImp;             // +0x04
    CScriptThing() {}
    __forceinline ~CScriptThing() {}                    // out-of-line copy 0x004AA840
};
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) GetHero
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual CScriptThing GetNearestWithScriptName(const CScriptThing& thing, const CCharString& name) const;   // slot 77 (+0x134) GetNearestWithScriptName
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164) SetTimer
    virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097();
    virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105();
    virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113();
    virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121();
    virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129();
    virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145();
    virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153();
    virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161();
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
    virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281();
    virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289();
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297();
    virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305();
    virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313();
    virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321();
    virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329();
    virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337();
    virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345();
    virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353();
    virtual void v354(); virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361();
    virtual void v362(); virtual void v363();
    virtual long AddNewConversation(const CScriptThing& thing, bool a, bool b);   // slot 364 (+0x5b0) AddNewConversation
    virtual void AddPersonToConversation(long conversation, const CScriptThing& thing);   // slot 365 (+0x5b4) AddPersonToConversation
    virtual void AddLineToConversation(long conversation, const CCharString& text, bool a, const CScriptThing& speaker, const CScriptThing& listener);   // slot 366 (+0x5b8) AddLineToConversation
};
struct CScriptDef {
    unsigned char _pad000[0xa40];
    float AmbushTradersSpySpeechDistance;               // +0xa40
    unsigned char _pad0a44[0xa4c - 0xa44];
    long  AmbushTradersSpyProximitySpeechDelay;         // +0xa4c
    long  AmbushTradersSpySpeechDelay;                  // +0xa50
    long  AmbushTradersSpyNoSpeechDelay;                // +0xa54
    long  AmbushTradersSpySentrySpeechDelay;            // +0xa58
};
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
extern CScriptDef* g_pScriptDef;                        // 0x0143E90C
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
namespace NScript {
struct CTimer {
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    unsigned char         _pad48[0x5a - 0x48];
    bool                  SpySeenBySentry;              // +0x5a
    bool                  SpyFollowing;                 // +0x5b
    bool                  SpyFollowingHero;             // +0x5c
    bool                  SpyHeroProximity;             // +0x5d
    unsigned char         _pad5e;
    bool                  SpyContactMade;               // +0x5f
    unsigned char         _pad60[0x69 - 0x60];
    bool                  EnableSpeechSpy;              // +0x69
    unsigned char         _pad6a[0x78 - 0x6a];
    long                  SpeechSpyCount;               // +0x78
    long                  SpeechSpyProximityCount;      // +0x7c
    long                  SpeechSpySentryCount;         // +0x80
    unsigned char         _pad84[0xa0 - 0x84];
    CTimer                SpeechSpyTimer;               // +0xa0
    long                  lastRandomSpySpeechIdx;       // +0xa4
    unsigned char         _pada8[0x140 - 0xa8];
    CCharString           SpySpeechNormal[5];           // +0x140
    CCharString           SpySpeechRandom[5];           // +0x154
    CCharString           SpySpeechProximityNormal[5];  // +0x168
    CCharString           SpySpeechProximityRandom[5];  // +0x17c
    CCharString           SpySpeechSentryNormal[5];     // +0x190
    CCharString           SpySpeechSentryRandom[5];     // +0x1a4
    void WatchForSpySpeechMarkers(CScriptThing spy);
};
void CQ_AmbushTradersScript::WatchForSpySpeechMarkers(CScriptThing spy)
{
    CCharString speech;
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        while (!EnableSpeechSpy && spy.IsAlive()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (!spy.IsAlive()) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            break;
        }
        {
        CScriptThing marker = Game->GetNearestWithScriptName(spy, "M_AT_EntitySpeech");
        if (SpySeenBySentry && !SpyContactMade && !SpyHeroProximity) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            SpeechSpyTimer.SetTime(g_pScriptDef->AmbushTradersSpySentrySpeechDelay);
            if (SpeechSpySentryCount < 5) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechSentryNormal[SpeechSpySentryCount];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    SpeechSpySentryCount++;
                }
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechSentryRandom[rand() % 5];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                }
            }
        } else if (SpyHeroProximity && !SpyContactMade) {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            SpeechSpyTimer.SetTime(g_pScriptDef->AmbushTradersSpyProximitySpeechDelay);
            if (SpeechSpyProximityCount < 5) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechProximityNormal[SpeechSpyProximityCount];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    SpeechSpyProximityCount++;
                }
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                long index;
                for (;;) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    index = rand() % 5;
                    if (index != lastRandomSpySpeechIdx)
                        break;
                    Game->NewScriptFrame();
                }
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                lastRandomSpySpeechIdx = index;
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                speech = SpySpeechProximityRandom[index];
#pragma inline_depth(0)
                if (spy.IsAlive()) {
#pragma inline_depth()
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long conversation = Game->AddNewConversation(spy, false, false);
                    Game->AddPersonToConversation(conversation, Game->GetHero());
                    Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                }
            }
        } else {
            if (IsActiveThreadTerminating())
                {
#pragma inline_depth(0)
                    return;
#pragma inline_depth()
                }
            if (IsDistanceBetweenThingsUnder(spy, marker, g_pScriptDef->AmbushTradersSpySpeechDistance)) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                if (SpeechSpyCount < 5) {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    speech = SpySpeechNormal[SpeechSpyCount];
#pragma inline_depth(0)
                    if (spy.IsAlive()) {
#pragma inline_depth()
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        long conversation = Game->AddNewConversation(spy, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                        SpeechSpyCount++;
                    }
                } else {
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    long index;
                    for (;;) {
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        index = rand() % 5;
                        if (index != lastRandomSpySpeechIdx)
                            break;
                        Game->NewScriptFrame();
                    }
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    lastRandomSpySpeechIdx = index;
                    if (IsActiveThreadTerminating())
                        {
#pragma inline_depth(0)
                            return;
#pragma inline_depth()
                        }
                    speech = SpySpeechRandom[index];
#pragma inline_depth(0)
                    if (spy.IsAlive()) {
#pragma inline_depth()
                        if (IsActiveThreadTerminating())
                            {
#pragma inline_depth(0)
                                return;
#pragma inline_depth()
                            }
                        long conversation = Game->AddNewConversation(spy, false, false);
                        Game->AddPersonToConversation(conversation, Game->GetHero());
                        Game->AddLineToConversation(conversation, speech, false, spy, Game->GetHero());
                    }
                }
                SpeechSpyTimer.SetTime(g_pScriptDef->AmbushTradersSpySpeechDelay);
            } else {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                SpeechSpyTimer.SetTime(g_pScriptDef->AmbushTradersSpyNoSpeechDelay);
            }
        }
        EnableSpeechSpy = false;
#pragma inline_depth(0)
        }
#pragma inline_depth()
        Game->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        {
#pragma inline_depth(0)
            return;
#pragma inline_depth()
        }
    {
#pragma inline_depth(0)
        return;
#pragma inline_depth()
    }
}
}
