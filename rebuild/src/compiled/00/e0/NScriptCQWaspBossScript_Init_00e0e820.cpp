// CQ_WaspBossScript::Init() -- register the Wasp Boss quest region and map offset, clear the quest state and reset the wasp-helper timer
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E0E820, length 192 bytes; vtable 0x012E00E4 slot 3 (factory 0x00E0E9F0,
// `push 5Ch` = Ego_r.pdb 0x70 - 0x14).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0; the quest's interface (+0x40) AddQuestRegion
// (slot 271) and SetQuestWorldMapOffset (slot 272); g_pGameScriptInterface (0x0143E8F8) SetTimer (slot 89) via CTimer.
// Layout (Ego_r.pdb - 0x14): the eight flags +0x48..+0x4f, SavedVillagerCount +0x50, QuestStartScreened +0x54,
// ReachedWaspHelper (CTimer) +0x58.
// Shape: the quest-name temporary is built before the C2DCoordI(10, 10) temporary (an implicit conversion goes first),
// whose two words are stored after its address is pushed; the clears share one zero register in source order.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

class CCPPointerInfo { public:                                  // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);           // destroys the pointee
    void* Data;
};

template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    ~CCountedPointer()
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

class CBaseClass { public:                                      // polymorphic root of the script handles
    CBaseClass();                                        // 0x0099A2D0
    virtual ~CBaseClass();                               // 0x0099A2E0
};

class CScriptThing : public CBaseClass { public:                       // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    CCountedPointer<CScriptThing> PImp;                  // +0x04 Data, +0x08 Info
    CScriptThing() {}                                    // the null thing
    ~CScriptThing() {}                                   // out of line 0x004AA840
    CScriptThing& operator=(const CScriptThing& other);  // 0x008AB980
    virtual void v01() const; virtual void v02() const; virtual void v03() const; virtual void v04() const; virtual void v05() const; virtual void v06() const; virtual void v07() const; virtual void v08() const;
    virtual void v09() const; virtual void v10() const; virtual void v11() const; virtual void v12() const; virtual void v13() const; virtual void v14() const; virtual void v15() const; virtual void v16() const;
    virtual void v17() const;
    virtual bool MsgIsKilledBy(const CCharString& killer) const;   // slot 18 (+0x48) 0x004AAAF0
    virtual void v19() const; virtual void v20() const; virtual void v21() const; virtual void v22() const; virtual void v23() const; virtual void v24() const; virtual void v25() const; virtual void v26() const;
    virtual void v27() const; virtual void v28() const; virtual void v29() const; virtual void v30() const; virtual void v31() const; virtual void v32() const; virtual void v33() const; virtual void v34() const;
    virtual void v35() const; virtual void v36() const; virtual void v37() const; virtual void v38() const; virtual void v39() const; virtual void v40() const; virtual void v41() const; virtual void v42() const;
    virtual void v43() const; virtual void v44() const; virtual void v45() const; virtual void v46() const; virtual void v47() const; virtual void v48() const; virtual void v49() const; virtual void v50() const;
    virtual void v51() const; virtual void v52() const; virtual void v53() const; virtual void v54() const; virtual void v55() const; virtual void v56() const; virtual void v57() const; virtual void v58() const;
    virtual void v59() const; virtual void v60() const;
    virtual bool IsUnconscious() const;                 // slot 61 (+0xf4) 0x004AAFA0
    virtual void v62() const; virtual void v63() const; virtual void v64() const; virtual void v65() const; virtual void v66() const; virtual void v67() const; virtual void v68() const; virtual void v69() const;
    virtual void v70() const; virtual void v71() const; virtual void v72() const; virtual void v73() const; virtual void v74() const;
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
class C2DCoordI { public:                                       // integer 2D coordinate
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) {}
};
class CGameScriptInterface { public:                           // retail vtable 0x01260F0C (slots: ghidra_out/cgsi_slots_aligned.tsv)
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
    virtual void SetTimer(long, long);   // slot 89 (+0x164) 0x006E76E0
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
    virtual void AddQuestRegion(const CCharString&, const CCharString&);   // slot 271 (+0x43c) 0x00891820
    virtual void SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&);   // slot 272 (+0x440) 0x00891860
};
class CScriptBase { public:                                     // retail 0x40 bytes (PDB 0x54: debug containers)
    virtual void v00();
    unsigned char _body[0x3c];                           // +0x04 .. +0x40
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

extern CGameScriptInterface* g_pGameScriptInterface;     // 0x0143E8F8

class CTimer { public:                                          // 4 bytes: an index into the global script timers
    long TimerIndex;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
};

namespace NScript {
class CQ_WaspBossScript : public CQuestScriptBase {
public:
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool DronesCreated;                                  // +0x4a
    bool QueenHornetAttacks;                             // +0x4b
    bool CutsceneFinished;                               // +0x4c
    bool SaidLineOnce;                                   // +0x4d
    bool PanickedVillagersScene;                         // +0x4e
    bool StartChase;                                     // +0x4f
    long SavedVillagerCount;                             // +0x50
    bool QuestStartScreened;                             // +0x54
    CTimer ReachedWaspHelper;                            // +0x58
    void Init();
};
} // namespace NScript
using NScript::CQ_WaspBossScript;

void NScript::CQ_WaspBossScript::Init()
{
    GSI->AddQuestRegion("Q_WaspBoss", "PicnicArea");
    GSI->SetQuestWorldMapOffset("Q_WaspBoss", C2DCoordI(10, 10));
    PanickedVillagersScene = false;
    MissionSucceeded = false;
    MissionFailed = false;
    DronesCreated = false;
    SaidLineOnce = false;
    QueenHornetAttacks = false;
    CutsceneFinished = false;
    StartChase = false;
    QuestStartScreened = false;
    SavedVillagerCount = 0;
    ReachedWaspHelper.SetTime(0);
}