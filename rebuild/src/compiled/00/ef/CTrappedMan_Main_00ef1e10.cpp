// CTrappedMan::Main() -- the trapped man on the hill: register an alive filter over Me, run a frame; then, until the script is done, wait for the hero to hit Me, show "You hit the man on the hill!" and, when his health is below 1e-4, "You hit him to the point of death!", once per script frame
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EF1E10, 384 bytes (entity-script vtable slot 1 = Main).
// Callees: CScriptThing(const CScriptThing&) @ 0x004ABE90 (filter member; filter vtable 0x012C32F0), the filter registration
//          @ 0x00F35B10, the filter's out-of-line dtor @ 0x00E241B0, IsDone @ 0x00F35B30, CCharString(const char*, long)
//          @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CScriptThing::MsgIsHitBy (slot 21, +0x54), CGameScriptInterface slots
//          7 NewScriptFrame (+0x1c), 112 AddScreenMessage (+0x1c0) and 264 GetHealth (+0x420); float pool 0x0129BA3C = 1e-4f.
// Layout: Ego_r.pdb CTrappedMan (28 bytes): CEntityScriptBase {vptr, Game +4, CScriptThing Me +8}, ParentClass +0x14,
//          PMasterData +0x18.
// Shape: the landed CHHUndead::Main filter block; `WAIT_UNTIL(Me.MsgIsHitBy("SCRIPT_NAME_HERO"))` is a `while (!x)` whose
//          neg/sbb/inc bool survives the string temporary's dtor (VC duplicates the test at the loop bottom); the outer
//          loop is a rotated `while (!IsDone())`.
struct CCharString {
    void* m_pStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CScriptThing {   // 12 bytes: vptr + counted implementation (vtable 0x01238C8C)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual bool MsgIsHitBy(const CCharString& hitter) const;   // slot 21 (+0x54)
    unsigned char _body[8];
    CScriptThing(const CScriptThing& other);       // 0x004ABE90
    ~CScriptThing();                               // 0x004AA840
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
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079();
    virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083(); virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087();
    virtual void v088(); virtual void v089(); virtual void v090(); virtual void v091(); virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095();
    virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099(); virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103();
    virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107(); virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111();
    virtual void AddScreenMessage(const CCharString& text, long selection) const;   // slot 112 (+0x1c0)
    virtual void v113(); virtual void v114(); virtual void v115(); virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120();
    virtual void v121(); virtual void v122(); virtual void v123(); virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128();
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168();
    virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176();
    virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184();
    virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192();
    virtual void v193(); virtual void v194(); virtual void v195(); virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200();
    virtual void v201(); virtual void v202(); virtual void v203(); virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208();
    virtual void v209(); virtual void v210(); virtual void v211(); virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216();
    virtual void v217(); virtual void v218(); virtual void v219(); virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224();
    virtual void v225(); virtual void v226(); virtual void v227(); virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232();
    virtual void v233(); virtual void v234(); virtual void v235(); virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240();
    virtual void v241(); virtual void v242(); virtual void v243(); virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248();
    virtual void v249(); virtual void v250(); virtual void v251(); virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256();
    virtual void v257(); virtual void v258(); virtual void v259(); virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263();
    virtual float GetHealth(const CScriptThing& thing) const;   // slot 264 (+0x420)
};
struct CIsThingAlive {                             // vtable 0x012C32F0: a filter holding a script thing
    virtual void v00();
    CScriptThing m_thing;                          // +0x04
    CIsThingAlive(const CScriptThing& thing) : m_thing(thing) { }
    ~CIsThingAlive();                              // 0x00E241B0 (out of line in this module)
};
struct CPersonalScript_EmptyScript;
struct CEmptyData;

class CTrappedMan {
public:
    void*                        __vftable;        // +0x00
    CGameScriptInterface*        Game;             // +0x04
    CScriptThing                 Me;               // +0x08
    CPersonalScript_EmptyScript* ParentClass;      // +0x14
    CEmptyData*                  PMasterData;      // +0x18
    void RegisterFilter(CIsThingAlive& filter);    // 0x00F35B10
    bool IsDone();                                 // 0x00F35B30
    void Main();
};

void CTrappedMan::Main()
{
    {
        CIsThingAlive filter(Me);
        RegisterFilter(filter);
    }
    Game->NewScriptFrame();
    if (IsDone())
        return;
    while (!IsDone()) {
        while (!Me.MsgIsHitBy("SCRIPT_NAME_HERO")) {
            Game->NewScriptFrame();
            if (IsDone())
                return;
        }
        if (IsDone())
            return;
        Game->AddScreenMessage("You hit the man on the hill!", 0);
        if (Game->GetHealth(Me) < 1e-4f) {
            if (IsDone())
                return;
            Game->AddScreenMessage("You hit him to the point of death!", 0);
        }
        Game->NewScriptFrame();
    }
}