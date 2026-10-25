// CGameflowAssistanceScript::Tutorial_CombatMultiplier() -- while assistance runs and the player wields a weapon, have the guildmaster comment when the hero's combat multiplier reaches a new high (5 progress, 10 adept, 15 excellent, past the def's master level: master and stop) or stays unimproved too long (try)
// Leaf: ?Tutorial_CombatMultiplier@CGameflowAssistanceScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CEFCC0, length 752 bytes (+ int3 padding; plain `ret`). The catalog's
//   "Tutorial_LowHealth" label is wrong: the spawner at 0x00CEFAE0 builds this thread under the name
//   "Tutorial_CombatMultiplier" (0x012C5E58), and the body only touches the CombatMult* members.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0; CGameScriptInterface (at +0x40) slots 7 NewScriptFrame (+0x1c), 149 GetPlayerCreatureCombatMultiplier
//   (+0x254), 195 IsPlayerWieldingWeapon (+0x30c), 274 HeroReceiveMessageFromGuildMaster (+0x448), 394
//   IsHeroControlledByPlayer (+0x628) (names from ghidra_out/cgsi_slots_aligned.tsv).
// Data: the script def 0x0143E90C (+0xfd8 frames-per-level allowance before the "try" nag, +0xfdc master level; no PDB
//   name found at those retail offsets), "" 0x0122D70E, "TEXT_QST_080_MULT_EXCELLENT" 0x012C5EDC, "..._ADEPT" 0x012C5EC4,
//   "..._PROGRESS" 0x012C5EA8, "..._TRY" 0x012C5E90, "..._MASTER" 0x012C5E74.
// Layout: retail = Ego_r.pdb CGameflowAssistanceScript - 0x14 (as the landed quest scripts): the interface +0x40, the
//   master data +0x44 (its stop flag at +0x67), CombatMultHighestLevel +0x50, CombatMultFramesSinceChange +0x54.
// Shape: the outer loop is NOT rotated (a top `jmp` into the flag test): write it `for (;;) { if (flag) break; ... }` with the
//   bottom as `if (!IsActiveThreadTerminating()) continue; return;` (a plain `while (!flag)` or a bottom `if (term) return;`
//   rotates it and duplicates the flag test). Every action is guarded by its own IsActiveThreadTerminating
//   check; the message temporaries are built right to left ("" first) from implicit `const char*` conversions.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CGameScriptInterface {
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
    virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147(); virtual void v148();
    virtual long GetPlayerCreatureCombatMultiplier() const;   // slot 149 (+0x254)
    virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155(); virtual void v156(); virtual void v157();
    virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163(); virtual void v164(); virtual void v165();
    virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171(); virtual void v172(); virtual void v173();
    virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179(); virtual void v180(); virtual void v181();
    virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187(); virtual void v188(); virtual void v189();
    virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194();
    virtual bool IsPlayerWieldingWeapon() const;   // slot 195 (+0x30c)
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203();
    virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211();
    virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243();
    virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267();
    virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273();
    virtual void HeroReceiveMessageFromGuildMaster(const CCharString& text, const CCharString& sound, bool important, bool blocking) const;   // slot 274 (+0x448)
    virtual void v275(); virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282();
    virtual void v283(); virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290();
    virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298();
    virtual void v299(); virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306();
    virtual void v307(); virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314();
    virtual void v315(); virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320(); virtual void v321(); virtual void v322();
    virtual void v323(); virtual void v324(); virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330();
    virtual void v331(); virtual void v332(); virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337(); virtual void v338();
    virtual void v339(); virtual void v340(); virtual void v341(); virtual void v342(); virtual void v343(); virtual void v344(); virtual void v345(); virtual void v346();
    virtual void v347(); virtual void v348(); virtual void v349(); virtual void v350(); virtual void v351(); virtual void v352(); virtual void v353(); virtual void v354();
    virtual void v355(); virtual void v356(); virtual void v357(); virtual void v358(); virtual void v359(); virtual void v360(); virtual void v361(); virtual void v362();
    virtual void v363(); virtual void v364(); virtual void v365(); virtual void v366(); virtual void v367(); virtual void v368(); virtual void v369(); virtual void v370();
    virtual void v371(); virtual void v372(); virtual void v373(); virtual void v374(); virtual void v375(); virtual void v376(); virtual void v377(); virtual void v378();
    virtual void v379(); virtual void v380(); virtual void v381(); virtual void v382(); virtual void v383(); virtual void v384(); virtual void v385(); virtual void v386();
    virtual void v387(); virtual void v388(); virtual void v389(); virtual void v390(); virtual void v391(); virtual void v392(); virtual void v393();
    virtual bool IsHeroControlledByPlayer() const;   // slot 394 (+0x628)
};

struct CScriptDef {
    unsigned char _pad[0xfd8];
    long          CombatMultFramesPerLevel;                 // +0xfd8
    long          CombatMultMasterLevel;                    // +0xfdc
};
extern CScriptDef* pGlobalScriptDef;                            // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)

struct CGameflowMasterData {
    unsigned char _pad[0x67];
    bool          StopAssistance;                           // +0x67
};
struct CScriptBase {
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CGameflowAssistanceScript : CScriptBase {
    CGameScriptInterface* GSI;                              // +0x40
    CGameflowMasterData*  PMasterData;                      // +0x44
    long                  FlourishesOffered;                // +0x48
    long                  FlourishesPerformed;              // +0x4c
    long                  CombatMultHighestLevel;           // +0x50
    long                  CombatMultFramesSinceChange;      // +0x54
    long                  HealthWarningsGiven;              // +0x58
    long                  WillEnergyWarningsGiven;          // +0x5c
    long                  SavedRenownLevel;                 // +0x60
    long                  SaveWarningsGiven;                // +0x64
    void Tutorial_CombatMultiplier();
};

void CGameflowAssistanceScript::Tutorial_CombatMultiplier()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        if (PMasterData->StopAssistance)
            break;
        if (GSI->IsPlayerWieldingWeapon() && GSI->IsHeroControlledByPlayer()) {
            if (IsActiveThreadTerminating())
                return;
            long multiplier = GSI->GetPlayerCreatureCombatMultiplier();
            if (multiplier > CombatMultHighestLevel) {
                if (IsActiveThreadTerminating())
                    return;
                CombatMultHighestLevel = multiplier;
                CombatMultFramesSinceChange = 0;
                if (multiplier > pGlobalScriptDef->CombatMultMasterLevel) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_MASTER", "", true, false);
                    return;
                }
                if (IsActiveThreadTerminating())
                    return;
                if (CombatMultHighestLevel == 15) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_EXCELLENT", "", true, false);
                } else if (CombatMultHighestLevel == 10) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_ADEPT", "", true, false);
                } else if (CombatMultHighestLevel == 5) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_PROGRESS", "", true, false);
                }
            } else {
                if (IsActiveThreadTerminating())
                    return;
                ++CombatMultFramesSinceChange;
                if (CombatMultFramesSinceChange > pGlobalScriptDef->CombatMultFramesPerLevel * CombatMultHighestLevel) {
                    if (IsActiveThreadTerminating())
                        return;
                    GSI->HeroReceiveMessageFromGuildMaster("TEXT_QST_080_MULT_TRY", "", true, false);
                    CombatMultFramesSinceChange = 0;
                }
            }
        }
        GSI->NewScriptFrame();
        if (!IsActiveThreadTerminating())
            continue;
        return;
    }
    if (IsActiveThreadTerminating())
        return;
}