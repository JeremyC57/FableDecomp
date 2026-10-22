// CQ_HeroSoulsMotherScript::CreateScreamers() -- start the super-screamer timer, gather the ScreamerHere points, show the quest bar and, until the bar is full, keep creating screamers at random points while fewer than the allowed number are alive; then remove the bar and record the mother's soul as collected
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D84630, length 662 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, rand 0x00BFEB16, ~vector<CScriptThing>
//   0x008AC970 (out of line), ~CBaseClass 0x0099A2E0 / operator delete 0x00BFE9BC (inline ~CScriptThing),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; CScriptThing::GetPos (slot 6); through the interface (+0x40):
//   NewScriptFrame (7), GetAllThingsWithScriptName (75), CreateCreature (91), DisplayQuestInfo (321), AddQuestInfoBar
//   (324), UpdateQuestInfoBar (332), RemoveQuestInfoElement (338); the timer service (global 0x0143E8F8) SetTimer (89).
// Data: the script def 0x0143E90C: HS_ScreamerCount +0xc70 (float), HS_ScreamersAtOnce +0xc74,
//   HS_SecondsBeforeNewSuperScreamer +0xc84.
// Layout (retail = Ego_r.pdb - 0x14, the csargs map 12 bytes): ScreamersAlive +0x58, BarToFill +0x5c, BarID +0x60,
//   NewSuperScreamerTimer +0x68; PMasterData (+0x44) CQ_SunnyvaleMasterData::CollectedSoulFromMother +0x66.
// Shape: the CScriptThing model of the landed CV_ChapelOfEvilScript::WatchForWeaponUnsheathed 0x00E6A340 (the discarded
//   CreateCreature result is destroyed inline); CRGBColour as the landed CQ_SecretPassageScript::EndMission 0x00DF0A50.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct CBaseClass {
    virtual ~CBaseClass();                                  // 0x0099A2E0
    CBaseClass();                                           // 0x0099A2D0
};
void operator delete(void* p);                              // 0x00BFE9BC (MSVCR71)
struct CCPPointerInfo {                                     // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);              // destroys the pointee
    void* Data;
};
template <class T> struct CCountedPointer {
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
struct C3DVector { float X, Y, Z; };
struct CScriptThingImpl;
struct CScriptThing : CBaseClass {                          // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005();
    virtual const C3DVector& GetPos() const;                // slot 6 (+0x18) 0x004AA980
    CCountedPointer<CScriptThingImpl> PImp;                 // +0x04 Data, +0x08 Info
    CScriptThing(const CScriptThing& other);                // 0x004ABE90
    ~CScriptThing() {}                                      // out of line 0x004AA840
};
namespace std {
template<class T> class allocator { };
template<class T, class A = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                              // 0x008AC970 for CScriptThing
    unsigned int size() const { return (unsigned int)(_M_finish - _M_start); }
    T& operator[](unsigned int n) { return _M_start[n]; }
};
}
struct CRGBColour {                                         // D3DCOLOR byte order
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
int __cdecl rand();                                         // 0x00BFEB16
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072(); virtual void v073(); virtual void v074();
    virtual long GetAllThingsWithScriptName(const CCharString& name, std::vector<CScriptThing>& things) const;   // slot 75 (+0x12c) GetAllThingsWithScriptName
    virtual void v076(); virtual void v077(); virtual void v078(); virtual void v079(); virtual void v080(); virtual void v081(); virtual void v082(); virtual void v083();
    virtual void v084(); virtual void v085(); virtual void v086(); virtual void v087(); virtual void v088();
    virtual void SetTimer(long timer, long time);   // slot 89 (+0x164) SetTimer
    virtual void v090();
    virtual CScriptThing CreateCreature(const CCharString& def, const C3DVector& pos, const CCharString& scriptName, bool a) const;   // slot 91 (+0x16c) CreateCreature
    virtual void v092(); virtual void v093(); virtual void v094(); virtual void v095(); virtual void v096(); virtual void v097(); virtual void v098(); virtual void v099();
    virtual void v100(); virtual void v101(); virtual void v102(); virtual void v103(); virtual void v104(); virtual void v105(); virtual void v106(); virtual void v107();
    virtual void v108(); virtual void v109(); virtual void v110(); virtual void v111(); virtual void v112(); virtual void v113(); virtual void v114(); virtual void v115();
    virtual void v116(); virtual void v117(); virtual void v118(); virtual void v119(); virtual void v120(); virtual void v121(); virtual void v122(); virtual void v123();
    virtual void v124(); virtual void v125(); virtual void v126(); virtual void v127(); virtual void v128(); virtual void v129(); virtual void v130(); virtual void v131();
    virtual void v132(); virtual void v133(); virtual void v134(); virtual void v135(); virtual void v136(); virtual void v137(); virtual void v138(); virtual void v139();
    virtual void v140(); virtual void v141(); virtual void v142(); virtual void v143(); virtual void v144(); virtual void v145(); virtual void v146(); virtual void v147();
    virtual void v148(); virtual void v149(); virtual void v150(); virtual void v151(); virtual void v152(); virtual void v153(); virtual void v154(); virtual void v155();
    virtual void v156(); virtual void v157(); virtual void v158(); virtual void v159(); virtual void v160(); virtual void v161(); virtual void v162(); virtual void v163();
    virtual void v164(); virtual void v165(); virtual void v166(); virtual void v167(); virtual void v168(); virtual void v169(); virtual void v170(); virtual void v171();
    virtual void v172(); virtual void v173(); virtual void v174(); virtual void v175(); virtual void v176(); virtual void v177(); virtual void v178(); virtual void v179();
    virtual void v180(); virtual void v181(); virtual void v182(); virtual void v183(); virtual void v184(); virtual void v185(); virtual void v186(); virtual void v187();
    virtual void v188(); virtual void v189(); virtual void v190(); virtual void v191(); virtual void v192(); virtual void v193(); virtual void v194(); virtual void v195();
    virtual void v196(); virtual void v197(); virtual void v198(); virtual void v199(); virtual void v200(); virtual void v201(); virtual void v202(); virtual void v203();
    virtual void v204(); virtual void v205(); virtual void v206(); virtual void v207(); virtual void v208(); virtual void v209(); virtual void v210(); virtual void v211();
    virtual void v212(); virtual void v213(); virtual void v214(); virtual void v215(); virtual void v216(); virtual void v217(); virtual void v218(); virtual void v219();
    virtual void v220(); virtual void v221(); virtual void v222(); virtual void v223(); virtual void v224(); virtual void v225(); virtual void v226(); virtual void v227();
    virtual void v228(); virtual void v229(); virtual void v230(); virtual void v231(); virtual void v232(); virtual void v233(); virtual void v234(); virtual void v235();
    virtual void v236(); virtual void v237(); virtual void v238(); virtual void v239(); virtual void v240(); virtual void v241(); virtual void v242(); virtual void v243();
    virtual void v244(); virtual void v245(); virtual void v246(); virtual void v247(); virtual void v248(); virtual void v249(); virtual void v250(); virtual void v251();
    virtual void v252(); virtual void v253(); virtual void v254(); virtual void v255(); virtual void v256(); virtual void v257(); virtual void v258(); virtual void v259();
    virtual void v260(); virtual void v261(); virtual void v262(); virtual void v263(); virtual void v264(); virtual void v265(); virtual void v266(); virtual void v267();
    virtual void v268(); virtual void v269(); virtual void v270(); virtual void v271(); virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void v276(); virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual void v284(); virtual void v285(); virtual void v286(); virtual void v287(); virtual void v288(); virtual void v289(); virtual void v290(); virtual void v291();
    virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295(); virtual void v296(); virtual void v297(); virtual void v298(); virtual void v299();
    virtual void v300(); virtual void v301(); virtual void v302(); virtual void v303(); virtual void v304(); virtual void v305(); virtual void v306(); virtual void v307();
    virtual void v308(); virtual void v309(); virtual void v310(); virtual void v311(); virtual void v312(); virtual void v313(); virtual void v314(); virtual void v315();
    virtual void v316(); virtual void v317(); virtual void v318(); virtual void v319(); virtual void v320();
    virtual void DisplayQuestInfo(bool display) const;   // slot 321 (+0x504) DisplayQuestInfo
    virtual void v322(); virtual void v323();
    virtual long AddQuestInfoBar(float a, float b, const CRGBColour& c1, const CRGBColour& c2, const CCharString& s1, const CCharString& s2, float scale) const;   // slot 324 (+0x510) AddQuestInfoBar
    virtual void v325(); virtual void v326(); virtual void v327(); virtual void v328(); virtual void v329(); virtual void v330(); virtual void v331();
    virtual void UpdateQuestInfoBar(long id, float a, float b, float c) const;   // slot 332 (+0x530) UpdateQuestInfoBar
    virtual void v333(); virtual void v334(); virtual void v335(); virtual void v336(); virtual void v337();
    virtual void RemoveQuestInfoElement(long id) const;   // slot 338 (+0x548) RemoveQuestInfoElement
};
extern CGameScriptInterface* g_pGameScriptInterface;        // 0x0143E8F8
struct CTimer {                                             // a script timer: the interface's timer index
    long Index;
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(Index, time); }
};
struct CScriptDef {
    unsigned char _pad0[0xc70];
    float HS_ScreamerCount;                                 // +0xc70
    long  HS_ScreamersAtOnce;                               // +0xc74
    unsigned char _padc78[0xc84 - 0xc78];
    long  HS_SecondsBeforeNewSuperScreamer;                 // +0xc84
};
extern CScriptDef* g_pScriptDef;                            // 0x0143E90C
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x66];
    bool CollectedSoulFromMother;                           // +0x66
};
namespace NScript {
struct CScriptBase {                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQ_HeroSoulsMotherScript : CScriptBase {
    CGameScriptInterface*   Game;                           // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                    // +0x44
    bool                    PlayedMotherCS;                 // +0x48
    unsigned char           csargs[0xc];                    // +0x4c map<CCharString, CCharString>
    long                    ScreamersAlive;                 // +0x58
    float                   BarToFill;                      // +0x5c
    long                    BarID;                          // +0x60
    bool                    QuestStartScreened;             // +0x64
    bool                    GotSuperScreamer;               // +0x65
    CTimer                  NewSuperScreamerTimer;          // +0x68
    void CreateScreamers();
};
void CQ_HeroSoulsMotherScript::CreateScreamers()
{
    NewSuperScreamerTimer.SetTime(g_pScriptDef->HS_SecondsBeforeNewSuperScreamer);
    std::vector<CScriptThing> screamerPoints;
    Game->GetAllThingsWithScriptName("ScreamerHere", screamerPoints);
    Game->DisplayQuestInfo(true);
    BarID = Game->AddQuestInfoBar(g_pScriptDef->HS_ScreamerCount, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255),
                                  "HUD_QUEST_ICON_UNDEAD", "", 1.0f);
    Game->UpdateQuestInfoBar(BarID, (float)ScreamersAlive, -1.0f, -1.0f);
    while (BarToFill < g_pScriptDef->HS_ScreamerCount) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (ScreamersAlive < g_pScriptDef->HS_ScreamersAtOnce) {
            if (IsActiveThreadTerminating())
                return;
            Game->CreateCreature("CREATURE_NW_SCREAMER_01", screamerPoints[rand() % screamerPoints.size()].GetPos(), "HS_Screamer", true);
        }
        Game->UpdateQuestInfoBar(BarID, BarToFill, -1.0f, -1.0f);
    }
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(BarID);
    PMasterData->CollectedSoulFromMother = true;
}
}