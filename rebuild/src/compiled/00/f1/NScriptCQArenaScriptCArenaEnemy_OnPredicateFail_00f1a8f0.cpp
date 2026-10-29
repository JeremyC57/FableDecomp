// NScript::CQ_ArenaScript::CArenaEnemy::OnPredicateFail() -- an arena enemy left the fight: one fewer of its creature type alive; if the hero killed it, add the current round/wave creature def's DeathScore to the crowd points; drop its quest-info health bar if it had one
// Leaf: ?OnPredicateFail@CArenaEnemy@CQ_ArenaScript@NScript@@EAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F1A8F0, 147 bytes, thiscall, plain `ret` (private virtual, entity vtable slot 5).
// Identity (proven): the row is referenced once, from slot 5 (OnPredicateFail) of the RTTI-less entity vtable 0x012F9C50
//   (0x012F9C64), which the landed NScript::CQ_ArenaScript::CArenaEnemy::Alloc 0x00F1A810 stores (slot 1 = the landed
//   CArenaEnemy Main 0x00F1A990; slots 4 / 6 the CEntityScriptBase defaults 0x00CDEBC0 / 0x00CDEBE0). Ego_r.pdb gives
//   CArenaEnemy exactly OnPredicateFail, Alloc and Main: ?OnPredicateFail@CArenaEnemy@CQ_ArenaScript@NScript@@EAEXXZ
//   (private virtual). Entity slot order (landed CTyler::Init 0x00E836F0): 0 dtor, 1 Main, 2 Init, 3 GetParentScript,
//   4 OnPersist, 5 OnPredicateFail, 6 OnInterrupted. Catalog label sub_00f1a8f0 (gap scan).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; CScriptThing slot 18 MsgIsKilledBy (+0x48);
//   CGameScriptInterface slot 338 RemoveQuestInfoElement (+0x548, ghidra_out/cgsi_slots_aligned.tsv).
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8.
// Layout: Ego_r.pdb CQ_ArenaScript::CArenaEnemy (40 bytes = the Alloc's new size): ParentClass +0x14, PMasterData +0x18,
//   HealthBarIndex +0x1c, BigCreature +0x20, MyCreatureType +0x24. CQ_ArenaScript at Ego_r - 0x14 (retail CScriptBase 0x40),
//   less 4 after the Dinkumware Rounds vector: Rounds +0x98, ArenaRound +0xa8, ArenaRoundWave +0xac, TotalCreatures +0xd0,
//   NewCrowdPoints +0xf4. Def elements are the retail polymorphic chain (FABLE_RETAIL_DEF_CLASS_BASE): CArenaRoundDef 0x38
//   (Waves +0x2c), CArenaWaveDef 0x3c (Creatures +0x2c), CArenaCreatureDef 0x38 (DeathScore +0x34) -- the strides retail uses.
// Shape: STLport `operator[]` as `*(begin() + n)`; the bool result kept in bl across the temporary's destructor.
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
    typedef _Tp* iterator;
    typedef _Tp& reference;
    iterator begin() { return this->_M_start; }
    reference operator[](unsigned int __n) { return *(begin() + __n); }
};
}
class CArenaCreatureDef : public CDefClassBase {       // vtable 0x012FB358, 0x38 bytes
public:
    virtual ~CArenaCreatureDef();
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F25E70
    virtual void Copy(const CDefClassBase* other);      // slot 19
    virtual unsigned long GetSizeofClass();             // slot 20
    CCharString CreatureType;                           // +0x28
    long        NumCreatures;                           // +0x2c
    CCharString HUDType;                                // +0x30
    long        DeathScore;                             // +0x34
};
class CArenaWaveDef : public CDefClassBase {           // vtable 0x012FB2F0, 0x3c bytes
public:
    virtual ~CArenaWaveDef();
    virtual void Transfer(CPersistContext& context);    // slot 18 0x00F261F0
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                           NumWaveCreatures;    // +0x28
    std::vector<CArenaCreatureDef> Creatures;           // +0x2c
    bool                           ShortWave;           // +0x38
};
class CArenaRoundDef : public CDefClassBase {          // 0x38 bytes
public:
    virtual ~CArenaRoundDef();
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
    long                        NumWaves;               // +0x28
    std::vector<CArenaWaveDef>  Waves;                  // +0x2c
};

