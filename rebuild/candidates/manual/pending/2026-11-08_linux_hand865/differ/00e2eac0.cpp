// Grade: DIFFER(361v363) 27 diff lines (parked 304v363, 89 lines): ~CScriptThing __forceinline with the distance path's terminate return in a `#pragma inline_depth(0)` region expands the destructor at exactly retail's three sites and calls 0x004AA840 at the fourth; left only retail's dead zero dword at the frame base (frame 0x10 v 0x0c, `mov [esp+0Ch],edi` before GetTrickster, edi the zero register from entry) -- every frame offset shifts by 4
// CV_AmbushScamScript::TricksterApproach() -- wait, one script frame at a time, until the trickster is dead, out of health or within TricksterApproachProximity of the hero; then flag DoTricksterIntro (he arrived) or TricksterKilledBeforeAmbush
// Leaf: TricksterApproach
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E2EAC0, 368 bytes.
// Callees: CV_AmbushScamScript::GetTrickster @ 0x00E2EC30 (CScriptThing by value), CScriptBase::IsActiveThreadTerminating
//          @ 0x00CB7940, IsDistanceBetweenThingsUnder(const CScriptThing&, const CScriptThing&, float) @ 0x00CBE2FF
//          (`__fastcall`), ~CScriptThing @ 0x004AA840 (once out of line) / its inline expansion (counted release, operator
//          delete @ 0x00BFE9BC, ~CBaseClass @ 0x0099A2E0); CScriptThing::IsAlive (inline; the game-side thing's slot 75,
//          +0x12c); CGameScriptInterface slots 7 NewScriptFrame (+0x1c), 70 GetHero (+0x118), 264 GetHealth (+0x420);
//          float pool 0x0129BA3C = 1e-4f.
// Layout: Ego_r.pdb CV_AmbushScamScript - 0x14: DoTricksterIntro +0x4e, TricksterKilledBeforeAmbush +0x57,
//          TricksterApproachProximity +0x5c; the CScriptThing model is the landed CQ_HangingTreeGoodScript one (0x00D74F80).
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"




struct CBaseClass {                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};

struct CScriptThing : CBaseClass {                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    __forceinline ~CScriptThing() {}                     // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const; virtual void v18() const; virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const;
    virtual void v25() const; virtual void v26() const; virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const;
    virtual void v33() const; virtual void v34() const; virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const;
    virtual void v41() const; virtual void v42() const; virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const;
    virtual void v49() const; virtual void v50() const; virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const;
    virtual void v57() const; virtual void v58() const; virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const;
    virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const;
    virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
struct CGameScriptInterface {   // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void v087(); virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102();
    virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110();
    virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126();
    virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134();
    virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150();
    virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158();
    virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174();
    virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182();
    virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198();
    virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206();
    virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222();
    virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230();
    virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238();
    virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246();
    virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254();
    virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262();
    virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};
bool __fastcall IsDistanceBetweenThingsUnder(const CScriptThing& a, const CScriptThing& b, float distance);   // 0x00CBE2FF
struct CScriptBase {                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

struct CV_AmbushScamScript : CQuestScriptBase {
    bool QuestFinished;                                  // +0x48
    bool QuestComplete;                                  // +0x49
    bool QuestActivated;                                 // +0x4a
    bool LeftRegion;                                     // +0x4b
    bool MoralitySwitch;                                 // +0x4c
    bool BuddyIntroComplete;                             // +0x4d
    bool DoTricksterIntro;                               // +0x4e
    bool DoAmbush;                                       // +0x4f
    bool TricksterApproachComplete;                      // +0x50
    bool TricksterSpeechComplete;                        // +0x51
    bool TricksterRetreatComplete;                       // +0x52
    bool TricksterQuestComplete;                         // +0x53
    bool TricksterWhiningComplete;                       // +0x54
    bool TricksterDeadOrLeft;                            // +0x55
    bool TricksterComplete;                              // +0x56
    bool TricksterKilledBeforeAmbush;                    // +0x57
    bool TricksterKilledAfterAmbush;                     // +0x58
    bool TricksterKilledAfterBeingSpared;                // +0x59
    bool TricksterResetAllegience;                       // +0x5a
    bool TricksterWaitHere;                              // +0x5b
    float TricksterApproachProximity;                    // +0x5c
    CScriptThing GetTrickster();                         // 0x00E2EC30
    void TricksterApproach();
};

void CV_AmbushScamScript::TricksterApproach()
{
    CScriptThing trickster = GetTrickster();
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (!trickster.IsAlive() || GSI->GetHealth(trickster) <= 1e-4f) {
            if (IsActiveThreadTerminating())
                return;
            TricksterKilledBeforeAmbush = true;
            break;
        }
        if (IsDistanceBetweenThingsUnder(trickster, GSI->GetHero(), TricksterApproachProximity)) {
#pragma inline_depth(0)
            if (IsActiveThreadTerminating())
                return;
#pragma inline_depth()
            DoTricksterIntro = true;
            break;
        }
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
}
