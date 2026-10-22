// CPuzzleBlockMoon::TurnIntoASunSign(bool createEffect) -- flip a Fire Heart puzzle pad from moon to sun: remove the old pad effect (optionally replacing it with the sun pad effect), spawn the sun tile object carrying this pad's data string, count the sun, and when every pad shows a sun (and the round is not just beginning) win the round with the Fire Heart frame effect, the minigame-complete sound and a short pause; finally remove the moon block
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D38000, length 770 bytes, `ret 4`.
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30; CCharString 0x0099EBF0 / 0x0099EAE0 ("" 0x0122D70E,
// "PuzzleBlockSun" 0x012CCE70, "OBJECT_LPDD_TILE_SUN_01" 0x012CD0EC, "MK_FH_FIREHEART" 0x012CD0DC,
// "TILE_FRAME_SUN_COMPLETE" 0x012CD0C4, "SND_MM_MINIGAME_COMPLETE" 0x012CD0A8); ~CScriptThing 0x004AA840 (out of line);
// CCountedPointer::operator= 0x00704580; CQ_FireHeartScript::IncNumberOfSuns 0x00D38310; CScriptThing slots 3
// GetDataString, 4 SetDataString (inline on the local), 6 GetPos, 10 GetAngleXY, 75 IsAlive, 77 IsNull; CGameScriptInterface
// slots 72 GetThingWithScriptName, 97 CreateObject, 100 CreateEffect, 108 RemoveThing, 376 Pause, 692 Play2DSound.
// Layout: entity-script base {vptr, Game +4, Me +8}, ParentClass +0x14, PMasterData +0x18, PadNumber +0x1c (Ego_r.pdb).
// Retail CQ_FireHeartScript = Ego_r.pdb less 0x14 (script base) and 4 per STLport container (as the landed
// SetupPuzzleBlocks 0x00D36140): NumberOfPads +0x7c, NumberOfSuns +0x80, PuzzleRound +0x88, RoundEnded +0x94,
// RoundBeginning +0x95, PreventRoundEnd +0x96, GoodRounds +0xa0, Effect +0xa8, PadSunFX +0xb4, ProphetsReleased +0xd4.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CCPPointerInfo;
template <class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer& operator=(const CCountedPointer& other);   // 0x00704580
};
struct C3DVector { float X, Y, Z; };
struct CScriptThingImp {
    virtual void i00(); virtual void i01(); virtual void i02(); virtual void i03();
    virtual void SetDataString(const CCharString& data);  // slot 4 (+0x10)
};
struct CScriptThing {                                    // 12 bytes, vtable 0x01238C8C
    virtual ~CScriptThing();                             // 0x004AA840
    CCountedPointer<CScriptThingImp> PImp;               // +0x04
    virtual void v01() const; virtual void v02() const;
    virtual CCharString GetDataString() const;           // slot 3 (+0x0c)
    virtual void SetDataString(const CCharString& data) { if (PImp.Data) PImp.Data->SetDataString(data); }   // slot 4 (+0x10)
    virtual void v05() const;
    virtual const C3DVector& GetPos() const;             // slot 6 (+0x18)
    virtual void v07() const; virtual void v08() const; virtual void v09() const;
    virtual float GetAngleXY() const;                    // slot 10 (+0x28)
    virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const; virtual void v17() const; virtual void v18() const;
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const; virtual void v61() const; virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const;
    virtual void v67() const; virtual void v68() const; virtual void v69() const; virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const;                        // slot 75 (+0x12c)
    virtual bool IsDead() const;                         // slot 76 (+0x130)
    virtual bool IsNull() const;                         // slot 77 (+0x134)
};
struct CGameScriptInterface {                      // retail vtable 0x01260F0C (ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000() const; virtual void v001() const; virtual void v002() const; virtual void v003() const; virtual void v004() const; virtual void v005() const; virtual void v006() const; virtual void v007() const; virtual void v008() const; virtual void v009() const;
    virtual void v010() const; virtual void v011() const; virtual void v012() const; virtual void v013() const; virtual void v014() const; virtual void v015() const; virtual void v016() const; virtual void v017() const; virtual void v018() const; virtual void v019() const;
    virtual void v020() const; virtual void v021() const; virtual void v022() const; virtual void v023() const; virtual void v024() const; virtual void v025() const; virtual void v026() const; virtual void v027() const; virtual void v028() const; virtual void v029() const;
    virtual void v030() const; virtual void v031() const; virtual void v032() const; virtual void v033() const; virtual void v034() const; virtual void v035() const; virtual void v036() const; virtual void v037() const; virtual void v038() const; virtual void v039() const;
    virtual void v040() const; virtual void v041() const; virtual void v042() const; virtual void v043() const; virtual void v044() const; virtual void v045() const; virtual void v046() const; virtual void v047() const; virtual void v048() const; virtual void v049() const;
    virtual void v050() const; virtual void v051() const; virtual void v052() const; virtual void v053() const; virtual void v054() const; virtual void v055() const; virtual void v056() const; virtual void v057() const; virtual void v058() const; virtual void v059() const;
    virtual void v060() const; virtual void v061() const; virtual void v062() const; virtual void v063() const; virtual void v064() const; virtual void v065() const; virtual void v066() const; virtual void v067() const; virtual void v068() const; virtual void v069() const;
    virtual void v070() const; virtual void v071() const;
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) const;   // slot 72 (+0x120) 0x008A7D60 public: virtual class CScriptThing __thiscall CGameScriptInterface::GetThingWithScriptName(class CCharString const &) const
    virtual void v073() const; virtual void v074() const; virtual void v075() const; virtual void v076() const; virtual void v077() const; virtual void v078() const; virtual void v079() const; virtual void v080() const; virtual void v081() const; virtual void v082() const;
    virtual void v083() const; virtual void v084() const; virtual void v085() const; virtual void v086() const; virtual void v087() const; virtual void v088() const; virtual void v089() const; virtual void v090() const; virtual void v091() const; virtual void v092() const;
    virtual void v093() const; virtual void v094() const; virtual void v095() const; virtual void v096() const;
    virtual CScriptThing CreateObject(const CCharString& name, const C3DVector& position, float angle, const CCharString& scriptName) const;   // slot 97 (+0x184) 0x00894950 public: virtual class CScriptThing __thiscall CGameScriptInterface::CreateObject(class CCharString const &, class C3DVector const &, float, class CCharString const &) const
    virtual void v098() const; virtual void v099() const;
    virtual CScriptThing CreateEffect(const CCharString& effect, const C3DVector& position, const CCharString& bone, float angle, bool b, bool c) const;   // slot 100 (+0x190) 0x0089F9E0 public: virtual class CScriptThing __thiscall CGameScriptInterface::CreateEffect(class CCharString const &, class C3DVector const &, class CCharString const &, float, bool, bool) const
    virtual void v101() const; virtual void v102() const; virtual void v103() const; virtual void v104() const; virtual void v105() const; virtual void v106() const; virtual void v107() const;
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) const;   // slot 108 (+0x1b0) 0x008910D0 public: virtual void __thiscall CGameScriptInterface::RemoveThing(class CScriptThing const &, bool, bool) const
    virtual void v109() const; virtual void v110() const; virtual void v111() const; virtual void v112() const; virtual void v113() const; virtual void v114() const; virtual void v115() const; virtual void v116() const; virtual void v117() const; virtual void v118() const;
    virtual void v119() const; virtual void v120() const; virtual void v121() const; virtual void v122() const; virtual void v123() const; virtual void v124() const; virtual void v125() const; virtual void v126() const; virtual void v127() const; virtual void v128() const;
    virtual void v129() const; virtual void v130() const; virtual void v131() const; virtual void v132() const; virtual void v133() const; virtual void v134() const; virtual void v135() const; virtual void v136() const; virtual void v137() const; virtual void v138() const;
    virtual void v139() const; virtual void v140() const; virtual void v141() const; virtual void v142() const; virtual void v143() const; virtual void v144() const; virtual void v145() const; virtual void v146() const; virtual void v147() const; virtual void v148() const;
    virtual void v149() const; virtual void v150() const; virtual void v151() const; virtual void v152() const; virtual void v153() const; virtual void v154() const; virtual void v155() const; virtual void v156() const; virtual void v157() const; virtual void v158() const;
    virtual void v159() const; virtual void v160() const; virtual void v161() const; virtual void v162() const; virtual void v163() const; virtual void v164() const; virtual void v165() const; virtual void v166() const; virtual void v167() const; virtual void v168() const;
    virtual void v169() const; virtual void v170() const; virtual void v171() const; virtual void v172() const; virtual void v173() const; virtual void v174() const; virtual void v175() const; virtual void v176() const; virtual void v177() const; virtual void v178() const;
    virtual void v179() const; virtual void v180() const; virtual void v181() const; virtual void v182() const; virtual void v183() const; virtual void v184() const; virtual void v185() const; virtual void v186() const; virtual void v187() const; virtual void v188() const;
    virtual void v189() const; virtual void v190() const; virtual void v191() const; virtual void v192() const; virtual void v193() const; virtual void v194() const; virtual void v195() const; virtual void v196() const; virtual void v197() const; virtual void v198() const;
    virtual void v199() const; virtual void v200() const; virtual void v201() const; virtual void v202() const; virtual void v203() const; virtual void v204() const; virtual void v205() const; virtual void v206() const; virtual void v207() const; virtual void v208() const;
    virtual void v209() const; virtual void v210() const; virtual void v211() const; virtual void v212() const; virtual void v213() const; virtual void v214() const; virtual void v215() const; virtual void v216() const; virtual void v217() const; virtual void v218() const;
    virtual void v219() const; virtual void v220() const; virtual void v221() const; virtual void v222() const; virtual void v223() const; virtual void v224() const; virtual void v225() const; virtual void v226() const; virtual void v227() const; virtual void v228() const;
    virtual void v229() const; virtual void v230() const; virtual void v231() const; virtual void v232() const; virtual void v233() const; virtual void v234() const; virtual void v235() const; virtual void v236() const; virtual void v237() const; virtual void v238() const;
    virtual void v239() const; virtual void v240() const; virtual void v241() const; virtual void v242() const; virtual void v243() const; virtual void v244() const; virtual void v245() const; virtual void v246() const; virtual void v247() const; virtual void v248() const;
    virtual void v249() const; virtual void v250() const; virtual void v251() const; virtual void v252() const; virtual void v253() const; virtual void v254() const; virtual void v255() const; virtual void v256() const; virtual void v257() const; virtual void v258() const;
    virtual void v259() const; virtual void v260() const; virtual void v261() const; virtual void v262() const; virtual void v263() const; virtual void v264() const; virtual void v265() const; virtual void v266() const; virtual void v267() const; virtual void v268() const;
    virtual void v269() const; virtual void v270() const; virtual void v271() const; virtual void v272() const; virtual void v273() const; virtual void v274() const; virtual void v275() const; virtual void v276() const; virtual void v277() const; virtual void v278() const;
    virtual void v279() const; virtual void v280() const; virtual void v281() const; virtual void v282() const; virtual void v283() const; virtual void v284() const; virtual void v285() const; virtual void v286() const; virtual void v287() const; virtual void v288() const;
    virtual void v289() const; virtual void v290() const; virtual void v291() const; virtual void v292() const; virtual void v293() const; virtual void v294() const; virtual void v295() const; virtual void v296() const; virtual void v297() const; virtual void v298() const;
    virtual void v299() const; virtual void v300() const; virtual void v301() const; virtual void v302() const; virtual void v303() const; virtual void v304() const; virtual void v305() const; virtual void v306() const; virtual void v307() const; virtual void v308() const;
    virtual void v309() const; virtual void v310() const; virtual void v311() const; virtual void v312() const; virtual void v313() const; virtual void v314() const; virtual void v315() const; virtual void v316() const; virtual void v317() const; virtual void v318() const;
    virtual void v319() const; virtual void v320() const; virtual void v321() const; virtual void v322() const; virtual void v323() const; virtual void v324() const; virtual void v325() const; virtual void v326() const; virtual void v327() const; virtual void v328() const;
    virtual void v329() const; virtual void v330() const; virtual void v331() const; virtual void v332() const; virtual void v333() const; virtual void v334() const; virtual void v335() const; virtual void v336() const; virtual void v337() const; virtual void v338() const;
    virtual void v339() const; virtual void v340() const; virtual void v341() const; virtual void v342() const; virtual void v343() const; virtual void v344() const; virtual void v345() const; virtual void v346() const; virtual void v347() const; virtual void v348() const;
    virtual void v349() const; virtual void v350() const; virtual void v351() const; virtual void v352() const; virtual void v353() const; virtual void v354() const; virtual void v355() const; virtual void v356() const; virtual void v357() const; virtual void v358() const;
    virtual void v359() const; virtual void v360() const; virtual void v361() const; virtual void v362() const; virtual void v363() const; virtual void v364() const; virtual void v365() const; virtual void v366() const; virtual void v367() const; virtual void v368() const;
    virtual void v369() const; virtual void v370() const; virtual void v371() const; virtual void v372() const; virtual void v373() const; virtual void v374() const; virtual void v375() const;
    virtual void Pause(float seconds) const;   // slot 376 (+0x5e0) 0x0088E4F0 public: virtual void __thiscall CGameScriptInterface::Pause(float) const
    virtual void v377() const; virtual void v378() const; virtual void v379() const; virtual void v380() const; virtual void v381() const; virtual void v382() const; virtual void v383() const; virtual void v384() const; virtual void v385() const; virtual void v386() const;
    virtual void v387() const; virtual void v388() const; virtual void v389() const; virtual void v390() const; virtual void v391() const; virtual void v392() const; virtual void v393() const; virtual void v394() const; virtual void v395() const; virtual void v396() const;
    virtual void v397() const; virtual void v398() const; virtual void v399() const; virtual void v400() const; virtual void v401() const; virtual void v402() const; virtual void v403() const; virtual void v404() const; virtual void v405() const; virtual void v406() const;
    virtual void v407() const; virtual void v408() const; virtual void v409() const; virtual void v410() const; virtual void v411() const; virtual void v412() const; virtual void v413() const; virtual void v414() const; virtual void v415() const; virtual void v416() const;
    virtual void v417() const; virtual void v418() const; virtual void v419() const; virtual void v420() const; virtual void v421() const; virtual void v422() const; virtual void v423() const; virtual void v424() const; virtual void v425() const; virtual void v426() const;
    virtual void v427() const; virtual void v428() const; virtual void v429() const; virtual void v430() const; virtual void v431() const; virtual void v432() const; virtual void v433() const; virtual void v434() const; virtual void v435() const; virtual void v436() const;
    virtual void v437() const; virtual void v438() const; virtual void v439() const; virtual void v440() const; virtual void v441() const; virtual void v442() const; virtual void v443() const; virtual void v444() const; virtual void v445() const; virtual void v446() const;
    virtual void v447() const; virtual void v448() const; virtual void v449() const; virtual void v450() const; virtual void v451() const; virtual void v452() const; virtual void v453() const; virtual void v454() const; virtual void v455() const; virtual void v456() const;
    virtual void v457() const; virtual void v458() const; virtual void v459() const; virtual void v460() const; virtual void v461() const; virtual void v462() const; virtual void v463() const; virtual void v464() const; virtual void v465() const; virtual void v466() const;
    virtual void v467() const; virtual void v468() const; virtual void v469() const; virtual void v470() const; virtual void v471() const; virtual void v472() const; virtual void v473() const; virtual void v474() const; virtual void v475() const; virtual void v476() const;
    virtual void v477() const; virtual void v478() const; virtual void v479() const; virtual void v480() const; virtual void v481() const; virtual void v482() const; virtual void v483() const; virtual void v484() const; virtual void v485() const; virtual void v486() const;
    virtual void v487() const; virtual void v488() const; virtual void v489() const; virtual void v490() const; virtual void v491() const; virtual void v492() const; virtual void v493() const; virtual void v494() const; virtual void v495() const; virtual void v496() const;
    virtual void v497() const; virtual void v498() const; virtual void v499() const; virtual void v500() const; virtual void v501() const; virtual void v502() const; virtual void v503() const; virtual void v504() const; virtual void v505() const; virtual void v506() const;
    virtual void v507() const; virtual void v508() const; virtual void v509() const; virtual void v510() const; virtual void v511() const; virtual void v512() const; virtual void v513() const; virtual void v514() const; virtual void v515() const; virtual void v516() const;
    virtual void v517() const; virtual void v518() const; virtual void v519() const; virtual void v520() const; virtual void v521() const; virtual void v522() const; virtual void v523() const; virtual void v524() const; virtual void v525() const; virtual void v526() const;
    virtual void v527() const; virtual void v528() const; virtual void v529() const; virtual void v530() const; virtual void v531() const; virtual void v532() const; virtual void v533() const; virtual void v534() const; virtual void v535() const; virtual void v536() const;
    virtual void v537() const; virtual void v538() const; virtual void v539() const; virtual void v540() const; virtual void v541() const; virtual void v542() const; virtual void v543() const; virtual void v544() const; virtual void v545() const; virtual void v546() const;
    virtual void v547() const; virtual void v548() const; virtual void v549() const; virtual void v550() const; virtual void v551() const; virtual void v552() const; virtual void v553() const; virtual void v554() const; virtual void v555() const; virtual void v556() const;
    virtual void v557() const; virtual void v558() const; virtual void v559() const; virtual void v560() const; virtual void v561() const; virtual void v562() const; virtual void v563() const; virtual void v564() const; virtual void v565() const; virtual void v566() const;
    virtual void v567() const; virtual void v568() const; virtual void v569() const; virtual void v570() const; virtual void v571() const; virtual void v572() const; virtual void v573() const; virtual void v574() const; virtual void v575() const; virtual void v576() const;
    virtual void v577() const; virtual void v578() const; virtual void v579() const; virtual void v580() const; virtual void v581() const; virtual void v582() const; virtual void v583() const; virtual void v584() const; virtual void v585() const; virtual void v586() const;
    virtual void v587() const; virtual void v588() const; virtual void v589() const; virtual void v590() const; virtual void v591() const; virtual void v592() const; virtual void v593() const; virtual void v594() const; virtual void v595() const; virtual void v596() const;
    virtual void v597() const; virtual void v598() const; virtual void v599() const; virtual void v600() const; virtual void v601() const; virtual void v602() const; virtual void v603() const; virtual void v604() const; virtual void v605() const; virtual void v606() const;
    virtual void v607() const; virtual void v608() const; virtual void v609() const; virtual void v610() const; virtual void v611() const; virtual void v612() const; virtual void v613() const; virtual void v614() const; virtual void v615() const; virtual void v616() const;
    virtual void v617() const; virtual void v618() const; virtual void v619() const; virtual void v620() const; virtual void v621() const; virtual void v622() const; virtual void v623() const; virtual void v624() const; virtual void v625() const; virtual void v626() const;
    virtual void v627() const; virtual void v628() const; virtual void v629() const; virtual void v630() const; virtual void v631() const; virtual void v632() const; virtual void v633() const; virtual void v634() const; virtual void v635() const; virtual void v636() const;
    virtual void v637() const; virtual void v638() const; virtual void v639() const; virtual void v640() const; virtual void v641() const; virtual void v642() const; virtual void v643() const; virtual void v644() const; virtual void v645() const; virtual void v646() const;
    virtual void v647() const; virtual void v648() const; virtual void v649() const; virtual void v650() const; virtual void v651() const; virtual void v652() const; virtual void v653() const; virtual void v654() const; virtual void v655() const; virtual void v656() const;
    virtual void v657() const; virtual void v658() const; virtual void v659() const; virtual void v660() const; virtual void v661() const; virtual void v662() const; virtual void v663() const; virtual void v664() const; virtual void v665() const; virtual void v666() const;
    virtual void v667() const; virtual void v668() const; virtual void v669() const; virtual void v670() const; virtual void v671() const; virtual void v672() const; virtual void v673() const; virtual void v674() const; virtual void v675() const; virtual void v676() const;
    virtual void v677() const; virtual void v678() const; virtual void v679() const; virtual void v680() const; virtual void v681() const; virtual void v682() const; virtual void v683() const; virtual void v684() const; virtual void v685() const; virtual void v686() const;
    virtual void v687() const; virtual void v688() const; virtual void v689() const; virtual void v690() const; virtual void v691() const;
    virtual unsigned long Play2DSound(const CCharString& sound) const;   // slot 692 (+0xad0) 0x0088F570 public: virtual unsigned long __thiscall CGameScriptInterface::Play2DSound(class CCharString const &) const
};

