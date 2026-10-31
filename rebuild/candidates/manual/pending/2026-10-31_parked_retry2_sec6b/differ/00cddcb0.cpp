// Grade: DIFFER(1479v1475) 14 diff lines (parked 1605v1475, 63 lines): __forceinline on ~list<CCharString>, ~CDefPointer and CDefBase::Release beats the inline budget at the last return (retail inlines every exit cleanup and tail-merges them into two def-release copies, exactly as ours now), while the in-body inline_depth(0) on the first return keeps the out-of-line ~list 0x0075F6C0 there; left: one store -- ours keeps `m_p = 0` (mov [esp+14h],ebx at the end of the inline ~CDefPointer) on the normal loop path, retail drops it there and keeps it only on the exits (tried: unconditional clear after the release, Release not forceinline -- both identical)
// CPersonalScriptMainScript::Main() -- the personal-script manager: every frame, for each newly loaded level activate its registered, inactive PersonalScript_<level> quest; when a region unloads zero the random-entity maxima and counts; when a region loads, look up its REGION_<NAME> def and copy the villager / guard / bandit maxima into the master data (counts zeroed, villager percentage 1), then activate its registered, inactive RegionScript_<region> quest (the "Loading" / "Cannot load" / "already active" debug strings are built and dropped)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CDDCB0, length 1488 bytes.
// Callees: CCharString() 0x0099E4B0 / (const char*, long) 0x0099EBF0 / (const CCharString&) 0x0099EC30 / ToUpper 0x0099EE20 /
//   ~ 0x0099EAE0, operator+ (__fastcall, hidden result in ecx) (const char*, const CCharString&) 0x0099F690 /
//   (const CCharString&, const char*) 0x0099F600 / (const CCharString&, const CCharString&) 0x0099F570,
//   list<CCharString>(const allocator&) 0x0075F560 / clear 0x0075F580 / ~list 0x0075F6C0, free 0x00BFEA14,
//   GFGetPDefinitionManager 0x00CDE3EF0, CDefinitionManager::GetDef<T> 0x00CDE280, IsActiveThreadTerminating 0x00CB7940;
//   the game script interface (+0x40) slots 7, 18, 22, 23, 276, 284, 285.
// Layout: master data (+0x44) CQ_SunnyvaleMasterData: RandomEntityVillagerMax +0x14 .. RandomEntityBanditCount +0x28,
//   RandomEntityVillagerPercentage +0x2c; the region def's MaxPopulation[3] +0x3c (as CV_RandomPopulationSimScript::
//   ProcessAlbionPopulationSim 0x00EC2240, whose def model this reuses).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14
struct CCharString {
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    CCharString(const CCharString& other);                  // 0x0099EC30
    ~CCharString();                                         // 0x0099EAE0
    CCharString ToUpper();                                  // 0x0099EE20
};
CCharString __fastcall operator+(const char* left, const CCharString& right);       // 0x0099F690
CCharString __fastcall operator+(const CCharString& left, const char* right);       // 0x0099F600
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570
namespace std {
template <class T> struct allocator { allocator() {} };
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;
};
template <class T> struct _List_iterator {
    _List_node<T>* _M_node;
    _List_iterator(_List_node<T>* n) : _M_node(n) {}
    T& operator*() const { return _M_node->_M_data; }
    _List_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
    bool operator!=(const _List_iterator& o) const { return _M_node != o._M_node; }
};
template <class T, class A = allocator<T> > struct list {    // retail STLport list: the sentinel pointer only
    typedef _List_iterator<T> iterator;
    _List_node<T>* _M_node;
    explicit list(const A& a = A());                    // 0x0075F560
    __forceinline ~list() { clear(); if (_M_node) free(_M_node); }                 // 0x0075F6C0 out of line
    void clear();                                       // 0x0075F580
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
};
}
struct CDefVirtual {
    virtual ~CDefVirtual();                                 // slot 0
    virtual void Destroy();                                 // slot 1
};
struct CRefCounted {
    long m_count;
};
struct CDefBase : CDefVirtual, CRefCounted {
    __forceinline void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
struct CRegionDef : CDefBase {                              // the REGION_<name> def (type name not recovered)
    unsigned char _pad08[0x3c - 0x08];
    long MaxPopulation[3];                                  // +0x3c villagers / guards / bandits
};
struct CDefPointer {
    CRegionDef* m_p;
    CDefPointer() : m_p(0) { }
    __forceinline ~CDefPointer()
    {
        if (m_p) {
            m_p->Release();
            m_p = 0;
        }
    }
    CRegionDef* operator->() const { return m_p; }
};
struct CDefinitionManager {
    bool GetDef(const CCharString& name, CDefPointer& out);   // 0x00CDE280
};
CDefinitionManager* GFGetPDefinitionManager();              // 0x00CD3EF0
struct CGameScriptInterface {                       // the quest's game script interface (slots: ghidra_out/cgsi_slots_aligned.tsv)
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006();
    virtual void NewScriptFrame() const;   // slot 7 (+0x1c) NewScriptFrame
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017();
    virtual bool MsgOnLevelLoaded(std::list<CCharString>& levels) const;   // slot 18 (+0x48) MsgOnLevelLoaded
    virtual void v019(); virtual void v020(); virtual void v021();
    virtual bool MsgOnRegionLoaded(CCharString& region) const;   // slot 22 (+0x58) MsgOnRegionLoaded
    virtual bool MsgOnRegionUnloaded(CCharString& region) const;   // slot 23 (+0x5c) MsgOnRegionUnloaded
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
    virtual void v272(); virtual void v273(); virtual void v274(); virtual void v275();
    virtual void ActivateQuest(const CCharString& quest) const;   // slot 276 (+0x450) ActivateQuest
    virtual void v277(); virtual void v278(); virtual void v279(); virtual void v280(); virtual void v281(); virtual void v282(); virtual void v283();
    virtual bool IsQuestActive(const CCharString& quest) const;   // slot 284 (+0x470) IsQuestActive
    virtual bool IsQuestRegistered(const CCharString& quest) const;   // slot 285 (+0x474) IsQuestRegistered
};

struct CQ_SunnyvaleMasterData {
    unsigned char _pad00[0x14];
    long  RandomEntityVillagerMax;                          // +0x14
    long  RandomEntityGuardMax;                             // +0x18
    long  RandomEntityBanditMax;                            // +0x1c
    long  RandomEntityVillagerCount;                        // +0x20
    long  RandomEntityGuardCount;                           // +0x24
    long  RandomEntityBanditCount;                          // +0x28
    float RandomEntityVillagerPercentage;                   // +0x2c
};
struct CScriptBase {                                        // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CPersonalScriptMainScript : CScriptBase {
    CGameScriptInterface*   Game;                           // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                    // +0x44
    void Main();
};
void CPersonalScriptMainScript::Main()
{
    while (!IsActiveThreadTerminating()) {
        {
            std::list<CCharString> levels;
            if (Game->MsgOnLevelLoaded(levels)) {
                if (IsActiveThreadTerminating())
                    {
#pragma inline_depth(0)
                        return;
#pragma inline_depth()
                    }
                for (std::list<CCharString>::iterator it = levels.begin(); it != levels.end(); ++it) {
                    if (IsActiveThreadTerminating())
                        return;
                    CCharString level = *it;
                    CCharString script = "PersonalScript_" + level;
                    if (Game->IsQuestRegistered(script) && !Game->IsQuestActive(script)) {
                        if (IsActiveThreadTerminating())
                            return;
                        Game->ActivateQuest(script);
                        CCharString message = CCharString("") + "Loading personal script " + script;
                    } else {
                        if (IsActiveThreadTerminating())
                            return;
                        if (!Game->IsQuestRegistered(script) || Game->IsQuestActive(script)) {
                            if (IsActiveThreadTerminating())
                                return;
                        }
                    }
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            CCharString unloadedRegion;
            if (Game->MsgOnRegionUnloaded(unloadedRegion)) {
                if (IsActiveThreadTerminating())
                    return;
                PMasterData->RandomEntityVillagerMax = 0;
                PMasterData->RandomEntityGuardMax = 0;
                PMasterData->RandomEntityBanditMax = 0;
                PMasterData->RandomEntityVillagerCount = 0;
                PMasterData->RandomEntityGuardCount = 0;
                PMasterData->RandomEntityBanditCount = 0;
            }
            CCharString loadedRegion;
            if (Game->MsgOnRegionLoaded(loadedRegion)) {
                if (IsActiveThreadTerminating())
                    return;
                CDefPointer def;
                CCharString defName = CCharString("REGION_") + CCharString(loadedRegion).ToUpper();
                if (GFGetPDefinitionManager()->GetDef(defName, def)) {
                    if (IsActiveThreadTerminating())
                        return;
                    PMasterData->RandomEntityVillagerMax = def->MaxPopulation[0];
                    PMasterData->RandomEntityGuardMax = def->MaxPopulation[1];
                    PMasterData->RandomEntityBanditMax = def->MaxPopulation[2];
                    PMasterData->RandomEntityVillagerCount = 0;
                    PMasterData->RandomEntityGuardCount = 0;
                    PMasterData->RandomEntityBanditCount = 0;
                    PMasterData->RandomEntityVillagerPercentage = 1.0f;
                }
                CCharString script = "RegionScript_" + loadedRegion;
                if (Game->IsQuestRegistered(script) && !Game->IsQuestActive(script)) {
                    if (IsActiveThreadTerminating())
                        return;
                    Game->ActivateQuest(script);
                    CCharString message = CCharString("") + "Loading region script " + script;
                } else {
                    if (IsActiveThreadTerminating())
                        return;
                    if (!Game->IsQuestRegistered(script)) {
                        if (IsActiveThreadTerminating())
                            return;
                        CCharString message = CCharString("") + "Cannot load script " + script + ": not registered";
                    } else if (Game->IsQuestActive(script)) {
                        if (IsActiveThreadTerminating())
                            return;
                        CCharString message = CCharString("") + "Script " + script + " already active";
                    }
                }
            }
        }
        Game->NewScriptFrame();
    }
}
