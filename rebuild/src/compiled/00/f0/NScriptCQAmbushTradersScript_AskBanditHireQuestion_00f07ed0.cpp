// NScript::CQ_AmbushTradersScript::AskBanditHireQuestion(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&) -- ask the hero whether to hire the bandit; on "yes" with enough gold pay him and return true, otherwise make the bandit say why not (if he still has health) and return false
// Leaf: ?AskBanditHireQuestion@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F07ED0, length 782 bytes (padded to 784), ret 8.
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// CScriptThing vtable 0x01238C8C slot 75 IsAlive, out-of-line ~CScriptThing 0x004AA840 (first GetScriptThing temporary
// only), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptGameResourceObjectScriptedThingBase vtable 0x0127094C
// slots 12 GetScriptThing, 13 Speak, 26 IsPerformingScriptTask; CGameScriptInterface slots 7, 39, 70, 114, 126, 127, 264, 520.
// Layout: Game +0x40 (Ego_r.pdb - 0x14); pGlobalScriptDef (0x0143E90C) AmbushTradersBanditHireCost is PDB 0xa7c, retail +0x9dc.
// Shape: the "cash poor" health test destroys its CScriptThing temporary out of line (inline_depth(0) region) while the
// "no hire" one inlines the destructor (__forceinline); the question's last literal reuses the bandit argument's slot.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); 
    virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); 
    virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); 
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); 
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); 
    virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); 
    virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); 
    virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074(); 
    virtual bool IsAlive() const;                      // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThing> PImp;
    __forceinline ~CScriptThing() { }                  // out-of-line copy 0x004AA840
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_2 = 2 };
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CScriptThing GetScriptThing() const;       // slot 12 (+0x30) 0x007E7490
    virtual void Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34) 0x007E7390
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual bool IsPerformingScriptTask() const;       // slot 26 (+0x68) 0x007E7450
};
class CGameScriptInterface { public:                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) 0x006E7410 public: virtual void __fastcall CGameScriptInterface::NewScriptFrame(void) const
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038();
    virtual long MsgIsQuestionAnsweredYesOrNo() const;   // slot 39 (+0x9c) 0x00894300 public: virtual long __fastcall CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo(void) const
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();   // slot 70 (+0x118) 0x00891CA0 public: virtual class CScriptThing & __fastcall CGameScriptInterface::GetHero(void)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090();
    virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113();
    virtual void GiveHeroYesNoQuestion(const CCharString& question, const CCharString& yes, const CCharString& no, const CCharString& other, bool showAnswers) const;   // slot 114 (+0x1c8) 0x0089E540 public: virtual void __fastcall CGameScriptInterface::GiveHeroYesNoQuestion(class CCharString const &, class CCharString const &, class CCharString const &, class CCharString const &, bool) const
    virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125();
    virtual void GiveHeroGold(long amount) const;   // slot 126 (+0x1f8) 0x00898F40 public: virtual void __fastcall CGameScriptInterface::GiveHeroGold(long) const
    virtual long GetHeroGold() const;   // slot 127 (+0x1fc) 0x00898FA0 public: virtual long __fastcall CGameScriptInterface::GetHeroGold(void) const
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137();
    virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177();
    virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197();
    virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217();
    virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237();
    virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
    virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257();
    virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420) 0x00890400 public: virtual float __fastcall CGameScriptInterface::GetHealth(class CScriptThing const &) const
    virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274();
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284();
    virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294();
    virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304();
    virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324();
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334();
    virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344();
    virtual void v345(); virtual void v346(); virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363(); virtual void v364();
    virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374();
    virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384();
    virtual void v385(); virtual void v386(); virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393(); virtual void v394();
    virtual void v395(); virtual void v396(); virtual void v397(); virtual void v398(); virtual void v399(); virtual void v400(); virtual void v401(); virtual void v402(); virtual void v403(); virtual void v404();
    virtual void v405(); virtual void v406(); virtual void v407(); virtual void v408(); virtual void v409(); virtual void v410(); virtual void v411(); virtual void v412(); virtual void v413(); virtual void v414();
    virtual void v415(); virtual void v416(); virtual void v417(); virtual void v418(); virtual void v419(); virtual void v420(); virtual void v421(); virtual void v422(); virtual void v423(); virtual void v424();
    virtual void v425(); virtual void v426(); virtual void v427(); virtual void v428(); virtual void v429(); virtual void v430(); virtual void v431(); virtual void v432(); virtual void v433(); virtual void v434();
    virtual void v435(); virtual void v436(); virtual void v437(); virtual void v438(); virtual void v439(); virtual void v440(); virtual void v441(); virtual void v442(); virtual void v443(); virtual void v444();
    virtual void v445(); virtual void v446(); virtual void v447(); virtual void v448(); virtual void v449(); virtual void v450(); virtual void v451(); virtual void v452(); virtual void v453(); virtual void v454();
    virtual void v455(); virtual void v456(); virtual void v457(); virtual void v458(); virtual void v459(); virtual void v460(); virtual void v461(); virtual void v462(); virtual void v463(); virtual void v464();
    virtual void v465(); virtual void v466(); virtual void v467(); virtual void v468(); virtual void v469(); virtual void v470(); virtual void v471(); virtual void v472(); virtual void v473(); virtual void v474();
    virtual void v475(); virtual void v476(); virtual void v477(); virtual void v478(); virtual void v479(); virtual void v480(); virtual void v481(); virtual void v482(); virtual void v483(); virtual void v484();
    virtual void v485(); virtual void v486(); virtual void v487(); virtual void v488(); virtual void v489(); virtual void v490(); virtual void v491(); virtual void v492(); virtual void v493(); virtual void v494();
    virtual void v495(); virtual void v496(); virtual void v497(); virtual void v498(); virtual void v499(); virtual void v500(); virtual void v501(); virtual void v502(); virtual void v503(); virtual void v504();
    virtual void v505(); virtual void v506(); virtual void v507(); virtual void v508(); virtual void v509(); virtual void v510(); virtual void v511(); virtual void v512(); virtual void v513(); virtual void v514();
    virtual void v515(); virtual void v516(); virtual void v517(); virtual void v518(); virtual void v519();
    virtual void EntityGiveGold(const CScriptThing& thing, long amount) const;   // slot 520 (+0x820) 0x0089DC70 public: virtual void __fastcall CGameScriptInterface::EntityGiveGold(class CScriptThing const &, long) const
};

