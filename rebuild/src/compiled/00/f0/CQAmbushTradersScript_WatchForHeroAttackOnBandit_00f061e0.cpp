// CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit) -- watch thread: frame by frame, whenever the hero hits the hired bandit (by weapon or an aggressive special ability) and the bandit still has health, flag the attack and count it
// Levers: ~CScriptThing __forceinline; CCountedPointer dtor zero order `Data = 0; Info = 0;`; for(;;) with in-body `#pragma inline_depth(0)` around the third terminating return (after the health test), the one exit retail calls 0x004AA840 at
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F061E0, length 395 bytes (+ int3 padding), `ret 0Ch` (the 12-byte
// CScriptThing is passed by value and destroyed here).
// Identity: Ego_r.pdb ?WatchForHeroAttackOnBandit@CQ_AmbushTradersScript@NScript@@QAEXVCScriptThing@@@Z (spawned as
// "WatchForHeroAttackOnBandit", string 0x012F7ED8, at 0x00F0604E).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0; the thing's implementation object slots 21 MsgIsHitBy (+0x54) and 43
// MsgIsHitByAnyAggressiveSpecialAbilityFrom (+0xAC) through inline CScriptThing virtuals (vtable 0x01238C8C); the
// by-value destructor: inline (operator delete 0x00BFE9BC, CBaseClass::~CBaseClass 0x0099A2E0) at two exits and the
// out-of-line ~CScriptThing 0x004AA840 at the third; through the quest's interface (+0x40): NewScriptFrame (slot 7),
// GetHealth (264).
// Data: "SCRIPT_NAME_HERO" 0x0125D1C8, 0.0001f 0x0129BA3C.
// Layout: retail = Ego_r.pdb - 0x14: BanditAttackedByHero +0x90, BanditAttackByHeroCount +0x94.
// Shape: the landed CQ_BountyHuntScript::WatchForLeaderKilled (0x00D17090) by-value CScriptThing model; the hit test
// is a named bool over one `||` chain whose operands each hold a "SCRIPT_NAME_HERO" conversion temporary (flags 1/2
// in ebx).
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CCPPointerInfo {                                 // counted-pointer block (Ego_r.pdb)
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

struct CBaseClass {
    virtual ~CBaseClass();                              // 0x0099A2E0
};

struct CScriptThing;
struct CCountedPointerScriptThing {                     // CCountedPointer<CScriptThing>
    CScriptThing*   Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    ~CCountedPointerScriptThing()
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

struct CScriptThing : CBaseClass {                      // 12 bytes, vtable 0x01238C8C
    CCountedPointerScriptThing PImp;                    // +0x04
    virtual __forceinline ~CScriptThing() {}                          // out of line: 0x004AA840
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& attacker) const     // slot 21 (+0x54)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsHitBy(attacker);
        return false;
    }
    virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26();
    virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual bool MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString& attacker) const     // slot 43 (+0xac)
    {
        if (PImp.Data)
            return PImp.Data->MsgIsHitByAnyAggressiveSpecialAbilityFrom(attacker);
        return false;
    }
};

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
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};

namespace NScript {
struct CScriptBase {                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};

struct CQ_AmbushTradersScript : CScriptBase {
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    unsigned char         _pad48[0x90 - 0x48];
    bool                  BanditAttackedByHero;         // +0x90 (PDB +0xa4)
    long                  BanditAttackByHeroCount;      // +0x94 (PDB +0xa8)
    void WatchForHeroAttackOnBandit(CScriptThing bandit);
};

void CQ_AmbushTradersScript::WatchForHeroAttackOnBandit(CScriptThing bandit)
{
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        bool attacked = bandit.MsgIsHitBy("SCRIPT_NAME_HERO") ||
                        bandit.MsgIsHitByAnyAggressiveSpecialAbilityFrom("SCRIPT_NAME_HERO");
        if (attacked) {
            if (IsActiveThreadTerminating())
                return;
            if (GSI->GetHealth(bandit) > 0.0001f) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                BanditAttackedByHero = true;
                ++BanditAttackByHeroCount;
            }
        }
        GSI->NewScriptFrame();
    }
}
}