// CV_BeardyBaldyScript::IsHeroWearingAnyTash() -- true when the hero wears any of the six moustaches (the hairstyle slot holds the tash)
// Leaf: IsHeroWearingAnyTash
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E538F0, 478 bytes (480 with padding), plain `ret`.
// Label: the manifest calls this CV_BordelloScript::IsHeroWearingTash (a byte twin of 0x00E44CC0); its callers 0x00E526A6 and
//   0x00E52FFD are CV_BeardyBaldyScript code and FableWin.pdb orders IsHeroWearingAnyBeard, IsHeroWearingAnyTash,
//   IsHeroWearingAnyOddHairdo (0x00E53AD0) -- this is CV_BeardyBaldyScript::IsHeroWearingAnyTash.
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0; through the interface (+0x40)
//   GetHero (slot 70, +0x118) and IsWearingHairstyle (slot 193, +0x304).
// Strings: "OBJECT_HERO_TASHSMITH_01" 0x012DC788, "OBJECT_HERO_TASHTRADER_01" 0x012E49A8, "OBJECT_HERO_TASHKHG_01" 0x012E4990, "OBJECT_HERO_TASHSHERIFF_01" 0x012E4974, "OBJECT_HERO_TASHCHINESE_01" 0x012E4958, "OBJECT_HERO_TASHSMALL_01" 0x012E493C.
// Shape: an `||` chain of implicit string conversions into a named bool: VC7.1 keeps one construction-flag bit per
//   conditional temporary in ebx (mov ebx,1/3/7/0Fh/1Fh/3Fh) and destroys the built ones after the chain.
// Layout: interface pointer at +0x40 (CQuestScriptBase, landed relatives). Local model: the generated CGameScriptInterface.h
//   carries no virtual slots.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing;
class CGameScriptInterface { public:                   // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069();
    virtual CScriptThing& GetHero();                                               // slot 70 (+0x118)
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
    virtual void v191(); virtual void v192();
    virtual bool IsWearingHairstyle(CScriptThing& thing, const CCharString& hairstyle) const; // slot 193 (+0x304)
};
namespace NScript {
class CV_BeardyBaldyScript {
public:
    unsigned char _base[0x40];                              // CScriptBase
    CGameScriptInterface* Game;                             // +0x40
    bool IsHeroWearingAnyTash();
};
} // namespace NScript
using NScript::CV_BeardyBaldyScript;
bool NScript::CV_BeardyBaldyScript::IsHeroWearingAnyTash()
{
    bool wearing = Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMITH_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHTRADER_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHKHG_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSHERIFF_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHCHINESE_01")
        || Game->IsWearingHairstyle(Game->GetHero(), "OBJECT_HERO_TASHSMALL_01");
    return wearing;
}