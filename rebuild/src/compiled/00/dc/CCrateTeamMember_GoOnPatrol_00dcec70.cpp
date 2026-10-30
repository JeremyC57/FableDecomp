// CCrateTeamMember::GoOnPatrol() -- an idle member (state 0) picks a patrol spot through the script interface (3 units out when its team is the parent script's current team, 1 unit otherwise, where the move flag is also cleared) unless it is out of action, then moves itself from its team's state-0 counter to state 1; a member already in another state only re-checks that it is out of action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCEC70, length 157 bytes (+ int3 padding).
// Callees: CEntityScriptBase::IsActiveThreadTerminating @ 0x00F35B30 (bool: virtual slot 3's thing through 0x00CB7940; the manifest
// "CCreatureAction_TrollWhackGroundBase::Initialise" label is propagated; a `jmp` tail call on the non-idle path), the script
// interface at +4: virtual slot 70 (+0x118: float distance, bool flag -> a position handle), slot 229 (+0x394: a script thing and
// the handle) and slot 238 (+0x3b8: a script thing and 0).
// Shape: the parent's team word (+0x78) compared against TeamID selects the branch; in each branch the nested
// `script->SetPatrolTarget(&m_thing, script->FindPatrolPosition(d, true))` fetches the script's vtable once (reused across the inner
// call, the member reloaded after it); the else branch keeps the thing address in ebx for its third call; then the state
// counters (`dec [team+state*4]`, the state store, `inc [team+4]` with the team reloaded after the store).
// Layout: Ego_r.pdb CCrateTeamMember (60 bytes): vptr +0, the script interface +4 (unnamed), a CScriptThing +8 (unnamed),
// ParentClass +0x14, PMasterData +0x18, CurrentAIState +0x1c, PreviousAIState +0x20, TeamID +0x24, MemberState +0x28, MyTeam +0x2c,
// ThingToPatrolTo +0x30; CCrateTeamManager (landed 0x00CDB360): StateCounter[6] at +0.
namespace NScript { class CPredicateBase; }
class CCPPointerInfo;
#include "engine/CCountedPointer.h"
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
struct CCrateTeamManager {
    long StateCounter[6];                   // +0x00
    long MemberCount;                       // +0x18
};
struct CQ_OrchardFarmRaidScript {
    unsigned char _pad[0x78];
    long m_currentTeamID;                   // +0x78
};
struct CScriptInterface {                   // the game script API object at +4: only three of its slots are used here
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005();
    virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017();
    virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035();
    virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039(); virtual void v040(); virtual void v041();
    virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053();
    virtual void v054(); virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064(); virtual void v065();
    virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual long FindPatrolPosition(float distance, bool flag);   // slot 70 (+0x118)
    virtual void v071(); virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076();
    virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082();
    virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094();
    virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100();
    virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106();
    virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112();
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118();
    virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124();
    virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130();
    virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142();
    virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154();
    virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166();
    virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172();
    virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178();
    virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190();
    virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196();
    virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202();
    virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
    virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214();
    virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220();
    virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226();
    virtual void v227(); virtual void v228();
    virtual void SetPatrolTarget(CScriptThing* thing, long position);   // slot 229 (+0x394)
    virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237();
    virtual void SetThingMoveFlag(CScriptThing* thing, long flag);      // slot 238 (+0x3b8)
};

class CEntityScriptBase
{
public:
    virtual void v00();
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

struct CCrateTeamMember : public CEntityScriptBase {                   // Ego_r.pdb: 60 bytes
    CScriptInterface*          m_pScript;        // +0x04
    CScriptThing               m_thing;          // +0x08: this member's own thing
    CQ_OrchardFarmRaidScript*  ParentClass;      // +0x14
    void*                      PMasterData;      // +0x18
    long                       CurrentAIState;   // +0x1c
    long                       PreviousAIState;  // +0x20
    long                       TeamID;           // +0x24
    long                       MemberState;      // +0x28
    CCrateTeamManager*         MyTeam;           // +0x2c
    CScriptThing               ThingToPatrolTo;  // +0x30
    void GoOnPatrol();
};

void CCrateTeamMember::GoOnPatrol()
{
    if (MemberState != 0) {
        IsActiveThreadTerminating();                    // a non-idle member only re-checks itself (the tail call)
        return;
    }
    if (TeamID == ParentClass->m_currentTeamID) {
        if (IsActiveThreadTerminating())
            return;
        m_pScript->SetPatrolTarget(&m_thing, m_pScript->FindPatrolPosition(3.0f, true));
    } else {
        if (IsActiveThreadTerminating())
            return;
        m_pScript->SetPatrolTarget(&m_thing, m_pScript->FindPatrolPosition(1.0f, true));
        m_pScript->SetThingMoveFlag(&m_thing, 0);
    }
    --MyTeam->StateCounter[MemberState];
    MemberState = 1;
    ++MyTeam->StateCounter[1];
}