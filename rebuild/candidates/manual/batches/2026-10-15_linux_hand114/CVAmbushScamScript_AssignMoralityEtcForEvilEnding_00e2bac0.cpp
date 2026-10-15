// CV_AmbushScamScript::AssignMoralityEtcForEvilEnding() -- give the hero the ambush-scam good morality, renown and experience from the script def
// Retail: Fable.exe @ 0x00E2BAC0, 77 bytes (+ int3 padding). Called once from the quest's ending code.
// Callees (virtual, CGameScriptInterface through Game at +0x40): GiveHeroMorality(float) slot 156 (+0x270),
// GiveHeroRenownPoints slot 161 (+0x284), GiveHeroExperience slot 128 (+0x200).
// Data: the script def @ 0x0143E90C. Its retail AmbushScam block sits 4 bytes below Ego_r.pdb's CScriptDef offsets:
// Experience +0x2a8, Renown +0x2ac, MoralityEvil +0x2b0, MoralityGood +0x2b4 -- this body reads +0x2b4 (MoralityGood).
// Label note: with the good-morality value this is the good-ending twin of 0x00E2BB10; the catalog name is shared.
// Shape: each argument is read through the def global and pushed as a dword (a float lvalue argument is copied with
// integer moves); the interface pointer is re-read from +0x40 for each call.
struct CGameScriptInterface {                       // retail vtable @ 0x01260F0C
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
    virtual void GiveHeroExperience(long amount) const;    // slot 128 (+0x200)
    virtual void v129(); virtual void v130(); virtual void v131(); virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136();
    virtual void v137(); virtual void v138(); virtual void v139(); virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144();
    virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152();
    virtual void v153(); virtual void v154(); virtual void v155();
    virtual void GiveHeroMorality(float amount) const;     // slot 156 (+0x270)
    virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160();
    virtual void GiveHeroRenownPoints(long amount) const;  // slot 161 (+0x284)
};

struct CScriptDef {
    unsigned char _pad[0x2a8];
    long AmbushScamExperience;                           // +0x2a8
    long AmbushScamRenown;                               // +0x2ac
    float AmbushScamMoralityEvil;                        // +0x2b0
    float AmbushScamMoralityGood;                        // +0x2b4
};
extern CScriptDef* g_pScriptDef;                         // 0x0143E90C

struct CV_AmbushScamScript {
    unsigned char _base[0x40];
    CGameScriptInterface* Game;                          // +0x40
    void AssignMoralityEtcForEvilEnding();
};

void CV_AmbushScamScript::AssignMoralityEtcForEvilEnding()
{
    Game->GiveHeroMorality(g_pScriptDef->AmbushScamMoralityGood);
    Game->GiveHeroRenownPoints(g_pScriptDef->AmbushScamRenown);
    Game->GiveHeroExperience(g_pScriptDef->AmbushScamExperience);
}