#include "engine/CBaseClass.h"                         // vtable 0x01231710, ~CBaseClass 0x0099A2E0
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
};
class CScriptThing : public CBaseClass {                // 12 bytes, vtable 0x01238C8C
public:
    virtual ~CScriptThing();                            // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48)
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
};
class CGameScriptInterfaceBase { public:                // the script interface (slots: ghidra_out/cgsi_slots_aligned.tsv, CGameScriptInterface vtable 0x01260F0C)
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
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119();
    virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127();
    virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135();
    virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143();
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151();
    virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159();
    virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167();
    virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175();
    virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183();
    virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191();
    virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199();
    virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207();
    virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215();
    virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223();
    virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231();
    virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239();
    virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247();
    virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255();
    virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267(); virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271();
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279();
    virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287();
    virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303();
    virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311();
    virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319();
    virtual void v320(); virtual void v321(); virtual void v322(); virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327();
    virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335();
    virtual void v336(); virtual void v337();
    virtual void RemoveQuestInfoElement(long element) const;   // slot 338 (+0x548)
};
class CScriptBase;
class CPersistContext;
class CEntityScriptBase {                               // Ego_r.pdb: vptr, Game +0x04, Me +0x08 (20 bytes)
public:
    virtual ~CEntityScriptBase();                       // slot 0
    virtual void Main() = 0;                            // slot 1
    virtual void Init();                                // slot 2
    virtual CScriptBase* GetParentScript() const = 0;   // slot 3
    virtual void OnPersist(CPersistContext& context);   // slot 4
    virtual void OnPredicateFail();                     // slot 5
    virtual void OnInterrupted();                       // slot 6
    CGameScriptInterfaceBase* Game;                     // +0x04
    CScriptThing          Me;                           // +0x08
};
class CScriptBase {                                     // retail 0x40 bytes
public:
    virtual ~CScriptBase();
    unsigned char _body[0x3c];                          // +0x04 .. +0x40
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;                     // +0x40
    TData* MasterScriptData;                            // +0x44
};

namespace NScript {
class CQ_SunnyvaleMasterData;
class CQ_ArenaScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    CCharString                  CrowdLoopTags[4][5];   // +0x48
    std::vector<CArenaRoundDef>  Rounds;                // +0x98
    long                         ArenaState;            // +0xa4
    long                         ArenaRound;            // +0xa8
    long                         ArenaRoundWave;        // +0xac
    long                         GoldMultiplier;        // +0xb0
    bool                         ChamLeaving;           // +0xb4
    long                         GoldTotal;             // +0xb8
    bool                         InHitCutsceneAlready;  // +0xbc
    bool                         NeedBertForSpeech;     // +0xbd
    CScriptThing                 CellsVillage;          // +0xc0
    bool                         PlayerLeaving;         // +0xcc
    bool                         PlayerWon;             // +0xcd
    long                         TotalCreatures[3];     // +0xd0
    long                         ExtraCreatures;        // +0xdc
    bool                         MissionSucceeded;      // +0xe0
    bool                         MissionFailed;         // +0xe1
    bool                         ArenaSpawnNeeded[16];  // +0xe2
    long                         NewCrowdPoints;        // +0xf4
    class CArenaEnemy;
};
class CQ_ArenaScript::CArenaEnemy : public CEntityScriptBase {
public:
    CQ_ArenaScript*          ParentClass;               // +0x14
    CQ_SunnyvaleMasterData*  PMasterData;               // +0x18
    long                     HealthBarIndex;            // +0x1c
    bool                     BigCreature;               // +0x20
    long                     MyCreatureType;            // +0x24
private:
    virtual void OnPredicateFail();
};

void CQ_ArenaScript::CArenaEnemy::OnPredicateFail()
{
    --ParentClass->TotalCreatures[MyCreatureType];
    if (Me.MsgIsKilledBy("SCRIPT_NAME_HERO"))
        ParentClass->NewCrowdPoints += ParentClass->Rounds[ParentClass->ArenaRound].Waves[ParentClass->ArenaRoundWave].Creatures[MyCreatureType].DeathScore;
    if (BigCreature)
        Game->RemoveQuestInfoElement(HealthBarIndex);
}
}   // namespace NScript