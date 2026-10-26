// CQ_PrisonWardenGameScript::WardenKeyMiniGame() -- sparkles the prison cell key rack and keeps it targetable only while the hero is within 3 units, until the hero uses it; then marks the diary key taken, shows this race's diary code number (year one or two) and removes the sparkles
// Leaf: WardenKeyMiniGame
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DDE230, 1017 bytes, plain `ret`.
// Callees: ~CScriptThing 0x004AA840 (out of line on the key-used path), ~CBaseClass 0x0099A2E0, operator delete
//   0x00BFE9BC, IsDistanceBetweenThingsOver 0x00CBE3EA (__fastcall), CCharString 0x0099EBF0 / 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; the inline CScriptThing::GetPos (impl slot 6, g_ZeroVector
//   0x0143E8E0) and MsgIsUsedBy (impl slot 20); interface slots 7, 40 MsgIsGameInfoClickedPast, 70, 72, 100 CreateEffect,
//   108 RemoveThing, 115 DisplayGameInfo, 469 EntitySetTargetable.
// Layout: HeroHasSecretDiaryKey +0x49 (retail = PDB - 0x14); master data PrisonRaceNumber +0xe8 (PDB offset).
// Shape: for(;;) with the terminating return FIRST (rotated so the entry test and the loop-end test share one inline
//   destructor copy); the key-used block is inside the loop and ends in a return, all of it under
//   `#pragma inline_depth(0)` so its exits call ~CScriptThing out of line. Relocation targets checked (probe/rpair.py).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
extern C3DVector g_ZeroVector;                          // 0x0143E8E0

class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};

struct CScriptThingImp {                                // the implementation object's vtable view
    virtual void i000(); virtual void i001(); virtual void i002(); virtual void i003(); virtual void i004(); virtual void i005();
    virtual const C3DVector& GetPos() const;            // slot 6 (+0x18)
    virtual void i007(); virtual void i008(); virtual void i009(); virtual void i010(); virtual void i011(); virtual void i012();
    virtual void i013(); virtual void i014(); virtual void i015(); virtual void i016(); virtual void i017(); virtual void i018();
    virtual void i019();
    virtual bool MsgIsUsedBy(const CCharString& user) const;   // slot 20 (+0x50)
};

struct CCountedPointerImp {
    CScriptThingImp* Data;
    CCPPointerInfo*  Info;
    ~CCountedPointerImp()
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

class CBaseClass { public:
    CBaseClass();                                       // 0x0099A2D0
    virtual ~CBaseClass();                              // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerImp PImp;                            // +0x04
    virtual __forceinline ~CScriptThing() {}            // out of line: 0x004AA840
    const C3DVector& GetPos() const                     // 0x004AA980
    {
        if (!PImp.Data)
            return g_ZeroVector;
        return PImp.Data->GetPos();
    }
    bool MsgIsUsedBy(const CCharString& user) const     // 0x004AAB10
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsUsedBy(user);
    }
};

bool __fastcall IsDistanceBetweenThingsOver(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE3EA

class CGameScriptInterface { public:                          // vtable 0x01260F0C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual bool MsgIsGameInfoClickedPast() const;   // slot 40 (+0xa0)
    virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048();
    virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055(); virtual void v056();
    virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero() const;   // slot 70 (+0x118)
    virtual void v071();
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120)
    virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080();
    virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096();
    virtual void v097(); virtual void v098(); virtual void v099();
    virtual CScriptThing CreateEffect(const CCharString& effect, const C3DVector& pos, const CCharString& bone, float scale, bool a, bool b) const;   // slot 100 (+0x190)
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0)
    virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114();
    virtual void DisplayGameInfo(const CCharString& text) const;   // slot 115 (+0x1cc)
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203();
    virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211();
    virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243();
    virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267();
    virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299();
    virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307();
    virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315();
    virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323();
    virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331();
    virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338(); virtual void v339();
    virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346(); virtual void v347();
    virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354(); virtual void v355();
    virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362(); virtual void v363();
    virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370(); virtual void v371();
    virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378(); virtual void v379();
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
    virtual void v468();
    virtual void EntitySetTargetable(const CScriptThing& thing, bool targetable) const;   // slot 469 (+0x754)
};

namespace NScript {
class CQ_SunnyvaleMasterData { public:
    unsigned char _pad000[0xe8];
    long          PrisonRaceNumber;                     // +0xe8
};

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

class CQ_PrisonWardenGameScript : public CScriptBase {
public:
    CGameScriptInterface*   GSI;                        // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool                    SecretDiaryOpened;          // +0x48 (retail = PDB - 0x14)
    bool                    HeroHasSecretDiaryKey;      // +0x49
    void WardenKeyMiniGame();
};

void CQ_PrisonWardenGameScript::WardenKeyMiniGame()
{
    CScriptThing keyRack = GSI->GetThingWithScriptName("PrisonCellKeyRack");
    CScriptThing sparkles = GSI->CreateEffect("SPARKLES_SMALL", keyRack.GetPos(), "", 0.0f, false, false);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (IsDistanceBetweenThingsOver(GSI->GetHero(), keyRack, 3.0f)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetTargetable(keyRack, false);
        } else {
            if (IsActiveThreadTerminating())
                return;
            GSI->EntitySetTargetable(keyRack, true);
        }
        if (keyRack.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
            HeroHasSecretDiaryKey = true;
            GSI->EntitySetTargetable(keyRack, false);
            if (PMasterData->PrisonRaceNumber % 2 == 0) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_TWO");
                while (!GSI->MsgIsGameInfoClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            } else {
                if (IsActiveThreadTerminating())
                    return;
                GSI->DisplayGameInfo("TEXT_QST_055_DIARY_CODE_NUMBER_YEAR_ONE");
                while (!GSI->MsgIsGameInfoClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            GSI->RemoveThing(sparkles, false, true);
            return;
#pragma inline_depth()
        }
        GSI->NewScriptFrame();
    }
}
}