namespace NScript {
struct CQ_FireHeartScript {                              // retail-offset view (see Layout)
    unsigned char  _pad00[0x7c];
    long           NumberOfPads;                         // +0x7c
    long           NumberOfSuns;                         // +0x80
    long           NumberOfVillagers;                    // +0x84
    long           PuzzleRound;                          // +0x88
    unsigned char  _pad8c[0x94 - 0x8c];
    bool           RoundEnded;                           // +0x94
    bool           RoundBeginning;                       // +0x95
    bool           PreventRoundEnd;                      // +0x96
    unsigned char  _pad97[0xa0 - 0x97];
    long           GoodRounds;                           // +0xa0
    float          PadRadius;                            // +0xa4
    CScriptThing   Effect;                               // +0xa8
    CCharString    PadSunFX;                             // +0xb4
    CCharString    PadMoonFX;                            // +0xb8
    unsigned char  _padbc[0xd4 - 0xbc];
    bool*          ProphetsReleased;                     // +0xd4 (vector<bool> _M_start, one byte per element)
    void IncNumberOfSuns();                              // 0x00D38310
};
}
struct CEntityScriptBase {                               // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                          // +0x04
    CScriptThing          Me;                            // +0x08
    bool IsActiveThreadTerminating() const;              // 0x00F35B30
};
struct CPuzzleBlockMoon : CEntityScriptBase {
    NScript::CQ_FireHeartScript* ParentClass;            // +0x14
    void*                        PMasterData;            // +0x18
    long                         PadNumber;              // +0x1c
    void TurnIntoASunSign(bool createEffect);
};

