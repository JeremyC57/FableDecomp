// NScript::CHeroBoastsScript::BoastRansomVictimHaveVictimKilled(CCharString quest) -- boast thread: until the script ends, wait on quest `quest`; a failed quest fails boast 0x29, a completed one completes it when the master flag RansomVictimHaveVictimKilled is set and fails it otherwise (each outcome after a final termination check), else a script frame
// Leaf: ?BoastRansomVictimHaveVictimKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE67D0, 196 bytes (208 with int3 padding), thiscall `ret 4` (the quest name by
//   value, destroyed on every exit).
// Identity (proven): NScript::CHeroBoastsScript::BoastRansomVictimHaveVictimKilled; evidence: CHeroBoastsScript's spawn of this body (0x00CE39CA:
//   `mov [edi+34h], 0x00CE67D0` in the CSpawnedFunc2 construction) passes the literal "BoastRansomVictimHaveVictimKilled"; Ego_r.pdb has
//   ?BoastRansomVictimHaveVictimKilled@CHeroBoastsScript@NScript@@QAEXVCCharString@@@Z, and the master flag the body reads (+0x12D) is
//   Ego_r.pdb NScript::CQ_SunnyvaleMasterData::RansomVictimHaveVictimKilled (offsets identical in retail).
// Label: the manifest's "WatchForRegionLoads" is propagated across family H034 (seven boast threads).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940 (Ego_r.pdb ?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ),
//   CCharString::~CCharString 0x0099EAE0, and the script interface virtuals (retail vtable 0x01260F0C, slot names from
//   ghidra_out/cgsi_slots_aligned.tsv): NewScriptFrame (7), MsgOnQuestCompletedBeforeScreenShown (28),
//   MsgOnQuestFailedBeforeScreenShown (29), SetBoastAsFailed (305), SetBoastAsCompleted (306).
// Layout: Ego_r.pdb CLevelScriptBase<NScript::CQ_SunnyvaleMasterData> {CGameScriptInterfaceBase* Game,
//   CQ_SunnyvaleMasterData* MasterScriptData} after CScriptBase, retail +0x40 / +0x44 (CScriptBase is 0x40 bytes in retail:
//   the PActiveThread IsActiveThreadTerminating reads is +0x2C). Local models -- Review at landing.
// Shape: completed-and-flag is one condition, so the completed query is made a second time on the fall-through; the two
//   "fail the boast" exits share one block (retail tail-merges them).
class CCharString {
public:
    void* PData;
    CCharString(const CCharString& other);
    ~CCharString();                                     // 0x0099EAE0
};

class CGameScriptInterfaceBase {                        // virtual slots 0..306 of the script interface; vNNN are slots not used here
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;                                                  // slot 7 (+0x1C)
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027();
    virtual bool MsgOnQuestCompletedBeforeScreenShown(const CCharString& quest) const;   // slot 28 (+0x70)
    virtual bool MsgOnQuestFailedBeforeScreenShown(const CCharString& quest) const;      // slot 29 (+0x74)
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037();
    virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045();
    virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061();
    virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual void v070(); virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085();
    virtual void v086(); virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093();
    virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101();
    virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109();
    virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117();
    virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125();
    virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133();
    virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141();
    virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149();
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165();
    virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173();
    virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181();
    virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197();
    virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205();
    virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213();
    virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221();
    virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229();
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237();
    virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245();
    virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253();
    virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261();
    virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269();
    virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277();
    virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285();
    virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293();
    virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301();
    virtual void v302(); virtual void v303(); virtual void v304();
    virtual void SetBoastAsFailed(long boast, const CCharString& quest) const;           // slot 305 (+0x4C4)
    virtual void SetBoastAsCompleted(long boast, const CCharString& quest) const;        // slot 306 (+0x4C8)
};

class CScriptBase {
public:
    unsigned char _pad00[0x40];                         // vptr, Threads, EntityScripts, EntityScriptBindings, LastWorldUpdateFrame,
                                                        // PActiveThread (+0x2C), RuntimeScriptedEntityPersist
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

namespace NScript {
class CQ_SunnyvaleMasterData {
public:
    unsigned char _pad000[0x12D];
    bool          RansomVictimHaveVictimKilled;                             // +0x12D
};
}

template <class TMasterData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TMasterData*              MasterScriptData;         // +0x44
};

namespace NScript {
class CHeroBoastsScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    void BoastRansomVictimHaveVictimKilled(CCharString quest);
};
}

void NScript::CHeroBoastsScript::BoastRansomVictimHaveVictimKilled(CCharString quest)
{
    while (!IsActiveThreadTerminating()) {
        if (Game->MsgOnQuestFailedBeforeScreenShown(quest)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(0x29, quest);
            return;
        }
        if (Game->MsgOnQuestCompletedBeforeScreenShown(quest) && MasterScriptData->RansomVictimHaveVictimKilled) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsCompleted(0x29, quest);
            return;
        }
        if (Game->MsgOnQuestCompletedBeforeScreenShown(quest)) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetBoastAsFailed(0x29, quest);
            return;
        }
        Game->NewScriptFrame();
    }
}