class CScriptDef { public:
    unsigned char _pad[0x9dc];
    long AmbushTradersBanditHireCost;                  // +0x9dc
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                   // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                  // 0x00CB7940
};
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    bool AskBanditHireQuestion(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject);
};

bool CQ_AmbushTradersScript::AskBanditHireQuestion(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject)
{
    if (bandit.IsAlive()) {
        if (IsActiveThreadTerminating()) return false;
        Game->GiveHeroYesNoQuestion("TEXT_QST_003_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        long answer = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return false;
            answer = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating()) return false;
        if (answer == 1) {
            if (IsActiveThreadTerminating()) return false;
            CScriptDef* def = pGlobalScriptDef;
            if (Game->GetHeroGold() >= -def->AmbushTradersBanditHireCost) {
                if (IsActiveThreadTerminating()) return false;
                Game->GiveHeroGold(pGlobalScriptDef->AmbushTradersBanditHireCost);
                Game->EntityGiveGold(bandit, -pGlobalScriptDef->AmbushTradersBanditHireCost);
                return true;
            }
            if (IsActiveThreadTerminating()) return false;
#pragma inline_depth(0)
            bool alive = Game->GetHealth(banditObject.GetScriptThing()) > 0.0f;
#pragma inline_depth()
            if (alive) {
                banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_CASH_POOR", TEXT_GROUP_SELECTION_2, false, true, false);
                while (banditObject.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
            return false;
        }
    }
    bool alive = Game->GetHealth(banditObject.GetScriptThing()) > 0.0f;
    if (alive) {
        banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_NO_HIRE", TEXT_GROUP_SELECTION_2, false, true, false);
        while (banditObject.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return false;
        }
        if (IsActiveThreadTerminating()) return false;
    }
    return false;
}
}