void CPuzzleBlockMoon::TurnIntoASunSign(bool createEffect)
{
    ParentClass->PreventRoundEnd = true;
    if (ParentClass->RoundEnded) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->PreventRoundEnd = false;
        return;
    }
    if (!ParentClass->Effect.IsNull() && ParentClass->Effect.IsAlive()) {
        if (IsActiveThreadTerminating())
            return;
        Game->RemoveThing(ParentClass->Effect, false, true);
    }
    if (createEffect) {
        if (IsActiveThreadTerminating())
            return;
        ParentClass->Effect.PImp = Game->CreateEffect(ParentClass->PadSunFX, Me.GetPos(), "", 0.0f, false, false).PImp;
    }
    CScriptThing block = Game->CreateObject("OBJECT_LPDD_TILE_SUN_01", Me.GetPos(), Me.GetAngleXY(), "PuzzleBlockSun");
    block.SetDataString(Me.GetDataString());
    ParentClass->IncNumberOfSuns();
    if (ParentClass->NumberOfSuns == ParentClass->NumberOfPads && !ParentClass->RoundBeginning) {
        if (IsActiveThreadTerminating())
            return;
        ++ParentClass->GoodRounds;
        Game->CreateEffect("TILE_FRAME_SUN_COMPLETE", Game->GetThingWithScriptName("MK_FH_FIREHEART").GetPos(), "", 0.0f, false, false);
        Game->Play2DSound("SND_MM_MINIGAME_COMPLETE");
        Game->Pause(0.2f);
        ParentClass->RoundEnded = true;
        ParentClass->ProphetsReleased[ParentClass->PuzzleRound] = true;
    }
    Game->RemoveThing(Me, false, true);
    ParentClass->PreventRoundEnd = false;
}
