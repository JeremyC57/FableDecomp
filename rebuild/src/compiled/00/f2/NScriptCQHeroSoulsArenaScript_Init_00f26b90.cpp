// NScript::CQ_HeroSoulsArenaScript::Init() -- set up the Hero Souls arena quest: copy the script def's HeroSoulsRounds, clear the creature count, the seven state flags and the level reached, reset the guard-chat timer and add the "Arena" quest region
// Leaf: ?Init@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F26B90, 149 bytes, thiscall, plain `ret` (public virtual, quest vtable slot 3).
// Identity (proven): the row is referenced once, from slot 3 (Init) of the RTTI-less quest vtable 0x012D3994 (0x012D39A0),
//   which the landed CQ_HeroSoulsArenaScript ctor 0x00D7C700 and Alloc 0x00D80BA0 store (slot 1 the landed RegisterMain
//   0x00D7C740, slot 2 Main 0x00D7C890, slot 4 the landed OnPersist 0x00D7C800). Ego_r.pdb
//   ?Init@CQ_HeroSoulsArenaScript@NScript@@UAEXXZ sits at 0x00F9B570, far from the class's other members (0x00D9D3C0..
//   0x00DA2140), exactly as retail's Init sits beside the arena defs' code (0x00F26xxx). Catalog label sub_00f26b90 (gap scan).
// Callees: std::vector<CHeroSoulsRoundDef>::operator= 0x00F26C30 (landed; Ego_r ??4?$vector@VCHeroSoulsRoundDef@@...@QAEAAV01@ABV01@@Z),
//   CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; the global interface 0x0143E8F8 slot 89 SetTimer
//   (inline CTimer::operator=(long), Ego_r ??4CTimer@@QAEAAV0@J@Z); Game slot 271 AddQuestRegion (ghidra_out/cgsi_slots_aligned.tsv).
// Data: pGlobalScriptDef 0x0143E90C; "Q_HeroSoulsArena" 0x012C3318, "Arena" 0x012C780C.
// Layout: CQ_HeroSoulsArenaScript at Ego_r - 0x14 (retail CScriptBase 0x40): CreatureCount +0x48, PlayedArenaCS +0x4c,
//   PlayerHitSummoner1 +0x4d, PlayerHitSummoner2 +0x4e, PlayerAttacksGuard +0x4f, JackTauntNearHOH +0x50, FirstWaveAttack
//   +0x51, PlayedHoHCS +0x52, ArenaLevelReached +0x54, GuardChatTimer +0x58, Rounds +0x5c (the landed OnPersist agrees).
//   CScriptDef HeroSoulsRounds retail +0x1050 = Ego_r 0x1114 - 0xc4 (the chunk 576 -0xb0 after FHTimeLimit, then 4 more for each
//   of the five Dinkumware vectors GUI_MeleeGrades / GUI_SkillGrades / GUI_WillGrades / Feats / ArenaRounds before it).
// Shape: the AddQuestRegion temporaries are built right to left ("Arena" first) and destroyed in reverse.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
namespace std {
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    vector& operator=(const vector& __x);               // 0x00F26C30 for CHeroSoulsRoundDef
};
}
class CHeroSoulsWaveDef;
class CHeroSoulsRoundDef : public CDefClassBase {      // 0x38 bytes (the landed operator= stride)
public:
    virtual ~CHeroSoulsRoundDef();
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                            NumWaves;           // +0x28
    std::vector<CHeroSoulsWaveDef>  Waves;              // +0x2c
};
class CScriptDef {                                      // retail layout (Ego_r 0x1154 with Dinkumware containers)
public:
    unsigned char _before[0x1050];
    std::vector<CHeroSoulsRoundDef> HeroSoulsRounds;    // +0x1050 (Ego_r 0x1114)
};
extern CScriptDef* pGlobalScriptDef;                    // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`)
class CGameScriptInterfaceBase { public:                // slots: ghidra_out/cgsi_slots_aligned.tsv (CGameScriptInterface vtable 0x01260F0C)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164)
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
    virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270();
    virtual void AddQuestRegion(const CCharString& quest, const CCharString& region) const;   // slot 271 (+0x43c)
};
class CGameScriptInterface : public CGameScriptInterfaceBase { };
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8 (the global script interface; no PDB data symbol)
class CTimer {                                          // Ego_r.pdb: one long TimerIndex
public:
    long TimerIndex;
    CTimer& operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
};
class CPersistContext;
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();                             // slot 0
protected:
    virtual void RegisterMain();                        // slot 1
    virtual void Main();                                // slot 2
    virtual void Init();                                // slot 3
public:
    virtual void OnPersist(CPersistContext& context);   // slot 4
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_HeroSoulsArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    long                             CreatureCount;        // +0x48
    bool                             PlayedArenaCS;        // +0x4c
    bool                             PlayerHitSummoner1;   // +0x4d
    bool                             PlayerHitSummoner2;   // +0x4e
    bool                             PlayerAttacksGuard;   // +0x4f
    bool                             JackTauntNearHOH;     // +0x50
    bool                             FirstWaveAttack;      // +0x51
    bool                             PlayedHoHCS;          // +0x52
    long                             ArenaLevelReached;    // +0x54
    CTimer                           GuardChatTimer;       // +0x58
    std::vector<CHeroSoulsRoundDef>  Rounds;               // +0x5c
    virtual void Init();
};

void CQ_HeroSoulsArenaScript::Init()
{
    Rounds = pGlobalScriptDef->HeroSoulsRounds;
    CreatureCount = 0;
    PlayedArenaCS = false;
    PlayerHitSummoner1 = false;
    PlayerHitSummoner2 = false;
    PlayerAttacksGuard = false;
    JackTauntNearHOH = false;
    FirstWaveAttack = false;
    PlayedHoHCS = false;
    ArenaLevelReached = 0;
    GuardChatTimer = 0;
    Game->AddQuestRegion("Q_HeroSoulsArena", "Arena");
}
}   // namespace NScript